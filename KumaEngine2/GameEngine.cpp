#include "pch.h"
#include "GameEngine.h"
#include "Layer.h"

namespace KumaEngine::cpp
{
	GameEngine::GameEngine()
	{
		weakRef_ = new CommonWeakRef{ this };
		weakRef_->Release();
	}

	GameEngine::~GameEngine()
	{
		weakRef_->Unlink();
	}

	STDMETHODIMP_(HRESULT __stdcall) GameEngine::GetWeakRef(IWeakRef** ref)
	{
		if (ref == nullptr)
		{
			return E_POINTER;
		}
		*ref = weakRef_.Get();
		weakRef_->AddRef();
		return S_OK;
	}

	STDMETHODIMP_(HRESULT __stdcall) GameEngine::Initialize(GameRenderDesc* desc)
	{
		return E_NOTIMPL;
	}

	STDMETHODIMP_(HRESULT __stdcall) GameEngine::SetScene(IScene* scene)
	{
		if (scene == nullptr)
		{
			return E_POINTER;
		}

		IScene* null = nullptr;
		if (nextScene_.compare_exchange_strong(null, scene))
		{
			scene->AddRef();
			return S_OK;
		}
		return E_FAIL;
	}

	STDMETHODIMP_(HRESULT __stdcall) GameEngine::GetScene(IScene** scene)
	{
		if (scene == nullptr)
		{
			return E_POINTER;
		}
		*scene = currentScene_.Get();
		currentScene_->AddRef();
		return S_OK;
	}

	STDMETHODIMP_(HRESULT __stdcall) GameEngine::CreateLayer(ILayer** layer)
	{
		if (layer == nullptr)
		{
			return E_POINTER;
		}

		ILayer* newLayer = new(std::nothrow) KumaEngine::cpp::CommonLayer();
		if (newLayer == nullptr)
		{
			return E_FAIL;
		}

		*layer = newLayer;
		return S_OK;
	}

	STDMETHODIMP_(HRESULT __stdcall) GameEngine::Run(IScene* initScene)
	{
		HRESULT hr = 0;
		currentScene_ = initScene;
		if (renderModule_ == nullptr || currentScene_ == nullptr)
		{
			return E_FAIL;
		}

		while (true)
		{
			IScene* nextScene = nullptr;
			nextScene = nextScene_.exchange(nextScene);
			if (nextScene != nullptr)
			{
				currentScene_->OnUnloaded();
				*static_cast<IScene**>(&currentScene_) = nextScene;
				if (FAILED(hr = currentScene_->OnLoaded()))
				{
					break;
				}
			}
			if (FAILED(hr = currentScene_->Update()))
			{
				break;
			}
			if (FAILED(hr = renderModule_->Update()))
			{
				break;
			}
		}
		return S_OK;
	}


}
