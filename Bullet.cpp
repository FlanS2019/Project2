#include "Bullet.h"
#include "modelRenderer.h"
#include "renderer.h"
#include "enemy.h"
#include "manager.h"
#include "explosion.h"
#include "score.h"


void Bullet::Init()
{
    m_Layer = 1;
    TransformComponent* transform = GetComponent<TransformComponent>();
    transform->SetPosition({ 0.0f, 0.0f, 0.0f });
    transform->SetScale({ 1.0f, 1.0f, 1.0f });

    ModelRenderer* modelRender = AddComponent<ModelRenderer>(this);

    modelRender->Load("asset\\Model\\bullet.obj");

    // シェーダー読込
    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
        "unlitTextureVS.cso");

    Renderer::CreatePixelShader(&m_PixelShader,
        "unlitTexturePS.cso");
}

void Bullet::Uninit()
{


    if (m_VertexShader) { m_VertexShader->Release(); m_VertexShader = nullptr; }
    if (m_PixelShader) { m_PixelShader->Release(); m_PixelShader = nullptr; }
    if (m_VertexLayout) { m_VertexLayout->Release(); m_VertexLayout = nullptr; }

    GameObject::Uninit();
}

void Bullet::Update(double deltaTime)
{

    float dt = static_cast<float>(deltaTime);

    // 速度に基づいて位置を更新
    TransformComponent* transform = GetComponent<TransformComponent>();
    Vector3 pos = transform->GetPosition();
    pos += m_Velocity * dt;
    transform->SetPosition(pos);

    //敵との衝突判定
	auto enemies = Manager::GetGameObjects<Enemy>();
    for (auto enemy : enemies) {
		Vector3 direction = enemy->GetPosition() - pos;
        float length = direction.length();
        if (length < 1.0f) { // 衝突とみなす距離（例: 1.0f）
            enemy->SetDestroy(); // 敵を破壊
            
            // 爆発を生成
            Explosion* explosion = Manager::AddGameObject<Explosion>();
            explosion->Init();
            TransformComponent* explosionTransform = explosion->GetComponent<TransformComponent>();
            explosionTransform->SetPosition(enemy->GetPosition());
            auto* score = Manager::GetGameObject<Score>();
            score->SetNumber(score->GetNumber()+15);
            SetDestroy(); // 弾を破壊

            break; // 最初の衝突でループを抜ける
		}
    }


    m_LifeTime -= dt;

    

    if(m_LifeTime <= 0.0f) {

		SetDestroy();
    }

    GameObject::Update(dt);
}

void Bullet::Draw()
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


}
