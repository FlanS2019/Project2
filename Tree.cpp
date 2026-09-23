#include "main.h"
#include "renderer.h"
#include "tree.h"
#include "DirectXTex.h"
#include "transform.h"
#include "bilbordPlaneComponent.h"
void Tree::Init()
{

	//透明オブジェクトは描画順を考慮してレイヤーを2に設定
    m_Layer = 2;
    TransformComponent* transform = GetComponent<TransformComponent>();
   // transform->SetPosition({ 0.0f, 0.0f, 0.0f });
    transform->SetScale({ 1.f, 1.f, 1.f });
    transform->SetRotation({0.0f, 0.0f, 0.0f });
    BilbordPlaneComponent* sprite = AddComponent<BilbordPlaneComponent>(this);

    sprite->SetSize(10.0f, 10.0f);
    sprite->LoadTexture(L"asset\\texture\\tree.png");



}

void Tree::Uninit()
{

}

void Tree::Update(double deltaTime) {

}

void Tree::Draw()
{

    BilbordPlaneComponent* sprite = GetComponent<BilbordPlaneComponent>();
    sprite->Draw();



}