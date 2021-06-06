#pragma once
#include<cstdint>
#include<combaseapi.h>

interface IKumaEngine_WeakRef;
interface IKumaEngine_Entity;
interface IKumaEngine_GameObject;
interface IKumaEngine_Component;
interface IKumaEngine_Scene;
interface IKumaEngine_Layer;
interface IKumaEngine_GameEngine;
interface IKumaEngine_Transform;
interface IKumaEngine_MeshRenderer;
interface IKumaEngine_Camera;
interface IKumaEngine_Surface;
interface IKumaEngine_Mesh;
interface IKumaEngine_Material;
interface IKumaEngine_Shader;

typedef struct _GameRenderDesc
{
	uint16_t width;
	uint16_t height;
	bool fullscreen;
} GameRenderDesc;
#if defined(__cplusplus)
#include<type_traits>
// {93528159-3A3B-4590-AE8B-429217AC7F58}
DEFINE_GUID(IID_ENTITY, 0x93528159, 0x3a3b, 0x4590, 0xae, 0x8b, 0x42, 0x92, 0x17, 0xac, 0x7f, 0x58);
DECLARE_INTERFACE_IID_(IKumaEngine_Entity, IUnknown, "93528159-3A3B-4590-AE8B-429217AC7F58")
{
	STDMETHOD(GetWeakRef(IKumaEngine_WeakRef** ref)) PURE;
};

// {73BFC47B-CE21-4D52-9996-4CD60848568D}
DEFINE_GUID(IID_WEAKREF, 0x73bfc47b, 0xce21, 0x4d52, 0x99, 0x96, 0x4c, 0xd6, 0x8, 0x48, 0x56, 0x8d);

DECLARE_INTERFACE_IID_(IKumaEngine_WeakRef, IUnknown, "73BFC47B-CE21-4D52-9996-4CD60848568D")
{
	STDMETHOD(LockEntity(REFIID iid, void** entity)) PURE;
};

// {06DA932D-20FC-4490-96ED-C65A59BE5DF6}
DEFINE_GUID(IID_ENTITY_ITERATOR,
	0x6da932d, 0x20fc, 0x4490, 0x96, 0xed, 0xc6, 0x5a, 0x59, 0xbe, 0x5d, 0xf6);

DECLARE_INTERFACE_IID_(IKumaEngine_EntityIterator, IUnknown, "06DA932D-20FC-4490-96ED-C65A59BE5DF6")
{
	STDMETHOD(GetNext(void** entity)) PURE;
};

// {475E9B20-FBFD-4B31-B21C-04F0BA6B8863}
DEFINE_GUID(IID_HAS_ITERATOR ,
	0x475e9b20, 0xfbfd, 0x4b31, 0xb2, 0x1c, 0x4, 0xf0, 0xba, 0x6b, 0x88, 0x63);

DECLARE_INTERFACE_IID_(IKumaEngine_HasIterator, IKumaEngine_Entity, "475E9B20-FBFD-4B31-B21C-04F0BA6B8863")
{
	STDMETHOD(GetIterator(REFIID itemType, IKumaEngine_EntityIterator * *iterator)) PURE;
};


// {1DF1AB26-5BEB-47C0-99B2-5523882982FC}
DEFINE_GUID(IID_SCENE ,
	0x1df1ab26, 0x5beb, 0x47c0, 0x99, 0xb2, 0x55, 0x23, 0x88, 0x29, 0x82, 0xfc);

DECLARE_INTERFACE_IID_(IKumaEngine_Scene, IKumaEngine_Entity, "1DF1AB26-5BEB-47C0-99B2-5523882982FC")
{
	STDMETHOD(OnLoaded()) PURE;
	STDMETHOD(OnUnloaded()) PURE;
	STDMETHOD(NewLayer(LPCWSTR newLayerName)) PURE;
	STDMETHOD(GetLayer(LPCWSTR layerName, IKumaEngine_Layer** layer)) PURE;
	STDMETHOD(Update()) PURE;
};

