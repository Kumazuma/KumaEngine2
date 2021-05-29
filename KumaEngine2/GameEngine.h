#pragma once
#include"KumaEngine2.h"
#include"helper.h"
#include<wrl.h>
#include<atomic>
using namespace Microsoft::WRL;

class CommonWeakRef;

class GameEngine : public RefCountImpl<IGameEngine, IEntity, IUnknown>
{
public:
	GameEngine();
	~GameEngine();
	STDMETHOD(GetWeakRef(IWeakRef** ref));
	STDMETHOD(Initialize(GameRenderDesc* desc));
	STDMETHOD(SetScene(IScene* scene));
	STDMETHOD(GetScene(IScene** scene));
	STDMETHOD(CreateLayer(ILayer** layer));
	STDMETHOD(Run(IScene* initScene));

private:
	ComPtr<IScene> currentScene_;
	ComPtr<IRenderer> renderer_;
	std::atomic<IScene*> nextScene_;
	ComPtr<CommonWeakRef> weakRef_;
};