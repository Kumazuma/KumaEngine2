#include"pch.h"
#include"Renderer.h"
#include<functional>
#include<array>
namespace KumaEngine::cpp
{
	D3D11RenderModule::D3D11RenderModule():
		isRunning_{ false },
		currentBackBufferIndex{ 0 },
		fenceValue_{ 0 },
		hFenceEvent_{NULL},
		weakRef_{ }
	{
		*(CommonWeakRef**)&weakRef_ = new CommonWeakRef{ this };
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
		ICamera* camera = nextCamera_.exchange(nullptr);
		if (camera != nullptr)
		{
			camera->Release();
		}
	}
	IFACEMETHODIMP D3D11RenderModule::Initialize(HWND hWnd, const GameRenderDesc* desc)
	{
		HRESULT hr = S_OK;
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
		swapChainDesc.Width = desc->width;
		swapChainDesc.Height = desc->height;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferCount = SWAP_CHAIN_COUNT;
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.Stereo = false;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER | DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;
		swapChainDesc.Flags = 0;
		
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc{};
		fullScreenDesc.RefreshRate.Numerator = 0;
		fullScreenDesc.RefreshRate.Denominator = 1;
		fullScreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		fullScreenDesc.Windowed = !desc->fullscreen;
		fullScreenDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;

		ComPtr<IDXGIFactory5> dxgiFactory;
		if (FAILED(hr = CreateDXGIFactory2(0, __uuidof(IDXGIFactory5), &dxgiFactory)))
		{
			return E_FAIL;
		}
		D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_1;
		D3D_FEATURE_LEVEL actual_feature_level = D3D_FEATURE_LEVEL_11_1;

		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> context;
		hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG, &feature_level, 1, D3D11_SDK_VERSION, &device, &actual_feature_level, &context);
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

		hr = dxgiFactory->CreateSwapChainForHwnd(device_.Get(), hWnd, &swapChainDesc, &fullScreenDesc, nullptr, &swapChain);
		if (FAILED(hr))
		{
			return E_FAIL;
		}

		hr = swapChain.As(&swapChain_);
		if (FAILED(hr))
		{
			return E_FAIL;
		}
		
		swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), &swapChainTexture);
		device_->CreateRenderTargetView(swapChainTexture.Get(), nullptr, &swapChainView);
		device_->CreateFence(0, D3D11_FENCE_FLAG_NONE, __uuidof(fence_), &fence_);
		hFenceEvent_ = CreateEventW(nullptr, false, false, nullptr);
		renderThread_ = std::thread(&D3D11RenderModule::Process, this);
		isRunning_ = true;
		return S_OK;
	}
	
	void D3D11RenderModule::Process()
	{
		HRESULT hr{ S_OK };
		while (isRunning_)
		{
			if (mainCamera_ == nullptr)
			{
				SwitchToThread();
				continue;
			}
			
			deviceContext_->ClearRenderTargetView(swapChainView.Get(), std::array<float, 4>({ 0.f, 0.f, 1.f, 1.f }).data());

			swapChain_->Present(1, 0);
			const UINT64 fence = fenceValue_;
			deviceContext_->Signal(fence_.Get(), fence);
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
		if (ref == nullptr)
		{
			return E_POINTER;
		}
		*ref = weakRef_.Get();
		weakRef_->AddRef();
		return S_OK;
	}
	IFACEMETHODIMP D3D11RenderModule::Update()
	{
		HRESULT hr = S_OK;
		ComPtr<IEntityIterator> iter;
		ICamera* next = nextCamera_.exchange(nullptr);
		if (next != nullptr)
		{
			mainCamera_ = next;
			next->Release();
			next = nullptr;
		}
		if (FAILED(hr = mainCamera_->GetIterator(__uuidof(ILayer), &iter)))
		{
			return E_FAIL;
		}
		ComPtr<ILayer>layer;
		while (iter->GetNext(&layer) == S_OK)
		{
			ComPtr<IEntityIterator> gameObjectIter;
			ComPtr<IGameObject> gameObject;
			if (FAILED(hr = layer->GetIterator(__uuidof(IGameObject), &gameObjectIter)))
			{
				continue;
			}
			while (iter->GetNext(&gameObject) == S_OK)
			{
				ComPtr<IMeshRenderer> meshRenderer;
				if (FAILED(gameObject->GetComponent(__uuidof(IMeshRenderer), &meshRenderer)))
				{
					continue;
				}
				meshRenderers.emplace_back(meshRenderer);
			}
		}

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
		HRESULT hr = S_OK;
		ComPtr<ID3D11RenderComponent> renderCom;
		hr = camera->QueryInterface(__uuidof(ComPtr<ID3D11RenderComponent>), &renderCom);
		if (FAILED(hr))
		{
			OutputDebugStringA("Camera is not D3D11 Render Component On " __FUNCSIG__);
			return E_INVALIDARG;
		}
		ICamera* prev = nextCamera_.exchange(camera);
		if (prev != nullptr)
		{
			prev->Release();
		}
		camera->AddRef();
		return S_OK;
	}

	IFACEMETHODIMP D3D11RenderModule::GetMesh(IKumaEngine_Mesh** mesh)
	{
		return E_NOTIMPL;
	}

	IFACEMETHODIMP D3D11RenderModule::GetTexture(IKumaEngine_Surface** texture)
	{
		return E_NOTIMPL;
	}
}