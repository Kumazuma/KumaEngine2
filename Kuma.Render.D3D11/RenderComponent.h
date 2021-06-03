#pragma once

#include"KumaEngine2.h"
#include"helper.h"
#include"Renderer.h"
#include<d3d11_4.h>
#include<DirectXMath.h>
#include<wrl.h>
#include<array>
#include<atomic>
namespace KumaEngine::cpp
{
	using namespace Microsoft::WRL;
	using namespace DirectX;
	class MeshRendererImpl : public ID3D11MeshRenderer
	{
	public:
		MeshRendererImpl();
		STDMETHOD(GetWeakRef(IWeakRef** ref));
		STDMETHOD(SetGameObject(IWeakRef* gameObject));
		STDMETHOD(Update());
		STDMETHOD(LateUpdate());
		STDMETHOD(SetMesh(IMesh* mesh));
		STDMETHOD(GetMesh(IMesh** mesh));
		STDMETHOD(GetMaterial(IMaterial** material));
		STDMETHOD(SetMaterial(IMaterial* material));

		STDMETHOD(PrepareRender());
		STDMETHOD(GetWorldTransform(DirectX::XMFLOAT4X4* out));
	private:
		ComPtr<IWeakRef> gameObj_;
		std::array<XMFLOAT4X4, 2> worldMatrices;
		int index_;
	};
	using MeshRenderer =
		RefCountImpl2<
			MeshRendererImpl,
			ID3D11MeshRenderer,
			IMeshRenderer,
			IComponent,
			IEntity,
			IUnknown>;
}
