#include "main.h"
#include "renderer.h"
#include "particle.h"
#include "DirectXTex.h"
#include "transform.h"
#include "bilbordPlaneComponent.h"
#include "input.h"
#include "manager.h"
#include "player.h"

bool		g_bOnClick;

void Particle::Init()
{

    //透明オブジェクトは描画順を考慮してレイヤーを2に設定
    m_Layer = 2;
    TransformComponent* transform = GetComponent<TransformComponent>();
    transform->SetScale({ 1.f, 1.f, 1.f });
    transform->SetRotation({ 0.0f, 0.0f, 0.0f });
    BilbordPlaneComponent* sprite = AddComponent<BilbordPlaneComponent>(this);

    sprite->SetSize(2.0f, 2.0f);
    sprite->LoadTexture(L"asset\\texture\\particle.png");

    for (int i = 0; i < PATICLE_MAX; i++) {
        m_Particle[i].Enable = false;
    }
}

void Particle::Uninit()
{
}

void Particle::Update(double deltaTime) {
    int count = 3;
  
    if(Input::GetKeyPress(VK_RETURN))
    {
        // 発生位置は常にプレイヤー位置を使う（プレイヤーがいなければ自身のTransformを使用）
        Vector3 emitPos;
        Player* player = Manager::GetGameObject<Player>();
        if (player) {
            emitPos = player->GetPosition();
        } else {
            TransformComponent* transform = GetComponent<TransformComponent>();
            emitPos = transform->GetPosition();
        }

        for (int i = 0; i < PATICLE_MAX; i++) {
            if (!m_Particle[i].Enable) {
                m_Particle[i].Enable = true;
                m_Particle[i].Life = 60;
                m_Particle[i].Position = emitPos;
                m_Particle[i].Velocity = {
                    ((float)rand() / RAND_MAX - 0.5f) * 30.0f,
                    ((float)rand() / RAND_MAX - 0.5f) * 30.0f,
                    ((float)rand() / RAND_MAX - 0.5f) * 30.0f,
                };
                count--;
                if (count <= 0)
                    break;
            }
        }
    }
    Vector3 gravity{ 0.0f,-9.8f,0.0f };

    for (int i = 0; i < PATICLE_MAX; i++) {
        if (m_Particle[i].Enable) {
            m_Particle[i].Velocity += gravity * deltaTime;
            m_Particle[i].Position += m_Particle[i].Velocity * deltaTime;

            m_Particle[i].Life--;
            if (m_Particle[i].Life < 0) {
                m_Particle[i].Enable = false;
            }
        }
    }
}

void Particle::Draw()
{
    // パーティクル全体を加算ブレンドで描画（光らせるため）
    Renderer::SetAdditiveEnable(true);
    Renderer::SetDepthEnable(false);

    BilbordPlaneComponent* sprite = GetComponent<BilbordPlaneComponent>();
    for (int i = 0; i < PATICLE_MAX; i++) {
        if (m_Particle[i].Enable) {
            // 各パーティクルはコンポーネントの位置を一時的にセットして描画
            sprite->SetPosition({ m_Particle[i].Position.x, m_Particle[i].Position.y, m_Particle[i].Position.z });
            sprite->Draw();
        }
    }

    Renderer::SetDepthEnable(true);
    Renderer::SetAdditiveEnable(false);
}