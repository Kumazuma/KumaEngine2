#include "pch.h"
#include "Mesh.h"
namespace KumaEngine::cpp
{
	D3D11MeshImpl::D3D11MeshImpl():
		vertexBuffer_{},
		indexBuffer_{}
	{
		*(CommonWeakRef**)&weakRef_ = new CommonWeakRef{ this };
	}
	D3D11MeshImpl::~D3D11MeshImpl()
	{
		weakRef_->Unlink();
	}
	STDMETHODIMP D3D11MeshImpl::GetWeakRef(IWeakRef** ref)
	{
		if (ref == nullptr)
		{
			return E_POINTER;
		}
		*ref = weakRef_.Get();
		weakRef_->AddRef();
		return S_OK;
	}

	//STDMETHODIMP_(HRESULT __stdcall) D3D11MeshImpl::Render(D3D11RenderModule* renderModule, ID3D11DeviceContext4* deviceContext)
	//{
	//	if (renderModule == nullptr || deviceContext == nullptr)
	//	{
	//		return E_INVALIDARG;
	//	}

	//	auto it = meshRenderers_.begin();
	//	auto const end = meshRenderers_.end();
	//	//std::vector<ComPtr<ID3D11MeshRenderer>> meshRenderers;
	//	while (it != end)
	//	{
	//		auto& comptr = *it;
	//		ComPtr<ID3D11MeshRenderer> meshRenderer;
	//		if (FAILED(comptr->LockEntity(__uuidof(ID3D11MeshRenderer), &meshRenderer)))
	//		{
	//			it = meshRenderers_.erase(it);
	//			continue;
	//		}
	//		++it;
	//		//TODO: Render
	//	}
	//	return S_OK;
	//}
}

