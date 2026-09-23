#pragma once
#include "scene.h"
class Result :public Scene
{


public:
	void Init()  override;
	void Uninit()override;
	void Update()override;
	void Draw()override;



};