#pragma once
#include"KumaEngine2.h"
#include"helper.h"
#include<vector>
#include<wrl.h>
using namespace Microsoft::WRL;
class CommonLayer : public RefCountImpl<ILayer, IEntity, IUnknown>
{
public:
	CommonLayer();
	~CommonLayer();
	STDMETHOD(GetWeakRef(IWeakRef** ref));
	STDMETHOD(GetIterator(IEntityIterator** iterator));
	STDMETHOD(AddObject(IGameObject* obj));
	STDMETHOD(RemoveObject(IGameObject* obj));
	STDMETHOD(Update());
	STDMETHOD(LateUpdate());
private:
	ComPtr<CommonWeakRef> weakRef_;
	std::vector<ComPtr<IGameObject>> objects;
};
