#include<wx/wxprec.h>
#include<wx/wx.h>
#include"KumaEngine2.h"
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