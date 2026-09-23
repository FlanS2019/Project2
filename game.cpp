#include "game.h"
#include "polygon2D.h"
#include "field.h"
#include "camera.h"
#include "gameObject.h"
#include "player.h"
#include "enemy.h"
#include "sprite.h"
#include "transform.h"
#include "grass.h"
#include "tree.h"
#include "sky.h"
#include "box.h"
#include "particle.h"
#include "manager.h"
#include "result.h"
#include "score.h"
#include "shadow.h"
#include "meshField.h"
#include "wave.h"

void Game::Init() {
	

	Manager::AddGameObject<Camera>();

	Manager::AddGameObject<Player>();

	Manager::AddGameObject<Sky>();
	Manager::AddGameObject<Box>()->SetPosition({ -10.0f,0.0f,1.0f });
	//AddGameObject<Enemy>()->SetPosition({ -5.f, 0.0f, 1.0f });
	//AddGameObject<Enemy>()->SetPosition({ 0.f, 0.0f, 1.0f });
	Manager::AddGameObject<Enemy>()->SetPosition({ 5.f, 0.0f, 1.0f });

	/*Manager::AddGameObject<Tree>()->SetPosition({ 0.f, 0.0f, 0.0f });
		Manager::AddGameObject<Tree>()->SetPosition({ 3.f, 0.0f, 3.0f });
	Manager::AddGameObject<Grass>()->SetPosition({ 10.f, 0.0f, 1.0f });*/

	//Manager::AddGameObject<Field>();
	//AddGameObject<Bullet>()->SetPosition({ 0.f, 0.0f, 0.0f });
	Manager::AddGameObject<MeshField>();
	//GameObject* uiObj = Manager::AddGameObject<Score>();
	/*auto* sprite = uiObj->AddComponent<SpriteComponent>(uiObj);
	sprite->SetSize(400.0f, 60.0f);
	sprite->LoadTexture(L"asset\\texture\\number.png");*/

	Manager::AddGameObject<Particle>()->SetPosition({ 8.0f,1.0f,1.0f });
	//Manager::AddGameObject<Shadow>()->SetPosition({ 0.0f,0.01f,0.0f });

	Manager::AddGameObject<Wave>()->SetPosition({ 0.0f, 0.3f, 4.0f });

}
void Game::Uninit() {




}
void Game::Update() {
	auto enemies = Manager::GetGameObjects<Enemy>();
	if (enemies.size() == 0) {
		Manager::ChangeScene<Result>(2.0f);
	}
}
void Game::Draw() {

}