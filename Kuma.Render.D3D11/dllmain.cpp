// dllmain.cpp : DLL 애플리케이션의 진입점을 정의합니다.
#include "pch.h"
#include "Renderer.h"
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
using namespace KumaEngine::cpp;
extern "C" HRESULT __declspec(dllexport) __cdecl CreateRenderModule(IRenderModule **ppModule)
{
    *ppModule = new(std::nothrow) KumaEngine::cpp::D3D11RenderModule{};
    return S_OK;
}