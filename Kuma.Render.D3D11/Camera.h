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
	class CameraImpl : public ID3D11Camera
	{
	public:
		STDMETHOD(GetWeakRef(IWeakRef** ref)) override;
		STDMETHOD(GetRenderModule(IRenderModule** ppModule))  override;
		STDMETHOD(Add(IHasIterator* layer)) override;
		STDMETHOD(Remove(IHasIterator* layer))  override;
		STDMETHOD(GetIterator(REFIID itemType, IEntityIterator** iterator)) override;
		STDMETHOD(SetNear(float value))  override;
		STDMETHOD_(float, GetNear())  override;
		STDMETHOD(SetFar(float value))  override;
		STDMETHOD_(float, GetFar())  override;
		STDMETHOD(Render(D3D11RenderModule* renderModule, ID3D11DeviceContext4* deviceContext)) override;
		STDMETHOD(Update(D3D11RenderModule* renderModule)) override;
		STDMETHOD_(bool, IsDoneRender()) override;
	private:
		std::vector<ComPtr<ID3D11MeshRenderer>> renderers_;
		std::vector<ID3D11MeshRenderer*> defferedRenderers_;
		std::vector<ID3D11MeshRenderer*> forwardRenderers_;
		std::unordered_map<IHasIterator*, ComPtr<IWeakRef>> layers_;
		bool isDoneRender_;
		std::mutex lock_;
	};
	using Camera = RefCountImpl2<CameraImpl, ID3D11Camera, ICamera, IEntity, IUnknown>;
}