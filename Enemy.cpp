#include "enemy.h"
#include "renderer.h"
//#include "Field.h"
#include "DirectXTex.h"
#include "modelRenderer.h"
#include "input.h"
#include "terrainHeight.h"


void Enemy::Init()
{
    m_Layer = 1;
    SetPosition({ 0.0f, 0.0f, 0.0f });
    SetScale({ 1.5f, 1.5f, 1.5f });

    ModelRenderer* modelRender = AddComponent<ModelRenderer>(this);

    modelRender->Load("asset\\Model\\player.obj");

    // シェーダー読込
    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
        "unlitTextureVS.cso");

    Renderer::CreatePixelShader(&m_PixelShader,
        "unlitTexturePS.cso");


}

void Enemy::Uninit()
{



    if (m_VertexShader) { m_VertexShader->Release(); m_VertexShader = nullptr; }
    if (m_PixelShader) { m_PixelShader->Release(); m_PixelShader = nullptr; }
    if (m_VertexLayout) { m_VertexLayout->Release(); m_VertexLayout = nullptr; }

    GameObject::Uninit();

}
void Enemy::Update(double deltaTime)
{
    float dt = static_cast<float>(deltaTime);

    // 地形の凸凹に合わせてY座標を調整
    Vector3 pos = GetPosition();
    pos.y = GetTerrainHeight(pos.x, pos.z);
    SetPosition(pos);

    GameObject::Update(dt);
}
void Enemy::Draw()
{
    // インプットレイアウト設定
    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

    // シェーダー設定
    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

    // マトリクス設定 (TransformComponentからワールド行列を取得)
    TransformComponent* transform = GetComponent<TransformComponent>();
    DirectX::XMMATRIX world = transform->GetWorldMatrix();
    Renderer::SetWorldMatrix(world);

    GameObject::Draw();//継承元のDraw（）を呼び出すことで、
    //PlayerクラスのDraw()内でModelRendererクラスのDraw()も呼び出されるようになる。

}