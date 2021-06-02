#pragma once
#include"KumaEngine2.h"
#include"helper.h"
#include<d3d11_4.h>
#include<wrl.h>
#include<vector>
#include"Renderer.h"
using namespace Microsoft::WRL;
namespace KumaEngine::cpp
{
	class D3D11MeshImpl: public IMesh
	{
	public:
		D3D11MeshImpl();
		~D3D11MeshImpl();
		STDMETHODIMP GetWeakRef(IWeakRef** ref) override;
	private:
		ComPtr<ID3D11Buffer> vertexBuffer_;
		ComPtr<ID3D11Buffer> indexBuffer_;
		ComPtr<CommonWeakRef> weakRef_;
		std::vector<ComPtr<IWeakRef>> meshRenderers_;
	};
	using D3D11Mesh = RefCountImpl2<D3D11MeshImpl, IMesh, IEntity, IUnknown>;
}