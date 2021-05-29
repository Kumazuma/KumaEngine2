#pragma once
#include "KumaEngine2.h"
#include "helper.h"
#include <d3d11.h>
#include <wrl.h>
namespace KumaEngine::cpp
{
	using namespace Microsoft::WRL;

	class D3D11Renderer: public RefCountImpl<IRenderer, IEntity, IUnknown>
	{
	public:
	private:
		bool isRunning_;
		
	};
}