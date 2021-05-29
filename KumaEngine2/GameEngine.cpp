#include "pch.h"
#include "GameEngine.h"
#include "Layer.h"
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

	ILayer* newLayer = new(std::nothrow) CommonLayer();
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
	if (renderer_ == nullptr || currentScene_ == nullptr)
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
		if (FAILED(hr = renderer_->Update()))
		{
			break;
		}
	}
	return S_OK;
}

CommonWeakRef::CommonWeakRef(IEntity* entity):
	obj_{entity}
{
}

STDMETHODIMP_(HRESULT __stdcall) CommonWeakRef::LockEntity(IEntity** entity)
{
	if (entity == nullptr)
	{
		return E_POINTER;
	}
	IEntity* ref = obj_.load(std::memory_order::memory_order_acquire);
	if (ref == nullptr)
	{
		return E_FAIL;
	}

	if (static_cast<LONG>(ref->AddRef()) <= 1)
	{
		return E_FAIL;
	}
	*entity = ref;
	
	return S_OK;
}

void CommonWeakRef::Unlink()
{
	obj_.store(nullptr, std::memory_order::memory_order_release);
}
