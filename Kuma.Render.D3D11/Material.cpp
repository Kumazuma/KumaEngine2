#include "pch.h"
#include "Material.h"
namespace KumaEngine::cpp
{
    STDMETHODIMP_(HRESULT __stdcall) D3D11Material::GetWeakRef(IKumaEngine_WeakRef** ref)
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP_(HRESULT __stdcall) D3D11Material::PrepareRender()
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP_(HRESULT __stdcall) D3D11Material::SetShader(IKumaEngine_Shader* shader)
    {
        if (shader == nullptr)
        {
            return E_POINTER;
        }

        ID3D11Shader* cur;
        if (FAILED(shader->QueryInterface(&cur)))
        {
            return E_INVALIDARG;
        }
        D3D11MaterialContext* newContext = new D3D11MaterialContext{};
        newContext->shader = cur;
        cur->Release();
        D3D11MaterialContext* prev = shader_.exchange(newContext);
        if (prev != nullptr)
        {
            delete prev;
        }
        return S_OK;
    }

    STDMETHODIMP_(HRESULT __stdcall) D3D11Material::GetShader(IKumaEngine_Shader** shader)
    {
        if (shader == nullptr)
        {
            return E_POINTER;
        }
        D3D11MaterialContext* context = shader_.load();
        if (context == nullptr)
        {
            return E_FAIL;
        }
        *shader = context->shader.Get();
        context->shader->AddRef();
        return S_OK;
    }

    STDMETHODIMP_(HRESULT __stdcall) D3D11Material::SetPropertyInt(const char* key, const int32_t* value, uint32_t size)
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP_(uint32_t __stdcall) D3D11Material::GetPropertyInt(const char* key, int32_t* value, uint32_t size)
    {
        return uint32_t();
    }

    STDMETHODIMP_(HRESULT __stdcall) D3D11Material::SetPropertyFloat(const char* key, const float* value, uint32_t size)
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP_(uint32_t __stdcall) D3D11Material::GetPropertyFloat(const char* key, float* value, uint32_t size)
    {
        return uint32_t();
    }

    STDMETHODIMP_(HRESULT __stdcall) D3D11Material::SetPropertyTexture(const char* key, IKumaEngine_Surface** value, uint32_t size)
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP_(uint32_t __stdcall) D3D11Material::GetPropertyTexture(const char* key, IKumaEngine_Surface** value, uint32_t size)
    {
        return uint32_t();
    }

    STDMETHODIMP_(HRESULT __stdcall) D3D11Material::GetIterator(REFIID itemType, IKumaEngine_EntityIterator** iterator)
    {
        return E_NOTIMPL;
    }

}
