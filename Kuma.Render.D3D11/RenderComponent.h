#pragma once

#include"KumaEngine2.h"
#include"helper.h"
namespace KumaEngine::cpp
{
	class MeshRenderer : public RefCountImpl < IMeshRenderer, IEntity, IUnknown>
	{
	public:
		STDMETHOD(GetRenderModule(IRenderModule**ppModule)) override;
		STDMETHOD(SetTexture(IKumaEngine_Surface* texture));
		STDMETHOD(SetMesh(IKumaEngine_Mesh* mesh));
	};
}
