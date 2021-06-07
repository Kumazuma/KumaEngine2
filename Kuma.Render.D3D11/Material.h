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
#include<unordered_map>
namespace KumaEngine::cpp
{
	using namespace Microsoft::WRL;
	struct D3D11MaterialContext
	{
		ComPtr<ID3D11Shader> shader;
		std::unordered_map<std::string, PropertyDescEx> properties;
		std::unordered_map<unsigned int, std::vector<uint8_t>> constantBuffers;
		std::unordered_map<unsigned int, std::vector<ComPtr<ID3D11Surface>>> surfaces;
	};
	class D3D11Material : public ID3D11Material
	{
	public:
		D3D11Material();
		STDMETHOD(GetWeakRef(IWeakRef** ref));
		STDMETHOD(PrepareRender());
		STDMETHOD(SetShader(IShader* shader));
		STDMETHOD(GetShader(IShader** shader));
		STDMETHOD(SetPropertyInt(const char* key, const int32_t* value, uint32_t size)) ;
		STDMETHOD_(uint32_t, GetPropertyInt(const char* key, int32_t* value, uint32_t size)) ;
		STDMETHOD(SetPropertyFloat(const char* key, const float* value, uint32_t size)) ;
		STDMETHOD_(uint32_t, GetPropertyFloat(const char* key, float* value, uint32_t size)) ;
		STDMETHOD(SetPropertyTexture(const char* key, ISurface** value, uint32_t size)) ;
		STDMETHOD_(uint32_t, GetPropertyTexture(const char* key, ISurface** value, uint32_t size)) ;
		STDMETHOD(GetIterator(REFIID itemType, IKumaEngine_EntityIterator** iterator));
	private:
		D3D11MaterialContext* context_;
		ComPtr<ID3D11Shader> preparedShader_;
		uint64_t preparedIdx_;
		uint64_t commitIdx;
		std::unordered_map<uint8_t, ComPtr<ID3D11Buffer>> constantBuffer_;
	};
}