#include "pch.h"
#include "Material.h"
#undef min
#undef max

namespace KumaEngine::cpp
{
    STDMETHODIMP_(HRESULT __stdcall) D3D11Material::GetWeakRef(IKumaEngine_WeakRef** ref)
    {
        return E_NOTIMPL;
    }

    D3D11Material::D3D11Material():
        preparedIdx_{ 0 },
        commitIdx{ 0 },
        currentBufferIndex_{ 0 }
    {
    }

    STDMETHODIMP_(HRESULT __stdcall) D3D11Material::PrepareRender(ID3D11Device5* device, ID3D11DeviceContext4* context)
    {
        if (preparedIdx_ >= commitIdx)
        {
            return S_OK;
        }
        std::unordered_map<uint32_t, ComPtr<ID3D11Buffer>> preparedBuffer;
        int nextBufferIndex{ currentBufferIndex_ == 1 ? 0 : 1 };
        if (preparedShader_ != context_->shader)
        {
            std::vector<ComPtr<ID3D11Buffer>> buffers{};
            buffers.reserve(constantBuffer_[nextBufferIndex].size());
            for (auto& it : constantBuffer_[nextBufferIndex])
            {
                buffers.emplace_back(std::move(it.second));
            }
            constantBuffer_[nextBufferIndex].clear();
            for (auto& it : context_->constantBuffers)
            {
                auto vecIt{ buffers.begin() };
                bool found{ false };
                while (vecIt != buffers.end())
                {
                    D3D11_BUFFER_DESC desc{};
                    (*vecIt)->GetDesc(&desc);
                    if (it.second.size() <= desc.ByteWidth)
                    {
                        found = true;
                        break;
                    }
                    ++vecIt;
                }
                ComPtr<ID3D11Buffer> buffer;
                if (found)
                {
                    buffer = std::move(*vecIt);
                    buffers.erase(vecIt);
                }
                else
                {
                    D3D11_BUFFER_DESC desc{};
                    desc.Usage = D3D11_USAGE_DYNAMIC;
                    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
                    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                    desc.ByteWidth = (it.second.size() / 4 + 1) * 4;
                    if (FAILED(device->CreateBuffer(&desc, nullptr, &buffer)))
                    {
                        assert(("FAILED(device->CreateBuffer(&desc, nullptr, &buffer))",false));
                    }
                }
                preparedBuffer.emplace(it.first, buffer);
            }
        }
        else
        {
            preparedBuffer = std::move(constantBuffer_[nextBufferIndex]);
        }

        for (auto& it : preparedBuffer)
        {
            const auto& data{ context_->constantBuffers[it.first] };
            D3D11_MAPPED_SUBRESOURCE mappedSubResource{};
            context->Map(it.second.Get(), 0, D3D11_MAP_WRITE, 0, &mappedSubResource);
            memcpy(mappedSubResource.pData, data.data(), data.size());
            context->Unmap(it.second.Get(), 0);
        }

        preparedSufaces_[nextBufferIndex] = context_->surfaces;
        constantBuffer_[nextBufferIndex] = std::move(preparedBuffer);
        currentBufferIndex_ = nextBufferIndex;

        return S_OK;
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
        if (key == nullptr || value == nullptr || size == 0)
        {
            return E_INVALIDARG;
        }

        auto& properies{ context_->properties };
        if (auto it = properies.find(key);  it != properies.end())
        {
            auto const& desc{ it->second };
            if (desc.kind != ShaderPropertyKind::Int)
            {
                return E_FAIL;
            }
            uint32_t const cpySize{ std::min(desc.prop.variable.size, size) };
            auto& cbuffer{ context_->constantBuffers[desc.prop.variable.cbufferSlotIndex] };
            memcpy(&cbuffer[desc.prop.variable.offset], value, cpySize);
            ++commitIdx;
            return S_OK;
        }
        
        return E_INVALIDARG;
    }

    STDMETHODIMP_(uint32_t __stdcall) D3D11Material::GetPropertyInt(const char* key, int32_t* value, uint32_t size)
    {
        if (key == nullptr || value == nullptr || size == 0)
        {
            return 0;
        }

        auto& properies{ context_->properties };
        if (auto it = properies.find(key);  it != properies.end())
        {
            auto const& desc{ it->second };
            if (desc.kind != ShaderPropertyKind::Int)
            {
                return 0;
            }
            uint32_t const cpySize{ std::min(desc.prop.variable.size, size) };
            auto& cbuffer{ context_->constantBuffers[desc.prop.variable.cbufferSlotIndex] };
            memcpy(value, &cbuffer[desc.prop.variable.offset], cpySize);
            return cpySize;
        }

        return 0;
    }

