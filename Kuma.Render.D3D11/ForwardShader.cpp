#include "pch.h"
#include "ForwardShader.h"
#include <d3dcompiler.h>
KumaEngine::cpp::D3D11ForwardShader::D3D11ForwardShader(ID3D11Device5* device, std::wstring_view path, std::string_view entryName)
{
	DWORD compileOption{};
	ComPtr<ID3DBlob> code;
	ComPtr<ID3DBlob> msg;
	HRESULT hr{};
#ifndef NDEBUG
	compileOption = D3DCOMPILE_DEBUG;
#endif
	hr = D3DCompileFromFile(
		path.data(),
		nullptr,
		nullptr,
		entryName.data(), "ps_5_0", compileOption, 0, &code, &msg);
	if (FAILED(hr))
	{
		if (msg == nullptr)
		{
			throw Exception{ "file not found" };
		}
		else
		{
			OutputDebugStringA((const char*)msg->GetBufferPointer());
			throw Exception{(const char*)msg->GetBufferPointer()};
		}
	}
	ComPtr<ID3D11ShaderReflection> reflection;
	if (FAILED(D3DReflect(code->GetBufferPointer(), code->GetBufferSize(), __uuidof(reflection), &reflection)))
	{
		throw Exception{ "Failed to shader code reflection" };
	}
	D3D11_SHADER_DESC shaderDesc{};
	reflection->GetDesc(&shaderDesc);
	const uint32_t cbufferCount{ shaderDesc.ConstantBuffers };
	const uint32_t boundResourceCount{ shaderDesc.BoundResources };
	for (int i = 0; i < boundResourceCount; ++i)
	{
		D3D11_SHADER_INPUT_BIND_DESC inputDesc{};
		reflection->GetResourceBindingDesc(i, &inputDesc);
		uint32_t registerIndex{ inputDesc.BindPoint };
		switch (inputDesc.Type)
		{
		case D3D_SIT_CBUFFER:
		{
			D3D11_SHADER_BUFFER_DESC bufferDesc{};
			ID3D11ShaderReflectionConstantBuffer* buffer{ reflection->GetConstantBufferByName(inputDesc.Name) };
			buffer->GetDesc(&bufferDesc);
			const uint32_t variableCount{ bufferDesc.Variables };
			constanceBufferSlots_.emplace(registerIndex, bufferDesc.Size);
			for (int j = 0; j < variableCount; ++j)
			{
				PropertyDescEx propertyDesc{};
				D3D11_SHADER_VARIABLE_DESC variableDesc{};
				D3D11_SHADER_TYPE_DESC typeDesc{};
				ID3D11ShaderReflectionVariable* variable{ buffer->GetVariableByIndex(j) };
				variable->GetDesc(&variableDesc);
				ID3D11ShaderReflectionType* type{ variable->GetType() };
				type->GetDesc(&typeDesc);
				switch (typeDesc.Type)
				{
				case D3D_SVT_INT:
					propertyDesc.kind = ShaderPropertyKind::Int;
					break;
				case D3D_SVT_FLOAT:
					propertyDesc.kind = ShaderPropertyKind::Float;
					break;
				}
				if (propertyDesc.kind == ShaderPropertyKind::None)
				{
					continue;
				}
				strncpy(propertyDesc.name, variableDesc.Name, 128);
				propertyDesc.dimension[0] = typeDesc.Rows;
				propertyDesc.dimension[1] = typeDesc.Columns;
				propertyDesc.prop.variable.cbufferSlotIndex = registerIndex;
				propertyDesc.prop.variable.offset = variableDesc.StartOffset;
				propertyDesc.prop.variable.size = variableDesc.Size;

				properties.push_back(propertyDesc);
			}
		}
			break;
		case D3D_SIT_TEXTURE:
		{
			PropertyDescEx propertyDesc{};
			D3D11_SHADER_TYPE_DESC typeDesc{};

			propertyDesc.kind = ShaderPropertyKind::Texture;
			propertyDesc.prop.texture.slotIndex = registerIndex;

			reflection->GetVariableByName(inputDesc.Name)->GetType()->GetDesc(&typeDesc);
			propertyDesc.dimension[0] = typeDesc.Rows;
			propertyDesc.dimension[1] = typeDesc.Columns;
			strncpy(propertyDesc.name, inputDesc.Name, 128);
			properties.push_back(propertyDesc);
		}
			break;
		case D3D_SIT_SAMPLER:
			samplerSlots_.emplace(registerIndex);
		}
		
	}
	if (FAILED(device->CreatePixelShader(code->GetBufferPointer(), code->GetBufferSize(), nullptr, &shader_)))
	{
		throw Exception{ "Failed Create Pixel Shader" };
	}
}

KumaEngine::cpp::D3D11ForwardShader::D3D11ForwardShader(D3D11ForwardShader&& rhs) noexcept
{
}

STDMETHODIMP_(HRESULT __stdcall) KumaEngine::cpp::D3D11ForwardShader::GetWeakRef(IKumaEngine_WeakRef** ref)
{
	return E_NOTIMPL;
}

STDMETHODIMP_(uint32_t __stdcall) KumaEngine::cpp::D3D11ForwardShader::GetPropertyCount()
{
	return properties.size();
}

STDMETHODIMP_(HRESULT __stdcall) KumaEngine::cpp::D3D11ForwardShader::GetPropertyDesc(uint32_t index, KumaEngine_ShaderPropertyDesc* desc)
{
	if (desc == nullptr)
	{
		return E_POINTER;
	}
	if (properties.size() <= index)
	{
		return E_INVALIDARG;
	}

	memcpy(&desc, &properties[index], sizeof(KumaEngine_ShaderPropertyDesc));
	return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) KumaEngine::cpp::D3D11ForwardShader::GetPropertyDescEx(uint32_t index, PropertyDescEx* desc)
{
	if (desc == nullptr)
	{
		return E_POINTER;
	}
	if (properties.size() <= index)
	{
		return E_INVALIDARG;
	}

	memcpy(&desc, &properties[index], sizeof(PropertyDescEx));
	return S_OK;
}

STDMETHODIMP_(uint32_t __stdcall) KumaEngine::cpp::D3D11ForwardShader::GetConstantBufferSize(uint32_t index)
{
	if (const auto it = constanceBufferSlots_.find(index); it != constanceBufferSlots_.end())
	{
		return it->second;
	}
	return 0;
}
