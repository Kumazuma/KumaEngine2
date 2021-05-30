#include "pch.h"
#include "RenderComponent.h"
namespace KumaEngine::cpp
{
    STDMETHODIMP_(HRESULT) MeshRendererImpl::UpdateAndSwap()
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP_(HRESULT) MeshRendererImpl::GetWeakRef(IKumaEngine_WeakRef** ref)
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP_(HRESULT) MeshRendererImpl::Render(ID3D11DeviceContext4* deviceContext)
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP MeshRendererImpl::GetRenderModule(IRenderModule** ppModule)
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP MeshRendererImpl::SetTexture(ISurface* texture)
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP MeshRendererImpl::SetMesh(IMesh* mesh)
    {
        return E_NOTIMPL;
    }
}
