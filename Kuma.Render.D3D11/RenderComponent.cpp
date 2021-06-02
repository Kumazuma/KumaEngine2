#include "pch.h"
#include "RenderComponent.h"
namespace KumaEngine::cpp
{
    STDMETHODIMP_(HRESULT) MeshRendererImpl::GetWeakRef(IKumaEngine_WeakRef** ref)
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP MeshRendererImpl::GetRenderModule(IRenderModule** ppModule)
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP MeshRendererImpl::SetMesh(IMesh* mesh)
    {
        return E_NOTIMPL;
    }
}
