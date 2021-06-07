#include "pch.h"
#include "RenderComponent.h"
namespace KumaEngine::cpp
{
	MeshRendererImpl::MeshRendererImpl():
		index_{ 0 },
		preparedMaterial_{},
		material_{},
		mesh_{},
		preparedMesh_{},
		worldMatrices{}
	{
	}
	MeshRendererImpl::~MeshRendererImpl()
	{
		if (auto* mat = material_.exchange(nullptr); mat != nullptr)
		{
			mat->Release();
		}
		if (auto* mesh = mesh_.exchange(nullptr); mesh != nullptr)
		{
			mesh->Release();
		}
		if (preparedMaterial_ != nullptr)
		{
			preparedMaterial_->Release();
		}
		if (preparedMesh_ != nullptr)
		{
			preparedMesh_->Release();
		}
	}
	STDMETHODIMP_(HRESULT __stdcall) MeshRendererImpl::GetWeakRef(IWeakRef** ref)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP_(HRESULT __stdcall) MeshRendererImpl::SetGameObject(IWeakRef* gameObject)
	{
		if (gameObject == nullptr)
		{
			return E_POINTER;
		}
		ComPtr<IGameObject> gameObject_;
		if (FAILED(gameObject->LockEntity(__uuidof(IGameObject), &gameObject_)))
		{
			return E_INVALIDARG;
		}
		gameObj_ = gameObject;
		return S_OK;
	}
	STDMETHODIMP_(HRESULT __stdcall) MeshRendererImpl::Update()
	{
		return S_OK;
	}
	STDMETHODIMP_(HRESULT __stdcall) MeshRendererImpl::LateUpdate()
	{
		return S_OK;
	}
	STDMETHODIMP_(HRESULT __stdcall) MeshRendererImpl::SetMesh(IMesh* mesh)
	{
		if (mesh == nullptr)
		{
			return E_POINTER;
		}
		ID3D11Mesh* d3d11Mesh{};
		if (FAILED(mesh->QueryInterface(&d3d11Mesh)))
		{
			return E_INVALIDARG;
		}
		ID3D11Mesh* prev = mesh_.exchange(d3d11Mesh);
		if (prev != nullptr)
		{
			prev->AddRef();
		}
		return S_OK;
	}

	STDMETHODIMP_(HRESULT __stdcall) MeshRendererImpl::GetMesh(IMesh** mesh)
	{
		if (mesh == nullptr)
		{
			return E_POINTER;
		}
		ID3D11Mesh* currentMesh{};
		currentMesh = mesh_.load(std::memory_order::memory_order_acquire);
		currentMesh->AddRef();
		*mesh = currentMesh;
		return S_OK;
	}
	STDMETHODIMP_(HRESULT __stdcall) MeshRendererImpl::GetMaterial(IMaterial** material)
	{
		if (material == nullptr)
		{
			return E_POINTER;
		}
		ID3D11Material* currentMat{};
		currentMat = material_.load(std::memory_order::memory_order_acquire);
		currentMat->AddRef();
		*material = currentMat;
		return S_OK;
	}
	STDMETHODIMP_(HRESULT __stdcall) MeshRendererImpl::SetMaterial(IMaterial* material)
	{
		if (material == nullptr)
		{
			return E_POINTER;
		}
		ID3D11Material* mat{};
		if (FAILED(material->QueryInterface(&mat)))
		{
			return E_INVALIDARG;
		}
		ID3D11Material* prev = material_.exchange(mat);
		if (prev != nullptr)
		{
			prev->AddRef();
		}
		return S_OK;
	}
	STDMETHODIMP_(HRESULT __stdcall) MeshRendererImpl::PrepareRender(ID3D11Device5* device, ID3D11DeviceContext4* context)
	{
		if (gameObj_ == nullptr)
		{
			return E_FAIL;
		}

		ComPtr<IGameObject> gameObject_;
		XMMATRIX mWorldTransform{ XMMatrixIdentity() };
		ComPtr<IWeakRef> weakRef_{ gameObj_ };
		do
		{
			if (FAILED(weakRef_->LockEntity(__uuidof(IGameObject), &gameObject_)))
			{
				break;
			}
			ComPtr<ITransform> transform;
			if (FAILED(gameObject_->GetComponent(__uuidof(ITransform), &transform)))
			{
				continue;
			}
			XMVECTOR vPosition{};
			XMVECTOR vScale{};
			XMVECTOR vRotation{};
			transform->GetPositionToVector(reinterpret_cast<float*>(&vPosition), sizeof(XMVECTOR));
			transform->GetRotationToVector(reinterpret_cast<float*>(&vScale), sizeof(XMVECTOR));
			transform->GetScaleToVector(reinterpret_cast<float*>(&vRotation), sizeof(XMVECTOR));
			mWorldTransform = mWorldTransform *
				XMMatrixScalingFromVector(vScale) *
				XMMatrixRotationRollPitchYawFromVector(vRotation) *
				XMMatrixTranslationFromVector(vPosition);

		} while (SUCCEEDED(gameObject_->GetParent(&weakRef_)));
		int volatile nextIndex{ index_ == 1 ? 0 : 1 };
		XMStoreFloat4x4(&worldMatrices[nextIndex], mWorldTransform);
		ID3D11Material* material = material_.load(std::memory_order::memory_order_relaxed);
		if (material != nullptr)
		{
			material->PrepareRender(device, context);
		}

		if (material != preparedMaterial_)
		{
			ID3D11Material* volatile oldMaterial = preparedMaterial_;
			preparedMaterial_ = material;
			oldMaterial->Release();
			material->AddRef();
		}
		index_ = nextIndex;

		return S_OK;
	}
	STDMETHODIMP_(HRESULT __stdcall) MeshRendererImpl::GetWorldTransform(DirectX::XMFLOAT4X4* out)
	{
		if (out == nullptr)
		{
			return E_POINTER;
		}
		int volatile currentIndex{ index_};
		*out = worldMatrices[currentIndex];
		return S_OK;
	}
	STDMETHODIMP_(HRESULT __stdcall) MeshRendererImpl::GetPreparedMesh(ID3D11Mesh** mesh)
	{
		if (mesh == nullptr)
		{
			return E_POINTER;
		}
		if (preparedMesh_ != nullptr)
		{
			preparedMesh_->AddRef();
		}
		*mesh = preparedMesh_;
		return S_OK;
	}
	STDMETHODIMP_(HRESULT __stdcall) MeshRendererImpl::GetPreparedMaterial(ID3D11Material** material)
	{
		if (material == nullptr)
		{
			return E_POINTER;
		}
		if (preparedMaterial_ != nullptr)
		{
			preparedMaterial_->AddRef();
		}
		*material = preparedMaterial_;
		return S_OK;
	}
}
