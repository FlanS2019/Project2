#include "main.h"
#include "renderer.h"
#include "score.h"
#include "DirectXTex.h"
#include "Sprite.h"

namespace
{
    constexpr int   kDigitCount = 4;
    constexpr float kCellSize = 100.0f;
    constexpr float kUVWidth = 1.0f / 5.0f;
    constexpr float kUVHeight = 1.0f / 5.0f; // 512x512を5x5グリッドで分割しているため1/5が正しい
}

void Score::Init()
{
    m_Layer = 3;
    TransformComponent* transform = GetComponent<TransformComponent>();
    transform->SetScale({ 1.f,1.f,1.f });
    transform->SetRotation({ 0,0,0 });

    // 4桁ぶんを1つのSpriteComponentにまとめる
    m_Sprite = AddComponent<SpriteComponent>(this);
    m_Sprite->SetCell(kCellSize, kCellSize, kDigitCount);
    m_Sprite->LoadTexture(L"asset\\texture\\number.png");
    m_Sprite->SetOffset(0.0f, 0.0f);

    SetNumber(0);
}

void Score::Uninit()
{
}

void Score::Update(double deltaTime) {
}

void Score::Draw()
{
    m_Sprite->Draw();
}

void Score::SetNumber(int num)
{
    number = num;

    for (int i = 0; i < kDigitCount; i++)
    {
        int digit = num % 10;
        num /= 10;
        int x = digit % 5;
        int y = digit / 5;

        // index = kDigitCount - 1 - i が左から数えた桁位置
        m_Sprite->SetCellUV(
            kDigitCount - 1 - i,
            x * kUVWidth,
            y * kUVHeight,
            kUVWidth,
            kUVHeight
        );
    }
}