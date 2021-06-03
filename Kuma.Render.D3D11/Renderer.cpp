#include"pch.h"
#include"Renderer.h"
#include"Camera.h"
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
		ICamera* camera{ nextCamera_.exchange(nullptr) };
		ICamera* updateCamera{ updatedCamera_.exchange(nullptr) };
		if (camera != nullptr)
		{
			camera->Release();
		}
		if (updateCamera != nullptr)
		{
			updateCamera->Release();
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
		ID3D11Camera* nowCamera{ nullptr };

		while (isRunning_)
		{
			ID3D11Camera* updatedCamera{ updatedCamera_.exchange(nullptr) };
			if (updatedCamera != nullptr)
			{
				if (nowCamera != nullptr)
				{
					nowCamera->Release();
				}
				nowCamera = updatedCamera;
			}
			if (nowCamera == nullptr)
			{
				SwitchToThread();
				continue;
			}
			
			deviceContext_->ClearRenderTargetView(swapChainView.Get(), std::array<float, 4>({ 0.f, 0.f, 1.f, 1.f }).data());
			
			nowCamera->Render(this, deviceContext_.Get());

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
		if (nowCamera != nullptr)
		{
			nowCamera->Release();
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
	//이 메소드에서는 새로운 카메라가 메인 카메라로 세팅 되면 교체해주고 메인 카메라의 업데이트를 호출합니다.
	IFACEMETHODIMP D3D11RenderModule::Update()
	{
		HRESULT hr = S_OK;
		ComPtr<IEntityIterator> iter;
		ICamera* next = nextCamera_.exchange(nullptr);
		if (next != nullptr)
		{
			ComPtr<ID3D11Camera> camera;
			HRESULT hr = S_OK;
			hr = next->QueryInterface(__uuidof(ID3D11Camera), &camera);
			next->Release();
			if (FAILED(hr))
			{
				return E_FAIL;
			}
			mainCamera_ = std::move(camera);
		}
		if (FAILED(mainCamera_->Update(this)))
		{
			return E_FAIL;
		}
		mainCamera_->AddRef();
		ID3D11Camera* prevUpdatedCamera = updatedCamera_.exchange(mainCamera_.Get());
		if (prevUpdatedCamera != nullptr)
		{
			prevUpdatedCamera->Release();
		}
		return S_OK;
	}
	IFACEMETHODIMP D3D11RenderModule::CreateMeshRenderer(IMeshRenderer** meshRenderer)
	{
		return E_NOTIMPL;
	}

	STDMETHODIMP_(HRESULT __stdcall) D3D11RenderModule::CreateCamera(ICamera** camera)
	{
		if (camera == nullptr)
		{
			return E_POINTER;
		}
		Camera* cam{ new(std::nothrow) Camera{} };
		if (cam == nullptr)
		{
			return E_FAIL;
		}
		*camera = cam;
		return S_OK;
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
		if (camera == nullptr)
		{
			return E_POINTER;
		}

		HRESULT hr = S_OK;
		ComPtr<ID3D11Camera> renderCom;
		hr = camera->QueryInterface(__uuidof(ComPtr<ID3D11Camera>), &renderCom);
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
	ComPtr<ID3D11Device5> D3D11RenderModule::GetDevice() const
	{
		return device_;
	}
	ComPtr<ID3D11DeviceContext4> D3D11RenderModule::GetDeviceContext() const
	{
		return deviceContext_;
	}
}