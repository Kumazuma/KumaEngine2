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

    STDMETHODIMP_(HRESULT __stdcall) CameraImpl::AddLayer(ILayer* layer)
    {
        if (layer == nullptr)
        {
            return E_POINTER;
        }
        std::lock_guard<decltype(lock_)> guard{ lock_ };
        if (auto it = layers_.find(layer); it == layers_.end())
        {
            layers_.emplace(layer, layer);
            return S_OK;
        }

        return E_INVALIDARG;
    }

    STDMETHODIMP_(HRESULT __stdcall) CameraImpl::RemoveLayer(ILayer* layer)
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

    STDMETHODIMP_(HRESULT __stdcall) CameraImpl::GetLayers(IKumaEngine_EntityIterator** iterator)
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

    STDMETHODIMP_(HRESULT __stdcall) CameraImpl::UpdateAndSwap()
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP_(HRESULT __stdcall) CameraImpl::Render(ID3D11DeviceContext4* deviceContext)
    {
        return E_NOTIMPL;
    }

    auto CameraImpl::GetLayers() -> std::unordered_map<ILayer*, ComPtr<ILayer>>&
    {
        return layers_;
    }
}
