#include "DinoGame.h"

Engine::DinoGame::DinoGame(Setting* setting) :Engine::Game(setting)
{
	setting->windowTitle = "DINO GAME";
}

Engine::DinoGame::~DinoGame()
{
}

void Engine::DinoGame::Init()
{
	texture = new Texture("capy_maulidzar.png");
	sprite = new Sprite(texture, defaultSpriteShader, defaultQuad);
	sprite
		->SetNumXFrames(5)
		->SetNumYFrames(2)
		->AddAnimation("run", 0, 4)
		->AddAnimation("jump", 0,4)
		->SetScale(1)
		->SetAnimationDuration(85)
		->SetPosition(x, y)
		->PlayAnim("run");

	inputManager
		->AddInputMapping("arrow-up", SDLK_UP);

	for (int i = 0; i <= 4; i++) {
		AddToLayer(backgrounds, "bg0" + to_string(i) + ".png");
	}
	for (int i = 5; i <= 8; i++) {
		AddToLayer(middlegrounds, "bg0" + to_string(i) + ".png");
	}

	offset = 2;
}

void Engine::DinoGame::Update()
{
	sprite->PlayAnim("run");

	//JUMP LOGIC
	if (inputManager->IsKeyPressed("arrow-up") && !jump) {
		float ratio = (GetGameTime() / 16.7f);
		gravity = 0.12f * ratio;
		yVelocity = 1.6f;
		jump = true;
	}

	if (y > 0) {
		yVelocity -= gravity;
		sprite->PlayAnim("jump");
	} else if (y < 0) {
		jump = false;
		yVelocity = 0;
		y = 0;
	}

	MoveLayer(backgrounds, 0.005f);
	MoveLayer(middlegrounds, 0.03f);

	y += yVelocity * GetGameTime();
	sprite->SetPosition(x, y);
	sprite->Update(GetGameTime());
}

void Engine::DinoGame::Render()
{
	DrawLayer(backgrounds);
	DrawLayer(middlegrounds);
	sprite->Draw();
}

void Engine::DinoGame::DrawLayer(vector<Sprite*>& bg)
{
	for (Sprite* s : bg) {
		s->Draw();
	}
}

void Engine::DinoGame::AddToLayer(vector<Sprite*>& bg, string name)
{
	Texture* texture = new Texture(name);

	Sprite* s = new Sprite(texture, defaultSpriteShader, defaultQuad);
	s->SetSize(setting->screenWidth + offset, setting->screenHeight);
	bg.push_back(s);

	Sprite* s2 = new Sprite(texture, defaultSpriteShader, defaultQuad);
	s2->SetSize(setting->screenWidth + offset, setting->screenHeight)->SetPosition(setting->screenWidth + offset - 1, 0);
	bg.push_back(s2);
}

void Engine::DinoGame::MoveLayer(vector<Sprite*>& bg, float speed)
{
	for (Sprite* s : bg) {
		if (s->GetPosition().x < -setting->screenWidth + offset) {
			s->SetPosition(setting->screenWidth + offset - 1, 0);
		}
		s->SetPosition(s->GetPosition().x - speed * GetGameTime(), s->GetPosition().y);
		s->Update(GetGameTime());
	}
}