// {ABB60990-A5A7-4851-9B44-BBB7C3051078}
DEFINE_GUID(IID_LAYER ,
	0xabb60990, 0xa5a7, 0x4851, 0x9b, 0x44, 0xbb, 0xb7, 0xc3, 0x5, 0x10, 0x78);

MIDL_INTERFACE("ABB60990-A5A7-4851-9B44-BBB7C3051078") IKumaEngine_Layer:public IKumaEngine_HasIterator
{
	STDMETHOD(AddObject(IKumaEngine_GameObject* obj)) PURE;
	STDMETHOD(RemoveObject(IKumaEngine_GameObject * obj)) PURE;
	STDMETHOD(Update()) PURE;
	STDMETHOD(LateUpdate()) PURE;
};

// {9EB2C8C8-B69A-4742-BF47-A93FE1F28B82}
DEFINE_GUID(IID_GAMEENGINE ,
	0x9eb2c8c8, 0xb69a, 0x4742, 0xbf, 0x47, 0xa9, 0x3f, 0xe1, 0xf2, 0x8b, 0x82);

DECLARE_INTERFACE_IID_(IKumaEngine_GameEngine, IKumaEngine_Entity, "9EB2C8C8-B69A-4742-BF47-A93FE1F28B82")
{
	STDMETHOD(Initialize(GameRenderDesc* desc)) PURE;
	STDMETHOD(SetScene(IKumaEngine_Scene* scene)) PURE;
	STDMETHOD(GetScene(IKumaEngine_Scene** scene)) PURE;
	STDMETHOD(CreateLayer(IKumaEngine_Layer** layer)) PURE;
	STDMETHOD(Run(IKumaEngine_Scene * initScene)) PURE;
};

// {3D0E0216-86F5-4BE4-BBE5-E53C928C79E4}
DEFINE_GUID(IID_GAMEOBJECT ,
	0x3d0e0216, 0x86f5, 0x4be4, 0xbb, 0xe5, 0xe5, 0x3c, 0x92, 0x8c, 0x79, 0xe4);

MIDL_INTERFACE("3D0E0216-86F5-4BE4-BBE5-E53C928C79E4") IKumaEngine_GameObject: public IKumaEngine_HasIterator
{
	STDMETHOD(GetComponent(REFIID guid, void** component)) PURE;
	STDMETHOD(GetParent(IKumaEngine_WeakRef** parent)) PURE;
	STDMETHOD(SetParent(IKumaEngine_WeakRef* parent)) PURE;
	STDMETHOD(Update()) PURE;
	STDMETHOD(LateUpdate()) PURE;
	STDMETHOD_(bool, IsDestroyed()) PURE;
};

// {C4832C0B-3163-41EE-833D-2CEA9EA9B588}
DEFINE_GUID(IID_COMPONENT ,
	0xc4832c0b, 0x3163, 0x41ee, 0x83, 0x3d, 0x2c, 0xea, 0x9e, 0xa9, 0xb5, 0x88);

DECLARE_INTERFACE_IID_(IKumaEngine_Component, IKumaEngine_Entity, "C4832C0B-3163-41EE-833D-2CEA9EA9B588")
{
	STDMETHOD(SetGameObject(IKumaEngine_WeakRef* gameObject)) PURE;
	STDMETHOD(Update()) PURE;
	STDMETHOD(LateUpdate()) PURE;
};

// {C1CADF76-5B42-4037-92E6-E5786553C539}
DEFINE_GUID(IID_COM_TRANSFORM ,
	0xc1cadf76, 0x5b42, 0x4037, 0x92, 0xe6, 0xe5, 0x78, 0x65, 0x53, 0xc5, 0x39);

