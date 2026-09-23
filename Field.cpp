#include "main.h"
#include "renderer.h"
#include "field.h"
#include "DirectXTex.h"
#include "transform.h"
#include "planeComponent.h"
#include "audio.h"
void Field::Init()
{

    m_Layer = 1;

    TransformComponent* transform = GetComponent<TransformComponent>();
    transform->SetPosition({ 0.0f, 0.0f, 0.0f });
    transform->SetScale({ 0.1f, 0.1f, 0.1f });
	transform->SetRotation({ -XM_PIDIV2, 0.0f, 0.0f });
    PlaneComponent* sprite = AddComponent<PlaneComponent>(this);

    sprite->SetSize(1000.0f,1000.0f);
    sprite->LoadTexture(L"asset\\texture\\jimen.jpg");
    Audio* bgm = AddComponent<Audio>(this);
    bgm->Load("asset\\audio\\bgm.wav");
    //bgm->Play(true);

}

void Field::Uninit()
{



    GameObject::Uninit();
}

void Field::Update(double deltaTime) {

}

void Field::Draw()
{

	PlaneComponent* sprite = GetComponent<PlaneComponent>();
	sprite->Draw();


 
}