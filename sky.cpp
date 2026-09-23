

#include "main.h"
#include "manager.h"
#include "sky.h"
#include "renderer.h"
//#include "Field.h"
#include "DirectXTex.h"
#include "modelRenderer.h"
#include "input.h"
#include "transform.h"
#include "camera.h"
#include "bullet.h"
#include <imgui.h>
#include "Camera.h"


void Sky::Init()
{
    m_Layer = 1;
    TransformComponent* transform = GetComponent<TransformComponent>();
    transform->SetPosition({ 0.0f, 0.0f, 0.0f });
    transform->SetScale({ 200.0f, 200.0f, 200.0f });

    ModelRenderer* modelRender = AddComponent<ModelRenderer>(this);

    modelRender->Load("asset\\Model\\sky.obj");

    // シェーダー読込
    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
        "unlitTextureVS.cso");

    Renderer::CreatePixelShader(&m_PixelShader,
        "unlitTexturePS.cso");


}

void Sky::Uninit()
{
	



    if (m_VertexShader) { m_VertexShader->Release(); m_VertexShader = nullptr; }
    if (m_PixelShader) { m_PixelShader->Release(); m_PixelShader = nullptr; }
    if (m_VertexLayout) { m_VertexLayout->Release(); m_VertexLayout = nullptr; }

    GameObject::Uninit();

}
void Sky::Update(double deltaTime)
{
    Camera* camera = Manager::GetGameObject<Camera>();
    TransformComponent* transform = GetComponent<TransformComponent>();
    transform->SetPosition(camera->GetPosition());


    GameObject::Update(deltaTime);


}

void Sky::Draw()
{
    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

    // TransformComponentからワールド行列を取得
    TransformComponent* transform = GetComponent<TransformComponent>();
    XMMATRIX world = transform->GetWorldMatrix();
    Renderer::SetWorldMatrix(world);

    GameObject::Draw();
}