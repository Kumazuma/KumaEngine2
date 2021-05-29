#include"pch.h"
#include"Renderer.h"
#include<functional>
namespace KumaEngine::cpp
{
	D3D11RenderModule::D3D11RenderModule():
		isRunning_{ false }
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
	}
	IFACEMETHODIMP D3D11RenderModule::Initialize(HWND hWnd, const GameRenderDesc* desc)
	{
		renderThread_ = std::thread(&D3D11RenderModule::Process, this);
		return E_NOTIMPL;
	}
	
	void D3D11RenderModule::Process()
	{
		while (isRunning_)
		{

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