DECLARE_INTERFACE_IID_(IKumaEngine_Transform, IKumaEngine_Entity, "C1CADF76-5B42-4037-92E6-E5786553C539")
{
	STDMETHOD_(void, SetPosition(float x, float y, float z)) PURE;
	STDMETHOD_(void, GetPosition(float* x, float* y, float* z)) PURE;
	STDMETHOD_(void, SetPositionFromVector(float* vec, size_t vectorSize)) PURE;
	STDMETHOD_(void, GetPositionToVector(float* vec, size_t vectorSize)) PURE;

	STDMETHOD_(void, SetScale(float x, float y, float z)) PURE;
	STDMETHOD_(void, GetScale(float* x, float* y, float* z)) PURE;
	STDMETHOD_(void, SetScaleFromVector(float* vec, size_t vectorSize)) PURE;
	STDMETHOD_(void, GetScaleToVector(float* vec, size_t vectorSize)) PURE;

	STDMETHOD_(void, SetRotation(float x, float y, float z)) PURE;
	STDMETHOD_(void, GetRotation(float* x, float* y, float* z)) PURE;
	STDMETHOD_(void, SetRotationFromVector(float* vec, size_t vectorSize)) PURE;
	STDMETHOD_(void, GetRotationToVector(float* vec, size_t vectorSize)) PURE;
};

// {874F3420-2758-4D38-BB28-1E76F29C3765}
DEFINE_GUID(IID_RENDER_MODULE ,
	0x874f3420, 0x2758, 0x4d38, 0xbb, 0x28, 0x1e, 0x76, 0xf2, 0x9c, 0x37, 0x65);

DECLARE_INTERFACE_IID_(IKumaEngine_RenderModule, IKumaEngine_Entity, "874F3420-2758-4D38-BB28-1E76F29C3765")
{
	STDMETHOD(Initialize(HWND hWnd, const GameRenderDesc * desc)) PURE;
	/// <summary>
	/// 렌더 모듈을 갱신한다. 이 함수는 스레드 세이프하지 않다.
	/// 이 함수가 동작 중에 이 객체와 관련된 객체들이 수정되면 안된다.
	/// </summary>
	/// <returns>성공하면 S_OK, 실패하면 다른 값을 반환한다.</returns>
	STDMETHOD(Update()) PURE;
	STDMETHOD(CreateMeshRenderer(IKumaEngine_MeshRenderer** meshRenderer)) PURE;
	STDMETHOD(CreateCamera(IKumaEngine_Camera** camera)) PURE;
	STDMETHOD(CreateShaderFromFile(const wchar_t* fileName, const char* entryPoint, IKumaEngine_Shader** shader)) PURE;
	STDMETHOD(LoadMeshFromFile(const wchar_t* meshId, const wchar_t* filePath)) PURE;
	STDMETHOD(LoadMeshFromMemory(const wchar_t* meshId, const wchar_t* ext, const uint8_t* bytes, size_t byteLength)) PURE;
	STDMETHOD(LoadTextureFromFile(const wchar_t* textureId, const wchar_t* filePath)) PURE;
	STDMETHOD(LoadTextureFromMemory(const wchar_t* textureId, const wchar_t* ext, const uint8_t * bytes, size_t byteLength)) PURE;
	STDMETHOD(GetMesh(IKumaEngine_Mesh** mesh)) PURE;
	STDMETHOD(GetTexture(IKumaEngine_Surface** texture)) PURE;
	/// <summary>
	/// 윈도우에 출력할 카메라를 지정한다. 이 함수는 스레드 세이프하다.
	/// </summary>
	/// <param name="camera">윈도우에 실제로 뿌릴 카메라</param>
	/// <returns>성공하면 S_OK, 실패하면 다른 값을 반환한다.</returns>
	STDMETHOD(SetMainCamera(IKumaEngine_Camera * camera)) PURE;

	HRESULT Initialize(HWND hWnd, const GameRenderDesc& desc)
	{
		return Initialize(hWnd, &desc);
	}
};

// {DEFAA1C4-4CA3-40F3-B354-C2BD5486CFC6}
DEFINE_GUID(IID_MESH_RENDERER,
	0xdefaa1c4, 0x4ca3, 0x40f3, 0xb3, 0x54, 0xc2, 0xbd, 0x54, 0x86, 0xcf, 0xc6);
