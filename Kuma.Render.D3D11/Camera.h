#pragma once
#include "KumaEngine2.h"
#include "helper.h"
#include <unordered_map>
#include <mutex>
#include <wrl.h>
namespace KumaEngine::cpp
{
	using namespace Microsoft::WRL;
	class Camera : public RefCountImpl<ICamera, ISurface, IEntity, IUnknown>
	{
	public:
		STDMETHOD(GetWeakRef(IWeakRef** ref));
		STDMETHOD(GetRenderModule(IRenderModule** ppModule));
		STDMETHOD(AddLayer(ILayer* layer)) ;
		STDMETHOD(RemoveLayer(ILayer* layer)) ;
		STDMETHOD(GetLayers(IKumaEngine_EntityIterator** iterator));
		STDMETHOD(SetNear(float value)) ;
		STDMETHOD_(float, GetNear()) ;
		STDMETHOD(SetFar(float value)) ;
		STDMETHOD_(float, GetFar()) ;
		auto GetLayers()->std::unordered_map<ILayer*, ComPtr<ILayer>>&;
	private:
		std::unordered_map<ILayer*, ComPtr<ILayer>> layers_;
		std::mutex lock_;
	};

}