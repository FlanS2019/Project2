#pragma once
#include <d3d11.h>
#include "vector3.h"

#include "gameObject.h"
class Field :public GameObject
{
private:
	

	

public:
	void Init();
	void Uninit();
	void Update(double);
	void Draw();
};