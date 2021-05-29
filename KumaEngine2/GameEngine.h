#pragma once
#include"KumaEngine2.h"
#include"helper.h"
#include<wrl.h>
#include<atomic>
using namespace Microsoft::WRL;

class CommonWeakRef;
namespace KumaEngine::cpp
{
	class GameEngine : public RefCountImpl<IGameEngine, IKumaEngine_Entity, IUnknown>
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
		ComPtr<IRenderModule> renderModule_;
		std::atomic<IScene*> nextScene_;
		ComPtr<CommonWeakRef> weakRef_;
	};
}
