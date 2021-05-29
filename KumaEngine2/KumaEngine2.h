#pragma once
#include<cstdint>
#include<combaseapi.h>

interface IWeakRef;
interface IEntity;
interface IGameObject;
interface IComponent;
interface IScene;
interface ILayer;
interface IGameEngine;
typedef struct _GameRenderDesc GameRenderDesc;
// {93528159-3A3B-4590-AE8B-429217AC7F58}
DEFINE_GUID(IID_ENTITY, 0x93528159, 0x3a3b, 0x4590, 0xae, 0x8b, 0x42, 0x92, 0x17, 0xac, 0x7f, 0x58);
DECLARE_INTERFACE_IID_(IEntity, IUnknown, "93528159-3A3B-4590-AE8B-429217AC7F58")
{
	STDMETHOD(GetWeakRef(IWeakRef** ref)) PURE;
};

// {73BFC47B-CE21-4D52-9996-4CD60848568D}
DEFINE_GUID(IID_WEAKREF, 0x73bfc47b, 0xce21, 0x4d52, 0x99, 0x96, 0x4c, 0xd6, 0x8, 0x48, 0x56, 0x8d);

DECLARE_INTERFACE_IID_(IWeakRef, IUnknown, "73BFC47B-CE21-4D52-9996-4CD60848568D")
{
	STDMETHOD(LockEntity(IEntity** entity)) PURE;
};
// {06DA932D-20FC-4490-96ED-C65A59BE5DF6}
DEFINE_GUID(IID_ENTITY_ITERATOR,
	0x6da932d, 0x20fc, 0x4490, 0x96, 0xed, 0xc6, 0x5a, 0x59, 0xbe, 0x5d, 0xf6);

DECLARE_INTERFACE_IID_(IEntityIterator, IUnknown, "06DA932D-20FC-4490-96ED-C65A59BE5DF6")
{
	STDMETHOD(GetNext(IEntity** entity)) PURE;
};

// {1DF1AB26-5BEB-47C0-99B2-5523882982FC}
DEFINE_GUID(IID_SCENE ,
	0x1df1ab26, 0x5beb, 0x47c0, 0x99, 0xb2, 0x55, 0x23, 0x88, 0x29, 0x82, 0xfc);

DECLARE_INTERFACE_IID_(IScene, IEntity, "1DF1AB26-5BEB-47C0-99B2-5523882982FC")
{
	STDMETHOD(OnLoaded()) PURE;
	STDMETHOD(OnUnloaded()) PURE;
	STDMETHOD(NewLayer(LPCWSTR newLayerName)) PURE;
	STDMETHOD(GetLayer(LPCWSTR layerName, ILayer** layer)) PURE;
	STDMETHOD(Update()) PURE;
};

// {ABB60990-A5A7-4851-9B44-BBB7C3051078}
DEFINE_GUID(IID_LAYER ,
	0xabb60990, 0xa5a7, 0x4851, 0x9b, 0x44, 0xbb, 0xb7, 0xc3, 0x5, 0x10, 0x78);

DECLARE_INTERFACE_IID_(ILayer, IEntity, "ABB60990-A5A7-4851-9B44-BBB7C3051078")
{
	STDMETHOD(GetIterator(IEntityIterator** iterator)) PURE;
	STDMETHOD(AddObject(IGameObject* obj)) PURE;
	STDMETHOD(RemoveObject(IGameObject * obj)) PURE;
	STDMETHOD(Update()) PURE;
	STDMETHOD(LateUpdate()) PURE;
};

// {9EB2C8C8-B69A-4742-BF47-A93FE1F28B82}
DEFINE_GUID(IID_GAMEENGINE ,
	0x9eb2c8c8, 0xb69a, 0x4742, 0xbf, 0x47, 0xa9, 0x3f, 0xe1, 0xf2, 0x8b, 0x82);

DECLARE_INTERFACE_IID_(IGameEngine, IEntity, "9EB2C8C8-B69A-4742-BF47-A93FE1F28B82")
{
	STDMETHOD(Initialize(GameRenderDesc* desc)) PURE;
	STDMETHOD(SetScene(IScene* scene)) PURE;
	STDMETHOD(GetScene(IScene** scene)) PURE;
	STDMETHOD(CreateLayer(ILayer** layer)) PURE;
	STDMETHOD(Run(IScene * initScene)) PURE;
};

// {3D0E0216-86F5-4BE4-BBE5-E53C928C79E4}
DEFINE_GUID(IID_GAMEOBJECT ,
	0x3d0e0216, 0x86f5, 0x4be4, 0xbb, 0xe5, 0xe5, 0x3c, 0x92, 0x8c, 0x79, 0xe4);

DECLARE_INTERFACE_IID_(IGameObject, IEntity, "3D0E0216-86F5-4BE4-BBE5-E53C928C79E4")
{
	STDMETHOD(GetComponent(REFIID guid, IComponent** component)) PURE;
	STDMETHOD(GetParent(IWeakRef** parent)) PURE;
	STDMETHOD(SetParent(IWeakRef* parent)) PURE;
	STDMETHOD(GetChildren(IEntityIterator** iterator)) PURE;
	STDMETHOD(Update()) PURE;
	STDMETHOD(LateUpdate()) PURE;
	STDMETHOD_(bool, IsDestroyed()) PURE;
};

// {C4832C0B-3163-41EE-833D-2CEA9EA9B588}
DEFINE_GUID(IID_COMPONENT ,
	0xc4832c0b, 0x3163, 0x41ee, 0x83, 0x3d, 0x2c, 0xea, 0x9e, 0xa9, 0xb5, 0x88);

DECLARE_INTERFACE_IID_(IComponent, IEntity, "C4832C0B-3163-41EE-833D-2CEA9EA9B588")
{
	STDMETHOD(SetGameObject(IWeakRef* gameObject)) PURE;
	STDMETHOD(Update()) PURE;
	STDMETHOD(LateUpdate()) PURE;
};

// {874F3420-2758-4D38-BB28-1E76F29C3765}
DEFINE_GUID(IID_RENDERER ,
	0x874f3420, 0x2758, 0x4d38, 0xbb, 0x28, 0x1e, 0x76, 0xf2, 0x9c, 0x37, 0x65);

DECLARE_INTERFACE_IID_(IRenderer, IEntity, "874F3420-2758-4D38-BB28-1E76F29C3765")
{
	STDMETHOD(Update()) PURE;
};

struct _GameRenderDesc
{
	uint16_t width;
	uint16_t height;
	bool fullscreen;
};

