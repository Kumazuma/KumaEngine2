#pragma once
#include "KumaEngine2.h"
#include "helper.h"
#include <d3d11.h>
#include <wrl.h>
#include <thread>
namespace KumaEngine::cpp
{
	using namespace Microsoft::WRL;

	class D3D11RenderModule: public RefCountImpl<IRenderModule, IEntity, IUnknown>
	{
	public:
		D3D11RenderModule();
		~D3D11RenderModule();
		STDMETHOD(Initialize(HWND hWnd, const ::GameRenderDesc* desc));
		STDMETHOD(GetWeakRef(IWeakRef** ref));
		STDMETHOD(Update());
		STDMETHOD(CreateMeshRenderer(IMeshRenderer** meshRenderer));
		STDMETHOD(CreateCamera(ICamera** camera));
		STDMETHOD(LoadMeshFromFile(const wchar_t* meshId, const wchar_t* filePath));
		STDMETHOD(LoadMeshFromMemory(const wchar_t* meshId, const wchar_t* ext, const uint8_t* bytes, size_t byteLength));
		STDMETHOD(LoadTextureFromFile(const wchar_t* textureId, const wchar_t* filePath));
		STDMETHOD(LoadTextureFromMemory(const wchar_t* textureId, const wchar_t* ext, const uint8_t* bytes, size_t byteLength));
		STDMETHOD(SetMainCamera(ICamera* camera));

	private:
		void Process();
	private:
		bool isRunning_;
		ComPtr<CommonWeakRef> weakRef_;
		ComPtr<ID3D11Device> device_;
		ComPtr<ID3D11DeviceContext> deviceContext_;
		std::thread renderThread_;
	};
}