    STDMETHODIMP_(HRESULT __stdcall) D3D11Material::SetPropertyFloat(const char* key, const float* value, uint32_t size)
    {
        if (key == nullptr || value == nullptr || size == 0)
        {
            return E_INVALIDARG;
        }

        auto& properies{ context_->properties };
        if (auto it = properies.find(key);  it != properies.end())
        {
            auto const& desc{ it->second };
            if (desc.kind != ShaderPropertyKind::Float)
            {
                return E_INVALIDARG;
            }
            uint32_t const cpySize{ std::min(desc.prop.variable.size, size) };
            auto& cbuffer{ context_->constantBuffers[desc.prop.variable.cbufferSlotIndex] };
            memcpy(&cbuffer[desc.prop.variable.offset], value, cpySize);
            ++commitIdx;
            return S_OK;
        }

        return E_FAIL;
    }

    STDMETHODIMP_(uint32_t __stdcall) D3D11Material::GetPropertyFloat(const char* key, float* value, uint32_t size)
    {
        if (key == nullptr || value == nullptr || size == 0)
        {
            return 0;
        }

        auto& properies{ context_->properties };
        if (auto it = properies.find(key);  it != properies.end())
        {
            auto const& desc{ it->second };
            if (desc.kind != ShaderPropertyKind::Float)
            {
                return 0;
            }
            uint32_t const cpySize{ std::min(desc.prop.variable.size, size) };
            auto& cbuffer{ context_->constantBuffers[desc.prop.variable.cbufferSlotIndex] };
            memcpy(value, &cbuffer[desc.prop.variable.offset], cpySize);
            return cpySize;
        }

        return 0;
    }

    STDMETHODIMP_(HRESULT __stdcall) D3D11Material::SetPropertyTexture(const char* key, ISurface** value, uint32_t size)
    {
        if (key == nullptr || value == nullptr || size == 0)
        {
            return E_INVALIDARG;
        }

        auto& properies{ context_->properties };
        if (auto it = properies.find(key);  it != properies.end())
        {
            auto const& desc{ it->second };
            if (desc.kind != ShaderPropertyKind::Texture)
            {
                return E_FAIL;
            }
            uint32_t slotCount{ std::min(desc.dimension[0], 1u) * std::min(desc.dimension[1], 1u) };
            uint32_t const cpySize{ std::min(slotCount, size) };
            auto& textures{ context_->surfaces[desc.prop.texture.slotIndex] };
            textures.clear();
            for (uint32_t i = 0; i < cpySize; ++i)
            {
                ComPtr<ID3D11Surface> surface;
                value[i]->QueryInterface(__uuidof(surface), &surface);
                textures.push_back(surface);
            }
            ++commitIdx;
            return S_OK;
        }

        return E_INVALIDARG;
    }

    STDMETHODIMP_(uint32_t __stdcall) D3D11Material::GetPropertyTexture(const char* key, IKumaEngine_Surface** value, uint32_t size)
    {
        if (key == nullptr || value == nullptr || size == 0)
        {
            return 0;
        }

        auto& properies{ context_->properties };
        if (auto it = properies.find(key);  it != properies.end())
        {
            auto const& desc{ it->second };
            if (desc.kind != ShaderPropertyKind::Texture)
            {
                return 0;
            }
            uint32_t slotCount{ std::min(desc.dimension[0], 1u) * std::min(desc.dimension[1], 1u) };
            auto& textures{ context_->surfaces[desc.prop.texture.slotIndex] };
            uint32_t const cpySize{ std::min(static_cast<uint32_t>(textures.size()), size) };
            for (uint32_t i = 0; i < cpySize; ++i)
            {
                value[i] = textures[i].Get();
                textures[i]->AddRef();
            }
            return cpySize;
        }

        return 0;
    }

    STDMETHODIMP_(HRESULT __stdcall) D3D11Material::GetIterator(REFIID itemType, IKumaEngine_EntityIterator** iterator)
    {
        return E_NOTIMPL;
    }

}
