#pragma once
#include <vector>
#include <list>
class GameObject;
class Scene;

class Manager
{

private:
	static std::list<GameObject*> m_GameObjects;
	static Scene* m_Scene;
	static Scene* m_NextScene;
	static float m_ChangeTime;
public:
	static void Init();
	static void Uninit();
	static void Update(double);
	static void Draw();

	template<typename T>
	static void ChangeScene(float Time = 0.0f) {
		if(m_NextScene == nullptr)
		{
			m_ChangeTime = Time;
			m_NextScene = new T();
		}
	}


	template<typename T>
	static T* AddGameObject() {
		T* obj = new T();
		obj->Init();
		m_GameObjects.push_back(obj);
		return obj;
	}


	template<typename T>
	static T* GetGameObject() {
		for (GameObject* obj : m_GameObjects) {
			T* casted = dynamic_cast<T*>(obj);//RTTI(é¿çséûå^èÓïÒ)ÇøÇÂÇ¢èdÇﬂ
			if (casted) return casted;
		}
		return nullptr;
	}
	template<typename T>
	static std::vector<T*> GetGameObjects() {
		std::vector<T*> result;
		for (GameObject* obj : m_GameObjects) {
			T* casted = dynamic_cast<T*>(obj);
			if (casted) result.push_back(casted);
		}
		return result;
	}



};