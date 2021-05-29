#pragma once
#include "KumaEngine2.h"
#include "helper.h"
#include<d3d11_4.h>
#include<dxgi1_5.h>
#include <wrl.h>
#include <thread>
namespace KumaEngine::cpp
{
	using namespace Microsoft::WRL;
	constexpr int SWAP_CHAIN_COUNT{ 2 };
	class D3D11RenderModule: public RefCountImpl<IRenderModule, IEntity, IUnknown>
	{
	public:
		D3D11RenderModule();
		~D3D11RenderModule();
		IFACEMETHODIMP Initialize(HWND hWnd, const ::GameRenderDesc* desc);
		IFACEMETHODIMP GetWeakRef(IWeakRef** ref);
		IFACEMETHODIMP Update();
		IFACEMETHODIMP CreateMeshRenderer(IMeshRenderer** meshRenderer);
		IFACEMETHODIMP CreateCamera(ICamera** camera);
		IFACEMETHODIMP LoadMeshFromFile(const wchar_t* meshId, const wchar_t* filePath);
		IFACEMETHODIMP LoadMeshFromMemory(const wchar_t* meshId, const wchar_t* ext, const uint8_t* bytes, size_t byteLength);
		IFACEMETHODIMP LoadTextureFromFile(const wchar_t* textureId, const wchar_t* filePath);
		IFACEMETHODIMP LoadTextureFromMemory(const wchar_t* textureId, const wchar_t* ext, const uint8_t* bytes, size_t byteLength);
		IFACEMETHODIMP SetMainCamera(ICamera* camera);
	private:
		void Process();
	private:
		bool isRunning_;
		ComPtr<CommonWeakRef> weakRef_;
		ComPtr<ID3D11Device5> device_;
		ComPtr<ID3D11DeviceContext4> deviceContext_;
		ComPtr<IDXGISwapChain3> swapChain_;
		ComPtr<ICamera> mainCamera_;
		ComPtr<ID3D11RenderTargetView> swapChainViews[SWAP_CHAIN_COUNT];
		ComPtr<ID3D11Texture2D> swapChainTextures[SWAP_CHAIN_COUNT];
		ComPtr<ID3D11Fence> fence_;
		HANDLE hFenceEvent_;
		UINT64 fenceValue_;
		INT currentBackBufferIndex;
		std::thread renderThread_;
	};
}