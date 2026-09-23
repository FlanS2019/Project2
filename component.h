#pragma once



class Component
{

protected:

	class  GameObject* m_GameObject = nullptr;

public:
	Component() = delete;

	Component(GameObject* Object) { m_GameObject = Object; }
	virtual ~Component() {}

	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update(double) {};
	virtual void Draw() {};

};