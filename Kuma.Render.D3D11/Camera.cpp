#include "pch.h"
#include "Camera.h"
namespace KumaEngine::cpp
{
    STDMETHODIMP_(HRESULT __stdcall) CameraImpl::GetWeakRef(IWeakRef** ref)
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP_(HRESULT __stdcall) CameraImpl::GetRenderModule(IRenderModule** ppModule)
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP_(HRESULT __stdcall) CameraImpl::Add(IHasIterator* layer)
    {
        if (layer == nullptr)
        {
            return E_POINTER;
        }
        std::lock_guard<decltype(lock_)> guard{ lock_ };
        if (auto it = layers_.find(layer); it == layers_.end())
        {
            ComPtr<IWeakRef> ref;
            if (FAILED(layer->GetWeakRef(&ref)))
            {
                return E_FAIL;
            }
            layers_.emplace(layer, std::move(ref));
            return S_OK;
        }

        return E_INVALIDARG;
    }

    STDMETHODIMP_(HRESULT __stdcall) CameraImpl::Remove(IHasIterator* layer)
    {
        if (layer == nullptr)
        {
            return E_POINTER;
        }
        std::lock_guard<decltype(lock_)> guard{ lock_ };
        if (auto it = layers_.find(layer); it != layers_.end())
        {
            layers_.erase(it);
            return S_OK;
        }

        return E_INVALIDARG;
    }

    STDMETHODIMP_(HRESULT __stdcall) CameraImpl::GetIterator(REFIID itemType, IEntityIterator** iterator)
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP_(HRESULT __stdcall) CameraImpl::SetNear(float value)
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP_(float __stdcall) CameraImpl::GetNear()
    {
        return 0.0f;
    }

    STDMETHODIMP_(HRESULT __stdcall) CameraImpl::SetFar(float value)
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP_(float __stdcall) CameraImpl::GetFar()
    {
        return 0.0f;
    }

    STDMETHODIMP_(HRESULT __stdcall) CameraImpl::Render(D3D11RenderModule* renderModule, ID3D11DeviceContext4* deviceContext)
    {
        isDoneRender_ = true;
        return E_NOTIMPL;
    }
    void VisitAndGetRenderer(ComPtr<IGameObject> ptr, std::vector<ComPtr<ID3D11MeshRenderer>> list)
    {
        ComPtr<ID3D11MeshRenderer> renderer{};
        ComPtr<IEntityIterator> iterator{};
        if (SUCCEEDED(ptr->GetComponent(__uuidof(ID3D11MeshRenderer), &renderer)))
        {
            list.emplace_back(renderer);
        }
        if (FAILED(ptr->GetIterator(__uuidof(IGameObject) , &iterator)))
        {
            return;
        }
        while (SUCCEEDED(iterator->GetNext(&ptr)))
        {
            VisitAndGetRenderer(ptr, list);
        }
    }
    STDMETHODIMP_(HRESULT __stdcall) CameraImpl::Update(D3D11RenderModule* renderModule)
    {
        isDoneRender_ = false;
        renderers_.clear();
        defferedRenderers_.clear();
        forwardRenderers_.clear();

        auto it{ layers_.begin() };
        auto const end{ layers_.end() };
        //Collect All Renderer in layers
        while (it != end)
        {
            ComPtr<IHasIterator> layer{};
            ComPtr<IEntityIterator> iterator{};
            if (FAILED(it->second->LockEntity(__uuidof(IHasIterator), &layer)) || FAILED(layer->GetIterator(__uuidof(IGameObject), &iterator)))
            {
                it = layers_.erase(it);
                continue;
            }
            ComPtr<IGameObject> gameObject;
            while (SUCCEEDED(iterator->GetNext(&gameObject)))
            {
                VisitAndGetRenderer(gameObject, renderers_);
            }
            ++it;
        }
        auto device{ renderModule->GetDevice() };
        auto context{ renderModule->GetDeviceContext() };
        //메테리얼 셰이더에 따라 포워드이냐, 디퍼드로 나눈다. 
        for (auto& it : renderers_)
        {
            ComPtr<IMaterial> material;
            ComPtr<IShader> shader;
            if (FAILED(it->PrepareRender(device.Get(), context.Get())))
            {
                continue;
            }

            it->GetMaterial(&material);
            if (material == nullptr)
            {
                continue;
            }
            material->GetShader(&shader);
            if (shader == nullptr)
            {
                defferedRenderers_.emplace_back(it.Get());
            }
            else
            {
                forwardRenderers_.emplace_back(it.Get());
            }
            //만약에 서페이스가 렌더타겟이면 해당 렌더 타겟도 업데이트 해준다.
            ComPtr<IEntityIterator> surfaces;
            ComPtr<IRenderTargetSurface> surface;
            if (FAILED(material->GetIterator(__uuidof(IRenderTargetSurface), &surfaces)))
            {
                continue;
            }

            while (SUCCEEDED(surfaces->GetNext(&surface)))
            {
                ComPtr<ID3D11RenderTargetSurface> _surface;
                if (FAILED(surface.As(&_surface)))
                {
                    continue;
                }
                _surface->Update(renderModule);
            }
        }
        return S_OK;
    }

    STDMETHODIMP_(bool __stdcall) CameraImpl::IsDoneRender()
    {
        return isDoneRender_;
    }
}
