#pragma once
#include "vector3.h"
#include "component.h"
#include <list>

class GameObject;
#include "transform.h"

class GameObject
{
protected:
	std::list<Component*> m_Components;
	bool  m_Destroy = false;
	int   m_Layer = 1; // レイヤー（描画順などに使用）
	float m_CameraZ;//ソート用Z値
	GameObject* m_Parent = nullptr;
public:
	GameObject()
	{
		AddComponent<TransformComponent>(this);
	}
	float GetCameraZ()const { return m_CameraZ; }
	void CalcCameraZ(Vector3 camerapos, Vector3 cameraForward) {
		TransformComponent* a = GetComponent<TransformComponent>();
		// ワールド行列の平行移動成分(r[3])からワールド座標を取得する
		// 子オブジェクトの場合、GetPosition()はローカル座標のため不正確
		XMMATRIX world = a->GetWorldMatrix();
		XMFLOAT3 worldPos;
		XMStoreFloat3(&worldPos, world.r[3]);
		Vector3 dir = { worldPos.x - camerapos.x, worldPos.y - camerapos.y, worldPos.z - camerapos.z };
		m_CameraZ = Vector3::dot(dir, cameraForward);
	}


	void SetDestroy() { m_Destroy = true; }
	//bool IsDestroy() const { return m_Destroy; }
	bool Destroy() {
		if(m_Destroy) {
			Uninit();
			delete this;
			return true;
		}
		return false;
	}

	int GetLayer() { return m_Layer; }
	void SetLayer(int layer) { m_Layer = layer; }

	virtual ~GameObject() {}

	void SetPosition(const Vector3& position) { GetComponent<TransformComponent>()->SetPosition(position); }
	Vector3 GetPosition() const { return const_cast<GameObject*>(this)->GetComponent<TransformComponent>()->GetPosition(); }

	void SetRotation(const Vector3& rotation) { GetComponent<TransformComponent>()->SetRotation(rotation); }
	Vector3 GetRotation() const { return const_cast<GameObject*>(this)->GetComponent<TransformComponent>()->GetRotation(); }

	void SetScale(const Vector3& scale) { GetComponent<TransformComponent>()->SetScale(scale); }
	Vector3 GetScale() const { return const_cast<GameObject*>(this)->GetComponent<TransformComponent>()->GetScale(); }

	void SetParent(GameObject* parent) { m_Parent = parent; }
	GameObject* GetParent() { return m_Parent; }
	


	virtual void Init()  {};
	virtual void Uninit(){
		for (auto component : m_Components) {
			component->Uninit();
			delete component;
		}
	};
	virtual void Update(double deltaTime){
		for (auto component : m_Components) {
			component->Update(deltaTime);
		}
	};
	virtual void Draw() {
		for (auto component : m_Components) {
			component->Draw();
		}
	};

	

	 template<typename T>
	 T* AddComponent(GameObject* obj)
	 {
		 T* component = new T(obj);

		 component->Init();

		 m_Components.push_back(component);

		 return component;
	 }

	 template<typename T>	
	 T* GetComponent() {

		 for(auto component : m_Components) {
			 // dynamic_castを使って、Tが使えるかどうかを確認？
			 T* casted = dynamic_cast<T*>(component);
			 if(casted) return casted;
		 }

		 return nullptr;
	 }



	
};