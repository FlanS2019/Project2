#pragma once
#include <d3d11.h>
#include "gameObject.h"
#include "Sprite.h"
class Score :public GameObject
{
private:
	SpriteComponent* m_Sprite = nullptr;
	int number;

public:
	void Init()override;
	void Uninit()override;
	void Update(double)override;
	void Draw()override;

	void SetNumber(int num);
	const int  GetNumber() { return number; }
};