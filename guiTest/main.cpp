#include<wx/wxprec.h>
#include<wx/wx.h>
#include"KumaEngine2.h"

class GameObject : public KumaEngine::cpp::IGameObject
{
	std::vector<KumaEngine::cpp::IComponent*> coms;
	ULONG refCnt;
	STDMETHOD(GetComponent(REFIID guid, void** component))
	{
		
		for (auto* it : coms)
		{
			if (SUCCEEDED(it->QueryInterface(guid, component)))
			{
				return S_OK;
			}
		}
		return E_FAIL;
	}
	STDMETHOD(AddComponent(KumaEngine::cpp::IComponent* component))
	{
		if (component == nullptr)
		{
			return E_POINTER;
		}
		coms.push_back(component);
		return S_OK;
	}
	
	STDMETHOD(GetWeakRef(IKumaEngine_WeakRef** ref))
	{

	}
	
	STDMETHOD(GetIterator(REFIID itemType, IKumaEngine_EntityIterator** iterator))
	{

	}

	STDMETHOD(GetParent(IKumaEngine_WeakRef** parent))
	{
		return E_NOTIMPL;
	}
	STDMETHOD(SetParent(IKumaEngine_WeakRef* parent))
	{
		return S_OK;
	}
	STDMETHOD(Update())
	{
		return S_OK;
	}
	STDMETHOD(LateUpdate())
	{
		return S_OK;
	}
	STDMETHOD_(bool, IsDestroyed())
	{
		return false;
	}
};

class App : public wxApp
{
public:
	bool OnInit() override
	{
		hRenderModule_ = LoadLibraryW(L"Kuma.Render.D3D11.dll");
		if (hRenderModule_ == nullptr)
		{
			wxLogDebug(L"failed to load Kuma.Render.D3D11.dll");
			return false;
		}

		auto fn = (lpfnCreateRenderModule)GetProcAddress(hRenderModule_, FN_CREATE_RENDER_MODULE_NAME);
		if (fn == nullptr)
		{
			wxLogDebug(L"failed to get " TEXT(FN_CREATE_RENDER_MODULE_NAME)"");
			FreeLibrary(hRenderModule_);
			return false;
		}
		fn(&renderModule_);
		wxFrame* frame = new wxFrame{ nullptr, wxID_ANY, wxT("Window") };
		GameRenderDesc desc{};
		desc.fullscreen = false;
		desc.width = 800;
		desc.height = 600;
		frame->Show();
		renderModule_->Initialize(frame->GetHWND(), desc);
		renderModule_->CreateCamera(&cam_);
		renderModule_->SetMainCamera(cam_);
		KumaEngine::cpp::IShader* shader;
		renderModule_->CreateShaderFromFile(L"./2D_GAME_PS.hlsl", "ps_main", &shader);
		
		if (shader != nullptr)
		{
			shader->Release();
		}
		timer_ = new wxTimer{ this };
		this->Bind(wxEVT_TIMER, [this](wxTimerEvent& evt)
			{
				renderModule_->Update();
			});
		timer_->Start(10);
		return true;
	}
	int OnExit() override
	{
		timer_->Stop();
		cam_->Release();
		renderModule_->Release();

		FreeLibrary(hRenderModule_);
		delete timer_;
		return wxApp::OnExit();
	}
private:
	wxTimer* timer_;
	KumaEngine::cpp::IRenderModule* renderModule_;
	KumaEngine::cpp::ICamera* cam_;
	HMODULE hRenderModule_;
};

IMPLEMENT_APP(App);