#pragma once
#include "component.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include "vector3.h"
using namespace DirectX;


class TransformComponent : public Component
{
public:

    TransformComponent(GameObject* obj) : Component(obj) {}

  

    Vector3 position = {0.0f, 0.0f, 0.0f};
    Vector3 rotation = {0.0f, 0.0f, 0.0f};
    Vector3 scale = {1.0f, 1.0f, 1.0f};

	void SetPosition(const Vector3& pos) { position = pos; }
	void SetRotation(const Vector3& rot) { rotation = rot; }
	void SetScale(const Vector3& scl) { scale = scl; }


	Vector3 GetPosition() const { return position; }
	Vector3 GetRotation() const { return rotation; }
	Vector3 GetScale() const { return scale; }

    Vector3 GetForward() const {
        XMMATRIX rotM = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
		Vector3 forward = { 0.0f, 0.0f, 0.0f };
        XMStoreFloat3((XMFLOAT3*)&forward,rotM.r[2]);
		return forward;
    }
    Vector3 GetRight() const {
		XMMATRIX rotM = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
        Vector3 right = { 0.0f, 0.0f, 0.0f };
		XMStoreFloat3((XMFLOAT3*)&right, rotM.r[0]);
        return right;
	}
    

 


    XMMATRIX GetWorldMatrix() const;
};