#include"pch.h"
#include"Renderer.h"
#include<functional>
#include<array>
namespace KumaEngine::cpp
{
	D3D11RenderModule::D3D11RenderModule():
		isRunning_{ false },
		currentBackBufferIndex{ 0 },
		fenceValue_{ 0 }
	{

	}
	D3D11RenderModule::~D3D11RenderModule()
	{
		isRunning_ = false;
		if (renderThread_.joinable())
		{
			renderThread_.join();
		}
		weakRef_->Unlink();
		CloseHandle(hFenceEvent_);
	}
	IFACEMETHODIMP D3D11RenderModule::Initialize(HWND hWnd, const GameRenderDesc* desc)
	{
		HRESULT hr = S_OK;
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
		swapChainDesc.Width = desc->width;
		swapChainDesc.Height = desc->height;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferCount = SWAP_CHAIN_COUNT;
		swapChainDesc.Scaling = DXGI_SCALING_ASPECT_RATIO_STRETCH;
		swapChainDesc.Stereo = false;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		swapChainDesc.SampleDesc.Count = 0;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER | DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_FULLSCREEN_VIDEO;
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc{};
		if (desc->fullscreen)
		{
			fullScreenDesc.RefreshRate.Numerator = 0;
			fullScreenDesc.RefreshRate.Denominator = 1;
			fullScreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			fullScreenDesc.Windowed = false;
			fullScreenDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
		}
		ComPtr<IDXGIFactory5> dxgiFactory;
		if (FAILED(hr = CreateDXGIFactory2(0, __uuidof(IDXGIFactory5), &dxgiFactory)))
		{
			return E_FAIL;
		}
		D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_1;

		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> context;
		hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG, &feature_level, 1, D3D11_SDK_VERSION, &device, &feature_level, &context);
		if (FAILED(hr))
		{
			return E_FAIL;
		}
		
		hr = device.As(&device_);
		if (FAILED(hr))
		{
			return E_FAIL;
		}
		
		hr = context.As(&deviceContext_);
		if (FAILED(hr))
		{
			return E_FAIL;
		}

		ComPtr<IDXGISwapChain1> swapChain;

		hr = dxgiFactory->CreateSwapChainForHwnd(device_.Get(), hWnd, &swapChainDesc, desc->fullscreen ? &fullScreenDesc : nullptr, nullptr, &swapChain);
		if (FAILED(hr))
		{
			return E_FAIL;
		}

		hr = swapChain.As(&swapChain_);
		if (FAILED(hr))
		{
			return E_FAIL;
		}
		
		swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), &swapChainTextures[0]);
		swapChain_->GetBuffer(1, __uuidof(ID3D11Texture2D), &swapChainTextures[1]);
		device_->CreateRenderTargetView(swapChainTextures[0].Get(), nullptr, &swapChainViews[0]);
		device_->CreateRenderTargetView(swapChainTextures[1].Get(), nullptr, &swapChainViews[1]);
		device_->CreateFence(0, D3D11_FENCE_FLAG_NONE, __uuidof(fence_), &fence_);
		hFenceEvent_ = CreateEventW(nullptr, false, false, nullptr);
		renderThread_ = std::thread(&D3D11RenderModule::Process, this);
		return S_OK;
	}
	
	void D3D11RenderModule::Process()
	{
		while (isRunning_)
		{
			if (mainCamera_ == nullptr)
			{
				SwitchToThread();
				continue;
			}
			deviceContext_->ClearRenderTargetView(swapChainViews[0].Get(), std::array<float, 4>({ 0.f, 0.f, 1.f, 1.f }).data());

			swapChain_->Present(1, 0);
			const UINT64 fence = fenceValue_;
			++fenceValue_;
			// Wait until the previous frame is finished.
			if (fence_->GetCompletedValue() < fence)
			{
				fence_->SetEventOnCompletion(fence, hFenceEvent_);
				WaitForSingleObject(hFenceEvent_, INFINITE);
			}
			currentBackBufferIndex = swapChain_->GetCurrentBackBufferIndex();
		}
	}

	IFACEMETHODIMP D3D11RenderModule::GetWeakRef(IWeakRef** ref)
	{
		return E_NOTIMPL;
	}
	IFACEMETHODIMP D3D11RenderModule::Update()
	{
		return E_NOTIMPL;
	}
	IFACEMETHODIMP D3D11RenderModule::CreateMeshRenderer(IMeshRenderer** meshRenderer)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP_(HRESULT __stdcall) D3D11RenderModule::CreateCamera(ICamera** camera)
	{
		return E_NOTIMPL;
	}
	IFACEMETHODIMP D3D11RenderModule::LoadMeshFromFile(const wchar_t* meshId, const wchar_t* filePath)
	{
		return E_NOTIMPL;
	}
	IFACEMETHODIMP D3D11RenderModule::LoadMeshFromMemory(const wchar_t* meshId, const wchar_t* ext, const uint8_t* bytes, size_t byteLength)
	{
		return E_NOTIMPL;
	}
	IFACEMETHODIMP D3D11RenderModule::LoadTextureFromFile(const wchar_t* textureId, const wchar_t* filePath)
	{
		return E_NOTIMPL;
	}
	IFACEMETHODIMP D3D11RenderModule::LoadTextureFromMemory(const wchar_t* textureId, const wchar_t* ext, const uint8_t* bytes, size_t byteLength)
	{
		return E_NOTIMPL;
	}

	STDMETHODIMP_(HRESULT __stdcall) D3D11RenderModule::SetMainCamera(ICamera* camera)
	{
		return E_NOTIMPL;
	}

}