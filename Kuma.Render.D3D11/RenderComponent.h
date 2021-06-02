#pragma once

#include"KumaEngine2.h"
#include"helper.h"
#include"Renderer.h"
#include<d3d11_4.h>
namespace KumaEngine::cpp
{

	class MeshRendererImpl : public ID3D11MeshRenderer
	{
	public:
		STDMETHOD(GetWeakRef(IWeakRef** ref)) override;
		STDMETHOD(GetRenderModule(IRenderModule**ppModule)) override;

		STDMETHOD(SetMesh(IMesh* mesh)) override;
	};
	using MeshRenderer =
		RefCountImpl2<
			MeshRendererImpl,
			ID3D11MeshRenderer,
			IMeshRenderer,
			ID3D11Camera,
			IComponent,
			IEntity,
			IUnknown>;
}
