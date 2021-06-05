#pragma once
#include"KumaEngine2.h"
#include"helper.h"
#include"Renderer.h"
#include<d3d11_4.h>
#include<DirectXMath.h>
#include<wrl.h>
#include<array>
#include<atomic>
#include<string_view>
#include<unordered_set>
namespace KumaEngine::cpp
{
	using namespace Microsoft::WRL;
	using namespace DirectX;
	class Exception: public std::exception
	{
	public:
		Exception(std::string&& msg):
			exception_{std::move(msg)}
		{
			
		}
		_NODISCARD virtual char const* what() const override
		{
			return exception_.c_str();
		}
	private:
		std::string exception_;
	};
	struct PropertyDescEx: public KumaEngine_ShaderPropertyDesc
	{
	/*
	* extended information
	*/
		union
		{
			struct
			{
				uint32_t cbufferSlotIndex;
				uint32_t offset;
				uint32_t size;
			} variable;
			struct
			{
				uint32_t slotIndex;
			} texture;
		} prop;
	};
	class D3D11ForwardShader : public RefCountImpl<ID3D11Shader, IShader, IEntity, IUnknown>
	{
	public:
		D3D11ForwardShader(ID3D11Device5* device, std::wstring_view path, std::string_view entryName);
		D3D11ForwardShader(D3D11ForwardShader&&) noexcept;
		STDMETHOD(GetWeakRef(IKumaEngine_WeakRef** ref));
		STDMETHOD_(uint32_t, GetPropertyCount());
		STDMETHOD(GetPropertyDesc(uint32_t index, KumaEngine_ShaderPropertyDesc* desc));
	private:
		std::vector<PropertyDescEx> properties_;
		std::unordered_set<uint32_t> samplerSlots_;
		std::unordered_set<uint32_t> constanceBufferSlots_;
		ComPtr<ID3D11PixelShader> shader_;
	};
}