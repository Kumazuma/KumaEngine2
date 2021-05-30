#pragma once
#include "KumaEngine2.h"
#include "helper.h"
#include "Renderer.h"
#include <unordered_map>
#include <mutex>
#include <wrl.h>
namespace KumaEngine::cpp
{
	using namespace Microsoft::WRL;
	class CameraImpl : public ICamera, ID3D11RenderComponent
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
		STDMETHOD(UpdateAndSwap());
		STDMETHOD(Render(ID3D11DeviceContext4* deviceContext));
		auto GetLayers()->std::unordered_map<ILayer*, ComPtr<ILayer>>&;
	private:
		std::unordered_map<ILayer*, ComPtr<ILayer>> layers_;
		std::mutex lock_;
	};
	using Camera = RefCountImpl2<CameraImpl, ICamera, ID3D11RenderComponent, ISurface, IEntity, IUnknown>;
}