#include "main.h"
#include "renderer.h"
#include "shadow.h"
#include "DirectXTex.h"
#include "transform.h"
#include "planeComponent.h"
#include "audio.h"
#include <imgui.h>
void Shadow::Init()
{

    m_Layer = 2;

    TransformComponent* transform = GetComponent<TransformComponent>();
   // transform->SetPosition({ 0.0f, 0.0f, 0.0f });
    //transform->SetScale({ 0.1f, 0.1f, 0.1f });
    transform->SetRotation({ -XM_PIDIV2, 0.0f, 0.0f });
    PlaneComponent* sprite = AddComponent<PlaneComponent>(this);

    sprite->SetSize(256.0f, 256.0f);
    sprite->LoadTexture(L"asset\\texture\\shadow.png");
   

}

void Shadow::Uninit()
{



    GameObject::Uninit();
}

void Shadow::Update(double deltaTime) {
	TransformComponent* transform = GetComponent<TransformComponent>();
	Vector3 scale = transform->GetScale();

    ImGui::Begin("Shadow"); 
    {
                ImGui::SliderFloat("SizeX", &scale.x, 0.1f, 10.0f);
                ImGui::SliderFloat("SizeY", &scale.y, 0.1f, 10.0f);
                ImGui::SliderFloat("SizeZ", &scale.z, 0.1f, 10.0f);
    }
	ImGui::End();

    transform->SetScale(scale);
}

void Shadow::Draw()
{
    PlaneComponent* sprite = GetComponent<PlaneComponent>();
    if (!sprite) return;

    // 深度テストを無効にして描画
    Renderer::SetDepthEnable(false);   
    sprite->GetMaterial().Diffuse = { 1,1,1,1 };
    sprite->Draw();

    // 深度テストを戻す
    Renderer::SetDepthEnable(true);
}