DECLARE_INTERFACE_IID_(IKumaEngine_MeshRenderer, IKumaEngine_Component, "DEFAA1C4-4CA3-40F3-B354-C2BD5486CFC6")
{
	STDMETHOD(SetMesh(IKumaEngine_Mesh* mesh)) PURE;
	STDMETHOD(GetMesh(IKumaEngine_Mesh ** mesh)) PURE;
	STDMETHOD(GetMaterial(IKumaEngine_Material ** material)) PURE;
	STDMETHOD(SetMaterial(IKumaEngine_Material * material)) PURE;
};

// {602389EE-64B8-4C4E-900D-A2FCE91B4A2B}
DEFINE_GUID(IID_SURFACE,
	0x602389ee, 0x64b8, 0x4c4e, 0x90, 0xd, 0xa2, 0xfc, 0xe9, 0x1b, 0x4a, 0x2b);
DECLARE_INTERFACE_IID_(IKumaEngine_Surface, IKumaEngine_Entity, "22F61AF9-ABFD-4096-8D5E-B7BAB49F017E")
{
	STDMETHOD(GetRenderModule(IKumaEngine_RenderModule** ppModule)) PURE;
};

// {6034E20E-FAB7-46F1-B94D-30F51D586B73}
DEFINE_GUID(IID_RENDERTARGET_SURFACE ,
	0x6034e20e, 0xfab7, 0x46f1, 0xb9, 0x4d, 0x30, 0xf5, 0x1d, 0x58, 0x6b, 0x73);

DECLARE_INTERFACE_IID_(IKumaEngine_RenderTargetSurface, IKumaEngine_Surface, "6034E20E-FAB7-46F1-B94D-30F51D586B73")
{
	STDMETHOD(GetCamera(IKumaEngine_Camera ** ppModule)) PURE;
	STDMETHOD(SetCamera(IKumaEngine_Camera * pModule)) PURE;
	STDMETHOD(SetSize(uint16_t width, uint16_t height)) PURE;
	STDMETHOD(GetSize(uint16_t* width, uint16_t* height)) PURE;
};

// {22F61AF9-ABFD-4096-8D5E-B7BAB49F017E}
DEFINE_GUID(IID_CAMERA ,
	0x22f61af9, 0xabfd, 0x4096, 0x8d, 0x5e, 0xb7, 0xba, 0xb4, 0x9f, 0x1, 0x7e);
DECLARE_INTERFACE_IID_(IKumaEngine_Camera, IKumaEngine_Entity, "22F61AF9-ABFD-4096-8D5E-B7BAB49F017E")
{
	STDMETHOD(GetRenderModule(IKumaEngine_RenderModule ** ppModule)) PURE;
	STDMETHOD(Add(IKumaEngine_HasIterator * layer)) PURE;
	STDMETHOD(Remove(IKumaEngine_HasIterator * layer)) PURE;
	STDMETHOD(GetIterator(REFIID itemType, IKumaEngine_EntityIterator * *iterator)) PURE;
	STDMETHOD(SetNear(float value)) PURE;
	STDMETHOD_(float, GetNear()) PURE;
	STDMETHOD(SetFar(float value)) PURE;
	STDMETHOD_(float, GetFar()) PURE;
};

// {24CDED12-2693-47C8-868E-64E83AED939E}
DEFINE_GUID(IID_MESH ,
	0x24cded12, 0x2693, 0x47c8, 0x86, 0x8e, 0x64, 0xe8, 0x3a, 0xed, 0x93, 0x9e);

MIDL_INTERFACE("24CDED12-2693-47C8-868E-64E83AED939E") IKumaEngine_Mesh: public IKumaEngine_Entity
{

};

// {6D631EA4-D201-4E97-967D-96E640E34D33}
DEFINE_GUID(IID_MATERIAL,
	0x6d631ea4, 0xd201, 0x4e97, 0x96, 0x7d, 0x96, 0xe6, 0x40, 0xe3, 0x4d, 0x33);

