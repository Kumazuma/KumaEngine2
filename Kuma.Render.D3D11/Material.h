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
		std::unordered_map<unsigned int, std::vector<uint8_t>> constantBuffer;
		std::unordered_map<unsigned int, std::vector<ComPtr<ID3D11Surface>>> surfaces;
	};
	class D3D11Material : public ID3D11Material
	{
	public:
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
		std::atomic<D3D11MaterialContext*> shader_;
		D3D11MaterialContext* preparedShader_;
		
	};
}