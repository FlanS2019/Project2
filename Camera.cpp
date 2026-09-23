#include "camera.h"
#include <DirectXMath.h>
#include "main.h"
#include "renderer.h"
#include "Player.h"
#include "manager.h"
#include "transform.h"
#include "input.h"


using namespace DirectX;

void Camera::Init()
{
	m_Layer = 0;
	TransformComponent* transform = GetComponent<TransformComponent>();
	transform->SetRotation(Vector3(0.0f, -XM_PI, 0.0f)); // 初期回転（ラジアン）
	transform->SetPosition(Vector3(0.0f, 20.0f, 30.0f));
	m_Target = Vector3(0.0f, 0.0f, 0.0f);
}
void Camera::Uninit()
{
	GameObject::Uninit();
}
void Camera::Update(double deltaTime)
{
	Player* player = Manager::GetGameObject<Player>();
	Vector3 playerPos = player->GetComponent<TransformComponent>()->GetPosition();
	float dt = static_cast<float>(deltaTime);
	
	TransformComponent* transform = GetComponent<TransformComponent>();
	Vector3 rotation = transform->GetRotation();

	if (Input::GetKeyPress(VK_RIGHT) || Input::GetKeyPress('E'))
	{
		rotation.y += 2.0f * dt; // 右に回転（ラジアン基準、約114度/秒）
	}
	if (Input::GetKeyPress(VK_LEFT) || Input::GetKeyPress('Q'))
	{
		rotation.y -= 2.0f * dt; // 左に回転（ラジアン基準）
	}
	transform->SetRotation(rotation);

	m_Target = playerPos + Vector3(0.0f, 2.0f, 0.0f); // プレイヤーを注視

	// カメラの位置を回転角度 rotation.y を用いて計算
	Vector3 position = m_Target + Vector3(-sinf(rotation.y) * 5.0f, 3.0f, -cosf(rotation.y) * 5.0f);
	transform->SetPosition(position);

	GameObject::Update(deltaTime);
}
void Camera::Draw()
{
	XMMATRIX projection = XMMatrixPerspectiveFovLH(1.0f,(float)SCREEN_WIDTH/SCREEN_HEIGHT, 1.0f, 1000.0f);
	Renderer::SetProjectionMatrix(projection);

	TransformComponent* transform = GetComponent<TransformComponent>();
	Vector3 pos = transform->GetPosition();

	XMFLOAT3 up = XMFLOAT3(0.0f,1.0f,0.0f);
	m_ViewMatrix = XMMatrixLookAtLH(XMLoadFloat3((XMFLOAT3*)&pos), XMLoadFloat3((XMFLOAT3*)&m_Target), XMLoadFloat3(&up));
	Renderer::SetViewMatrix(m_ViewMatrix);

	GameObject::Draw();
}