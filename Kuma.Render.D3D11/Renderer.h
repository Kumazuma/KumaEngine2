#pragma once
#include "KumaEngine2.h"
#include "helper.h"
#include<d3d11_4.h>
#include<dxgi1_5.h>
#include <wrl.h>
#include <thread>
#include <vector>
#include <mutex>
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
		IFACEMETHODIMP GetMesh(IKumaEngine_Mesh** mesh);
		IFACEMETHODIMP GetTexture(IKumaEngine_Surface** texture);
	private:
		void Process();
	private:
		bool isRunning_;
		ComPtr<CommonWeakRef> weakRef_;
		ComPtr<ID3D11Device5> device_;
		ComPtr<ID3D11DeviceContext4> deviceContext_;
		ComPtr<IDXGISwapChain3> swapChain_;
		ComPtr<ICamera> mainCamera_;
		std::atomic<ICamera*> updatedCamera_;
		std::atomic<ICamera*> nextCamera_;
		ComPtr<ID3D11RenderTargetView> swapChainView;
		ComPtr<ID3D11Texture2D> swapChainTexture;
		ComPtr<ID3D11Fence> fence_;
		std::vector<ComPtr<IMeshRenderer>> meshRenderers;
		HANDLE hFenceEvent_;
		UINT64 fenceValue_;
		INT currentBackBufferIndex;
		std::thread renderThread_;
	};

	// {0CAD4CF4-6190-4F4F-93CC-10B5712267C8}
	DEFINE_GUID(IID_D3D11_RENDER_COM,
		0xcad4cf4, 0x6190, 0x4f4f, 0x93, 0xcc, 0x10, 0xb5, 0x71, 0x22, 0x67, 0xc8);
	MIDL_INTERFACE("0CAD4CF4-6190-4F4F-93CC-10B5712267C8") ID3D11Camera: ICamera
	{	
		STDMETHOD(Render(D3D11RenderModule* renderModule, ID3D11DeviceContext4* deviceContext)) PURE;
		STDMETHOD(Update()) PURE;
		STDMETHOD_(bool, IsDoneRender()) PURE;
	};
	
	// {75BF8137-CD3F-4CBE-AF16-1CE7CAC19486}
	DEFINE_GUID(IID_D3D11_MESH_RENDERER ,
		0x75bf8137, 0xcd3f, 0x4cbe, 0xaf, 0x16, 0x1c, 0xe7, 0xca, 0xc1, 0x94, 0x86);
	MIDL_INTERFACE("0CAD4CF4-6190-4F4F-93CC-10B5712267C8") ID3D11MeshRenderer: IMeshRenderer
	{
		
	};

	// {74634E34-1CDF-4215-82A2-0DECF337AF2F}
	DEFINE_GUID(IID_RENDERABLE,
		0x74634e34, 0x1cdf, 0x4215, 0x82, 0xa2, 0xd, 0xec, 0xf3, 0x37, 0xaf, 0x2f);

	MIDL_INTERFACE("0CAD4CF4-6190-4F4F-93CC-10B5712267C8") ID3D11Mesh: IMesh
	{
		STDMETHOD(GetVertexBuffer(ID3D11Buffer** ppBuffer)) PURE;
		STDMETHOD(GetIndexBuffer(ID3D11Buffer** ppBuffer)) PURE;
		STDMETHOD_(UINT, GetIndexCount()) PURE;
		STDMETHOD_(UINT, GetVertexCount()) PURE;
	};
}