#include "pch.h"
#include "Material.h"
namespace KumaEngine::cpp
{
    STDMETHODIMP_(HRESULT __stdcall) D3D11Material::GetWeakRef(IKumaEngine_WeakRef** ref)
    {
        return E_NOTIMPL;
    }

    D3D11Material::D3D11Material():
        preparedIdx_{ 0 },
        commitIdx{ 0 }
    {
    }

    STDMETHODIMP_(HRESULT __stdcall) D3D11Material::PrepareRender()
    {
        if (preparedIdx_ >= commitIdx)
        {
            return S_OK;
        }
        return E_NOTIMPL;
    }

    STDMETHODIMP_(HRESULT __stdcall) D3D11Material::SetShader(IShader* shader)
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
        D3D11MaterialContext* newContext{ new D3D11MaterialContext{} };
        newContext->shader = cur;
        cur->Release();
        D3D11MaterialContext* prev = context_;
        
        uint32_t propertyCount{ cur->GetPropertyCount() };
        for (uint32_t i = 0; i < propertyCount; ++i)
        {
            PropertyDescEx desc{};
            cur->GetPropertyDescEx(i, &desc);
            newContext->properties.emplace(desc.name, desc);
            if (desc.kind == ShaderPropertyKind::Texture)
            {
                continue;
            }
            if (auto it = newContext->constantBuffers.find(desc.prop.variable.cbufferSlotIndex); it == newContext->constantBuffers.end())
            {
                newContext->constantBuffers[i].resize(cur->GetConstantBufferSize(i), 0);
            }
        }

        context_ = newContext;
        if (prev != nullptr)
        {
            delete prev;
        }
        ++commitIdx;
        return S_OK;
    }

    STDMETHODIMP_(HRESULT __stdcall) D3D11Material::GetShader(IShader** shader)
    {
        if (shader == nullptr)
        {
            return E_POINTER;
        }
        D3D11MaterialContext* context = context_;
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
        auto const& properies{ context_->properties };
        
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
