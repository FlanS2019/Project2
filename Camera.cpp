#include "camera.h"
#include <DirectXMath.h>
#include "main.h"
#include "renderer.h"
#include "Player.h"
#include "manager.h"
#include "transform.h"
#include "input.h"


using namespace DirectX;

bool Camera::s_FreeMode = false;

void Camera::Init()
{
	m_Layer = 0;
	TransformComponent* transform = GetComponent<TransformComponent>();
	transform->SetRotation(Vector3(0.0f, -XM_PI, 0.0f)); // 初期回転（ラジアン）
	transform->SetPosition(Vector3(0.0f, 20.0f, 30.0f));
	m_Target = Vector3(0.0f, 0.0f, 0.0f);

	s_FreeMode = false;
	m_FreePosition = Vector3(0.0f, 20.0f, 30.0f);
	m_FreeRotation = Vector3(0.0f, -XM_PI, 0.0f);
}
void Camera::Uninit()
{
	GameObject::Uninit();
}
void Camera::Update(double deltaTime)
{
	float dt = static_cast<float>(deltaTime);
	TransformComponent* transform = GetComponent<TransformComponent>();

	// Cキーでフリーカメラモードのオン/オフを切り替え
	if (Input::GetKeyTrigger('C'))
	{
		s_FreeMode = !s_FreeMode;

		if (s_FreeMode)
		{
			// 切り替えた瞬間の位置・回転を引き継いでフリーモード開始
			m_FreePosition = transform->GetPosition();
			m_FreeRotation = transform->GetRotation();
		}
	}

	if (s_FreeMode)
	{
		// ---------------- フリーカメラモード ----------------
		// 矢印キーで視点回転（左右：Y軸、上下：X軸）
		if (Input::GetKeyPress(VK_RIGHT)) m_FreeRotation.y += 2.0f * dt;
		if (Input::GetKeyPress(VK_LEFT))  m_FreeRotation.y -= 2.0f * dt;
		if (Input::GetKeyPress(VK_UP))    m_FreeRotation.x += 1.5f * dt;
		if (Input::GetKeyPress(VK_DOWN))  m_FreeRotation.x -= 1.5f * dt;

		// 真上・真下を向きすぎないように角度を制限
		if (m_FreeRotation.x > XM_PIDIV2 - 0.1f)  m_FreeRotation.x = XM_PIDIV2 - 0.1f;
		if (m_FreeRotation.x < -XM_PIDIV2 + 0.1f) m_FreeRotation.x = -XM_PIDIV2 + 0.1f;

		transform->SetRotation(m_FreeRotation);

		Vector3 forward = transform->GetForward();
		Vector3 right = transform->GetRight();

		float freeSpeed = 20.0f;
		if (Input::GetKeyPress(VK_SHIFT)) freeSpeed *= 3.0f; // Shiftで高速移動

		if (Input::GetKeyPress('W')) m_FreePosition += forward * freeSpeed * dt;
		if (Input::GetKeyPress('S')) m_FreePosition -= forward * freeSpeed * dt;
		if (Input::GetKeyPress('D')) m_FreePosition += right * freeSpeed * dt;
		if (Input::GetKeyPress('A')) m_FreePosition -= right * freeSpeed * dt;
		if (Input::GetKeyPress(VK_SPACE)) m_FreePosition.y += freeSpeed * dt; // 上昇
		if (Input::GetKeyPress('X'))      m_FreePosition.y -= freeSpeed * dt; // 下降

		transform->SetPosition(m_FreePosition);
		m_Target = m_FreePosition + forward;
	}
	else
	{
		// ---------------- 通常モード（プレイヤー追従） ----------------
		Player* player = Manager::GetGameObject<Player>();
		Vector3 playerPos = player->GetComponent<TransformComponent>()->GetPosition();

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

		// フリーモードに入る時のためにも同期しておく
		m_FreePosition = position;
		m_FreeRotation = rotation;
	}

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
