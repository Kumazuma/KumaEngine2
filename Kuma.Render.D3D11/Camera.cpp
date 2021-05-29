#include "pch.h"
#include "Camera.h"
namespace KumaEngine::cpp
{
    STDMETHODIMP_(HRESULT __stdcall) Camera::GetWeakRef(IWeakRef** ref)
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP_(HRESULT __stdcall) Camera::GetRenderModule(IRenderModule** ppModule)
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP_(HRESULT __stdcall) Camera::AddLayer(ILayer* layer)
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

    STDMETHODIMP_(HRESULT __stdcall) Camera::RemoveLayer(ILayer* layer)
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

    STDMETHODIMP_(HRESULT __stdcall) Camera::GetLayers(IKumaEngine_EntityIterator** iterator)
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP_(HRESULT __stdcall) Camera::SetNear(float value)
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP_(float __stdcall) Camera::GetNear()
    {
        return 0.0f;
    }

    STDMETHODIMP_(HRESULT __stdcall) Camera::SetFar(float value)
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP_(float __stdcall) Camera::GetFar()
    {
        return 0.0f;
    }

    auto Camera::GetLayers() -> std::unordered_map<ILayer*, ComPtr<ILayer>>&
    {
        return layers_;
    }
}
