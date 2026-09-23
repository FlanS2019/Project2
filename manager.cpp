#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "gameObject.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "game.h"
#include "title.h"
#include "audio.h"

std::list<GameObject*> Manager::m_GameObjects;
Scene* Manager::m_Scene     = nullptr;
Scene* Manager::m_NextScene = nullptr;
float Manager::m_ChangeTime = 0.0f;



void Manager::Init()
{
	Input::Init();
	Renderer::Init();
	Audio::InitMaster();
	m_Scene = new Title();
	m_Scene->Init();


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(GetWindow());
	ImGui_ImplDX11_Init(Renderer::GetDevice(), Renderer::GetDeviceContext());
	

}


void Manager::Uninit()
{
	
	if (m_Scene != nullptr) {
		m_Scene->Uninit();
		delete m_Scene;
	}
	for(GameObject* obj : m_GameObjects) {
		if (obj != nullptr) {
			obj->Uninit();
			delete obj;
			obj = nullptr;
		}
	}


	m_GameObjects.clear();
	Audio::UninitMaster();
	Renderer::Uninit();
	Input::Uninit();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Manager::Update(double deltaTime)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	
	bool show_demo_window = true;
	

#ifdef _DEBUG

	ImGui::Begin("Debug");

	ImGui::Text("FPS : %.1f", 1.0 / deltaTime);
	ImGui::End();

#endif

	Input::Update();
	if(m_Scene != nullptr)
	{
		m_Scene->Update();
	}
	for (GameObject* obj : m_GameObjects) {
		if (obj != nullptr) {
			obj->Update(deltaTime);
		}
    }
	m_GameObjects.remove_if([](GameObject* obj) {
		
		return obj->Destroy();

     });

	//ÉVÅ[ÉìêÿÇËë÷Ç¶
	if (m_NextScene != nullptr) {
		m_ChangeTime -= deltaTime;

		if(m_ChangeTime<0.0f)
		{
			if (m_Scene != nullptr)
			{
				m_Scene->Uninit();
				delete m_Scene;
			}
			for (GameObject* obj : m_GameObjects) {
				if (obj != nullptr) {
					obj->Uninit();
					delete obj;
					obj = nullptr;
				}
			}
			m_GameObjects.clear();
			m_Scene = m_NextScene;
			m_Scene->Init();
			m_NextScene = nullptr;
		}
	}

}

void Manager::Draw()
{
	
	Renderer::Begin();

	Camera* camera = GetGameObject<Camera>();
	if(camera)
	{
		Vector3 forward = camera->GetForward();
		Vector3 position = camera->GetPosition();


		for (GameObject* obj : m_GameObjects) {
			obj->CalcCameraZ(position, forward);
		}

		m_GameObjects.sort([](GameObject* a, GameObject* b) {
			return a->GetCameraZ() > b->GetCameraZ();
			});
	}


	for(int layer = 0; layer < 4; layer++) {
		for (GameObject* obj : m_GameObjects) {
			if (obj != nullptr && obj->GetLayer() == layer) {
				obj->Draw();
			}
		}
	}
	
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
	Renderer::End();
}
