#pragma once
#include "vector3.h"
#include "gameObject.h"
#include "transform.h"

class Camera : public GameObject
{
private:
	Vector3 m_Target{ 0.0f, 0.0f, 0.0f };
	XMMATRIX m_ViewMatrix;

	static bool s_FreeMode;               // フリーカメラモード中かどうか
	Vector3 m_FreePosition{ 0.0f, 20.0f, 30.0f }; // フリーモード時のカメラ座標
	Vector3 m_FreeRotation{ 0.0f, -XM_PI, 0.0f }; // フリーモード時の回転(x:見上げ下げ, y:左右)


public:
	void Init();
	void Uninit();
	void Update(double);
	void Draw();

	XMMATRIX GetViewMatrix() const { return m_ViewMatrix; }

	// フリーカメラモード中かどうか（Playerの入力停止に使う）
	static bool IsFreeMode() { return s_FreeMode; }

	virtual Vector3 GetForward() const {
		return const_cast<Camera*>(this)->GetComponent<TransformComponent>()->GetForward();
	}

	virtual Vector3 GetRight() const {
		return const_cast<Camera*>(this)->GetComponent<TransformComponent>()->GetRight();
	}
};