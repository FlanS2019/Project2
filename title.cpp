#include "title.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "polygon2D.h"
#include "main.h"
#include "game.h"

void Title::Init()
{
	Manager::AddGameObject<Polygon2D>()->Init(0.0,0.0,SCREEN_WIDTH,SCREEN_HEIGHT,L"asset\\texture\\image.png");

}

void Title::Uninit()
{
}

void Title::Update()
{
	if (Input::GetKeyTrigger(VK_RETURN)) {
		Manager::ChangeScene<Game>();
	}

}

void Title::Draw()
{


}