MIDL_INTERFACE("24CDED12-2693-47C8-868E-64E83AED939E") IKumaEngine_Material: public IKumaEngine_Entity
{
	STDMETHOD(SetShader(IKumaEngine_Shader* shader))PURE;
	STDMETHOD(GetShader(IKumaEngine_Shader **shader))PURE;
	STDMETHOD(SetPropertyInt(const char* key, const int32_t* value, uint32_t size)) PURE;
	STDMETHOD_(uint32_t, GetPropertyInt(const char* key, int32_t* value, uint32_t size)) PURE;
	STDMETHOD(SetPropertyFloat(const char* key, const float* value, uint32_t size)) PURE;
	STDMETHOD_(uint32_t, GetPropertyFloat(const char* key, float* value, uint32_t size)) PURE;
	STDMETHOD(SetPropertyTexture(const char* key, IKumaEngine_Surface** value, uint32_t size)) PURE;
	STDMETHOD_(uint32_t, GetPropertyTexture(const char* key, IKumaEngine_Surface** value, uint32_t size)) PURE;
	STDMETHOD(GetIterator(REFIID itemType, IKumaEngine_EntityIterator** iterator)) PURE;
};

// {96A50ABC-71A9-4523-8B1B-FDCBC85D3E3D}
DEFINE_GUID(IID_SHADER ,
	0x96a50abc, 0x71a9, 0x4523, 0x8b, 0x1b, 0xfd, 0xcb, 0xc8, 0x5d, 0x3e, 0x3d);

enum class ShaderPropertyKind
{
	None,
	Texture,
	Float,
	Int
};

struct KumaEngine_ShaderPropertyDesc
{
	char name[128];
	ShaderPropertyKind kind;
	uint32_t dimension[2];
};

MIDL_INTERFACE("96A50ABC-71A9-4523-8B1B-FDCBC85D3E3D") IKumaEngine_Shader: public IKumaEngine_Entity
{
	STDMETHOD_(uint32_t, GetPropertyCount()) PURE;
	STDMETHOD(GetPropertyDesc(uint32_t index, KumaEngine_ShaderPropertyDesc* desc)) PURE;
};

namespace KumaEngine
{
	namespace cpp
	{
		typedef ::IKumaEngine_EntityIterator IEntityIterator;
		typedef ::IKumaEngine_WeakRef IWeakRef;
		typedef ::IKumaEngine_Entity IEntity;
		typedef ::IKumaEngine_GameObject IGameObject;
		typedef ::IKumaEngine_Component IComponent;
		typedef ::IKumaEngine_Scene IScene;
		typedef ::IKumaEngine_Layer ILayer;
		typedef ::IKumaEngine_GameEngine IGameEngine;
		typedef ::IKumaEngine_Transform ITransform;
		typedef ::IKumaEngine_RenderModule IRenderModule;
		typedef ::IKumaEngine_MeshRenderer IMeshRenderer;
		typedef ::IKumaEngine_Camera ICamera;
		typedef ::IKumaEngine_Surface ISurface;
		typedef ::IKumaEngine_RenderTargetSurface IRenderTargetSurface;
		typedef ::IKumaEngine_Mesh IMesh;
		typedef ::IKumaEngine_Material IMaterial;
		typedef ::IKumaEngine_Shader IShader;
		typedef ::IKumaEngine_HasIterator IHasIterator;
	}
};
#else
typedef struct IKumaEngine_EntityVtbl
{
	BEGIN_INTERFACE
		HRESULT(STDMETHODCALLTYPE* QueryInterface)(IKumaEngine_Entity* This, REFIID riid, void** ppvObject);
		ULONG(STDMETHODCALLTYPE* AddRef)(IKumaEngine_Entity* This);
		ULONG(STDMETHODCALLTYPE* Release)(IKumaEngine_Entity* This);
		HRESULT(STDMETHODCALLTYPE* GetWeakRef)(IKumaEngine_Entity* This, IKumaEngine_WeakRef** ref);
	END_INTERFACE
} IKumaEngine_EntityVtbl;
interface IKumaEngine_Entity
{
	CONST_VTBL struct IKumaEngine_EntityVtbl* lpVtbl;
};

#endif

typedef HRESULT(__cdecl *lpfnCreateRenderModule)(IKumaEngine_RenderModule**);
#define FN_CREATE_RENDER_MODULE_NAME "CreateRenderModule"