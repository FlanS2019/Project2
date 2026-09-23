#pragma once

#include <d3d11.h>
#include "vector3.h"

#include "gameObject.h"
class Explosion :public GameObject
{
private:

	
	int m_Frame = 0;
	float m_Timer = 0.0f;


public:
	void Init();
	void Uninit();
	void Update(double);
	void Draw();
};