#include "pch.h"
#include "Layer.h"

CommonLayer::CommonLayer()
{
    weakRef_ = new CommonWeakRef{ this };
    weakRef_->Release();
}

CommonLayer::~CommonLayer()
{
    weakRef_->Unlink();
}

STDMETHODIMP_(HRESULT __stdcall) CommonLayer::GetWeakRef(IWeakRef** ref)
{
    if (ref == nullptr)
    {
        return E_POINTER;
    }
    *ref = weakRef_.Get();
    weakRef_->AddRef();
    return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) CommonLayer::GetIterator(IEntityIterator** iterator)
{
    return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) CommonLayer::AddObject(IGameObject* obj)
{
    if (obj == nullptr)
    {
        return E_POINTER;
    }
    objects.push_back(obj);
    return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) CommonLayer::RemoveObject(IGameObject* obj)
{
    if (obj == nullptr)
    {
        return E_POINTER;
    }

    auto it = std::find(objects.begin(), objects.end(), obj);
    if (it == objects.end())
    {
        return E_INVALIDARG;
    }
    objects.erase(it);
    return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) CommonLayer::Update()
{
    HRESULT hr = S_OK;
    auto it = objects.begin();
    auto const end = objects.end();
    while (it != end)
    {
        auto& ptr = *it;
        if (ptr->IsDestroyed())
        {
            it = objects.erase(it);
        }
        else if (FAILED(hr = ptr->Update()))
        {
            it = objects.erase(it);
        }
        else
        {
            ++it;
        }
    }

    return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) CommonLayer::LateUpdate()
{
    HRESULT hr = S_OK;
    auto it = objects.begin();
    auto const end = objects.end();
    while (it != end)
    {
        auto& ptr = *it;
        if (ptr->IsDestroyed())
        {
            it = objects.erase(it);
        }
        else if (FAILED(hr = ptr->LateUpdate()))
        {
            it = objects.erase(it);
        }
        else
        {
            ++it;
        }
    }
    return S_OK;
}
