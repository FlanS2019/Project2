#include "main.h"
#include "renderer.h"
#include "grass.h"
#include "DirectXTex.h"
#include "transform.h"
#include "bilbordPlaneComponent.h"
void Grass::Init()
{


    m_Layer = 2;
    TransformComponent* transform = GetComponent<TransformComponent>();
    // transform->SetPosition({ 0.0f, 0.0f, 0.0f });
    transform->SetScale({ 1.f, 1.f, 1.f });
    transform->SetRotation({ 0.0f, 0.0f, 0.0f });
    BilbordPlaneComponent* sprite = AddComponent<BilbordPlaneComponent>(this);

    sprite->SetSize(1.0f, 1.0f);
    sprite->LoadTexture(L"asset\\texture\\grass.png");



}

void Grass::Uninit()
{

}

void Grass::Update(double deltaTime) {

}

void Grass::Draw()
{

    BilbordPlaneComponent* sprite = GetComponent<BilbordPlaneComponent>();
    sprite->Draw();



}