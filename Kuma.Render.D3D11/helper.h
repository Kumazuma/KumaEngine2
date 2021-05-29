#pragma once
#include "KumaEngine2.h"

#include<Unknwnbase.h>
#include<xutility>
template<typename ...Ts>
struct DoQueryInterfaceHelper;
template<typename COM_TYPE, typename TARGET_INTERFACE, typename ...Ts>
struct DoQueryInterfaceHelper<COM_TYPE, TARGET_INTERFACE, Ts...> : private DoQueryInterfaceHelper<COM_TYPE, Ts...>
{
	static HRESULT DoQueryInterface(COM_TYPE* obj, REFIID riid, void** ppvObject)
	{
		if (ppvObject == nullptr)
		{
			return E_POINTER;
		}

		if (__uuidof(TARGET_INTERFACE) == riid)
		{
			*ppvObject = reinterpret_cast<void*>(static_cast<TARGET_INTERFACE*>(obj));
			obj->AddRef();
			return S_OK;
		}
		return DoQueryInterfaceHelper<COM_TYPE, Ts...>::DoQueryInterface(obj, riid, ppvObject);
	}
};
template<typename COM_TYPE, typename TARGET_INTERFACE>
struct DoQueryInterfaceHelper<COM_TYPE, TARGET_INTERFACE>
{
	static HRESULT DoQueryInterface(COM_TYPE* obj, REFIID riid, void** ppvObject)
	{
		if (ppvObject == nullptr)
		{
			return E_POINTER;
		}

		if (__uuidof(TARGET_INTERFACE) == riid)
		{
			*ppvObject = reinterpret_cast<void*>(static_cast<TARGET_INTERFACE*>(obj));
			return S_OK;
		}
		return E_NOINTERFACE;
	}
};

template<typename T, typename ...BaseType>
class RefCountImpl : public T
{
public:
	template<typename ...Arg>
	RefCountImpl(Arg&& ...args) :
		T{ std::forward<Arg>(args)... }
	{
		refCount = 1;
	}

	STDMETHOD(QueryInterface(REFIID riid,/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject))
	{
		return DoQueryInterfaceHelper<T, T, BaseType...>::DoQueryInterface(this, riid, ppvObject);
	}

	STDMETHOD_(ULONG, AddRef())
	{
		return InterlockedIncrement(&refCount);

	}

	STDMETHOD_(ULONG, Release())
	{
		ULONG r = InterlockedDecrement(&refCount);
		if (r == 0)
		{
			delete this;
		}
		return r;
	}
	LONG refCount;
};
namespace KumaEngine::cpp
{
	class CommonWeakRef : public RefCountImpl <IWeakRef, IUnknown>
	{
	public:
		CommonWeakRef(IEntity* entity);
		STDMETHOD(LockEntity(IEntity** entity));
		void Unlink();
	private:
		std::atomic<IEntity*> obj_;
	};
}
