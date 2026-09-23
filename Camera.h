#pragma once
#include "vector3.h"
#include "gameObject.h"
#include "transform.h"

class Camera : public GameObject
{
private:
	Vector3 m_Target{ 0.0f, 0.0f, 0.0f };
	XMMATRIX m_ViewMatrix;


public:
	void Init();
	void Uninit();
	void Update(double);
	void Draw();

	XMMATRIX GetViewMatrix() const { return m_ViewMatrix; }


	virtual Vector3 GetForward() const {
		return const_cast<Camera*>(this)->GetComponent<TransformComponent>()->GetForward();
	}

	virtual Vector3 GetRight() const {
		return const_cast<Camera*>(this)->GetComponent<TransformComponent>()->GetRight();
	}
};