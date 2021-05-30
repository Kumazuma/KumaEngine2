#pragma once

#include"KumaEngine2.h"
#include"helper.h"
#include"Renderer.h"
#include<d3d11_4.h>
namespace KumaEngine::cpp
{

	class MeshRendererImpl : public IMeshRenderer, ID3D11RenderComponent
	{
	public:
		STDMETHOD(UpdateAndSwap()) override;
		STDMETHOD(GetWeakRef(IWeakRef** ref)) override;
		STDMETHOD(Render(ID3D11DeviceContext4* deviceContext)) override;
		STDMETHOD(GetRenderModule(IRenderModule**ppModule)) override;
		STDMETHOD(SetTexture(ISurface* texture)) override;
		STDMETHOD(SetMesh(IMesh* mesh)) override;
	};
	using MeshRenderer =
		RefCountImpl2<
			MeshRendererImpl,
			IMeshRenderer,
			ID3D11RenderComponent,
			IComponent,
			IEntity,
			IUnknown>;
}
