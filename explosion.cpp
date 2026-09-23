#include "explosion.h"
#include "BilbordPlaneComponent.h"
void Explosion::Init()
{
    m_Layer = 2;
	TransformComponent* transform = GetComponent<TransformComponent>();
	// transform->SetPosition({ 0.0f, 0.0f, 0.0f });
	transform->SetScale({ 1.f, 1.f, 1.f });
	transform->SetRotation({ 0.0f, 0.0f, 0.0f });
	BilbordPlaneComponent* sprite = AddComponent<BilbordPlaneComponent>(this);

	sprite->SetSize(10.0f, 10.0f);
	sprite->LoadTexture(L"asset\\texture\\Explosion.png");

	sprite->SetUV(
		0.0f,
		0.0f,
		0.25f,
		0.25f);

}

void Explosion::Uninit()
{
	// 爆発の終了処理コードをここに記述


}

void Explosion::Update(double deltaTime)
{
    m_Timer += (float)deltaTime;

    if (m_Timer >= 0.05f)
    {
        m_Timer = 0.0f;

        m_Frame++;

        if (m_Frame >= 15)
        {
            SetDestroy();
            return;
        }

        BilbordPlaneComponent* sprite =
            GetComponent<BilbordPlaneComponent>();

        int x = m_Frame % 4;
        int y = m_Frame / 4;

        sprite->SetUV(
            x * 0.25f,
            y * 0.25f,
            0.25f,
            0.25f);
    }
}

void Explosion::Draw()
{
	BilbordPlaneComponent* sprite = GetComponent<BilbordPlaneComponent>();
	sprite->Draw();

}