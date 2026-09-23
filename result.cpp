#include "result.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "title.h"
#include "polygon2D.h"
#include "main.h"
#include "game.h"


void Result::Init()
{
	Manager::AddGameObject<Polygon2D>()->Init(0.0, 0.0,
		SCREEN_WIDTH, SCREEN_HEIGHT, L"asset\\texture\\image1.png");

}

void Result::Uninit()
{

}

void Result::Update()
{
	if (Input::GetKeyTrigger(VK_RETURN)) {
		Manager::ChangeScene<Title>();
	}
}

void Result::Draw()
{

}
