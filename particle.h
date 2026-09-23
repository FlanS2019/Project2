#pragma once
#include <d3d11.h>
#include "vector3.h"

#include "gameObject.h"
class Particle :public GameObject
{
private:


	struct PATICLE
	{
		int status;
		bool Enable;
		int Life;
		Vector3 Position;
		Vector3 Velocity;
		Vector3 Acselalete;
	};

	static const int PATICLE_MAX = 10000;
	PATICLE m_Particle[PATICLE_MAX];


public:
	void Init();
	void Uninit();
	void Update(double);
	void Draw();
};