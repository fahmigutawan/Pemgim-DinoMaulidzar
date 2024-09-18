#include "DinoMaulidzar.h"

Engine::DinoMaulidzar::DinoMaulidzar(Setting* setting) :Engine::Game(setting)
{
	setting->windowTitle = "DINO GAME";
}

Engine::DinoMaulidzar::~DinoMaulidzar()
{
}


void Engine::DinoMaulidzar::Init()
{
	InitEnvironment();
	InitMaulidzar();
	InitObstacle();

	text = new Text("lucon.ttf", 24, defaultTextShader);
	text->SetScale(1.0f)->SetColor(255, 255, 255)->SetPosition(0, setting->screenHeight - (text->GetFontSize() * text->GetScale()));

}

void Engine::DinoMaulidzar::Update()
{
	if (isPlay) {
		UpdateEnvironment();
		UpdateMaulidzar();
		UpdateObstacles();
		CalculateScore();
		CollisionDetector();

		speed += 0.000005;
	}
	else {
		speed = defaultSpeed;
	}

	text->SetText(("Score = " + to_string(score)) );
}

void Engine::DinoMaulidzar::Render()
{
	RenderEnvironment();
	RenderMaulidzar();
	RenderObstacles();
	text->Draw();

}

void Engine::DinoMaulidzar::InitMaulidzar()
{
	textureMaulidzar = new Texture("maulidzar.png");
	spriteMaulidzar = new Sprite(textureMaulidzar, defaultSpriteShader, defaultQuad);

	spriteMaulidzar
		->SetNumXFrames(10)
		->SetNumYFrames(3)
		->AddAnimation("run", 0, 26)
		->AddAnimation("jump", 0,0)
		->SetAnimationDuration(20)
		->PlayAnim("run");

	spriteMaulidzar->SetBoundingBoxSize(spriteMaulidzar->GetScaleWidth() - 120,
		spriteMaulidzar->GetScaleHeight() - 120);

	inputManager
		->AddInputMapping("arrow-up", SDLK_UP);
}

void Engine::DinoMaulidzar::InitObstacle()
{
	textureObstacle = new Texture("cactus.png");

	for (int i = 0; i <= obstacleMaxCount; i++) {
		Sprite* obs = new Sprite(textureObstacle, defaultSpriteShader, defaultQuad);

		obs	
			->SetPosition(setting->screenWidth, 0)
			->SetScale(0.2f);

		obs->SetBoundingBoxSize(obs->GetScaleWidth() - 120,
			obs->GetScaleHeight() - 120);

		spritesObstacle.push_back(obs);
		obstacleAvailabilityStatus.push_back(true);
	}
}

void Engine::DinoMaulidzar::InitEnvironment()
{
	textureBackground = new Texture("back.png");
	textureGround = new Texture("ground.png");

	int counter = 0;

	for (int i = 0; i <= environmentMaxCount; i++) {
		float offsetX = counter * setting->screenWidth;
		Sprite* back = new Sprite(textureBackground, defaultSpriteShader, defaultQuad);
		Sprite* ground = new Sprite(textureGround, defaultSpriteShader, defaultQuad);

		back
			->SetSize(setting->screenWidth + environmentOffset, setting->screenHeight)
			->SetPosition(offsetX, 0);

		ground
			->SetSize(setting->screenWidth + environmentOffset, 200)
			->SetPosition(offsetX, 0);

		spritesBackground.push_back(back);
		spritesGround.push_back(ground);

		counter++;
	}
}

void Engine::DinoMaulidzar::UpdateMaulidzar()
{
	JumpPhysic();

	spriteMaulidzar
		->PlayAnim("run");

	if (inputManager->IsKeyPressed("arrow-up") && !isJump) {
		Jump();
	}

	yMaulidzar += yVelocity * GetGameTime();

	spriteMaulidzar
		->SetPosition(xMaulidzar, yMaulidzar)
		->Update(GetGameTime());
}

void Engine::DinoMaulidzar::UpdateObstacles() {
	int random = rand() % 200;
	nextObstacleDelayWaitTime = nextObstacleDelayWaitTime + random;

	for (Sprite* s : spritesObstacle) {
		MoveObstacleToScreen(s);
	}

	obstacleDelayWaitTimeCounter += GetGameTime();
}

void Engine::DinoMaulidzar::UpdateEnvironment() {
	int counterBackground = 0;
	int counterGround = 0;

	for (Sprite* s : spritesBackground) {
		float offsetX = counterBackground * setting->screenWidth;

		if (s->GetPosition().x < -setting->screenWidth - offsetX) {
			s->SetPosition(setting->screenWidth + offsetX, 0);
		}

		s->SetPosition(s->GetPosition().x - speed * GetGameTime(), 0);
		s->Update(GetGameTime());

		counterBackground++;
		if (counterBackground >= spritesBackground.size() - 1) {
			counterBackground = 0;
		}
	}

	for (Sprite* s : spritesGround) {
		float offsetX = counterGround * setting->screenWidth;

		if (s->GetPosition().x < -setting->screenWidth - offsetX) {
			s->SetPosition(setting->screenWidth + offsetX, 0);
		}

		s->SetPosition(s->GetPosition().x - speed * GetGameTime(), 0);
		s->Update(GetGameTime());

		counterGround++;
		if (counterGround >= spritesGround.size() - 1) {
			counterGround = 0;
		}
	}
}

void Engine::DinoMaulidzar::RenderMaulidzar()
{
	spriteMaulidzar->Draw();
}

void Engine::DinoMaulidzar::RenderObstacles()
{
	for (Sprite* s : spritesObstacle) {
		s->Draw();
	}
}

void Engine::DinoMaulidzar::RenderEnvironment()
{
	for (Sprite* s : spritesBackground) {
		s->Draw();
	}

	for (Sprite* s : spritesGround) {
		s->Draw();
	}
}

void Engine::DinoMaulidzar::StartGame()
{
	//TODO
}

void Engine::DinoMaulidzar::RestartGame()
{
	//TODO
}

void Engine::DinoMaulidzar::StopGameBecauseOfDeath()
{
	//TODO
}

void Engine::DinoMaulidzar::PlayRunAnimation()
{
	//TODO
}

void Engine::DinoMaulidzar::PlayIdleAnimation()
{
	//TODO
}

void Engine::DinoMaulidzar::PlayJumpAnimation()
{
	//TODO
}

void Engine::DinoMaulidzar::Jump()
{
	float ratio = (GetGameTime() / 16.7f);
	gravity = 0.12f * ratio;
	yVelocity = 2.2f;
	isJump = true;
}

void Engine::DinoMaulidzar::JumpPhysic() {
	if (yMaulidzar > -30) {
		yVelocity -= gravity;
		spriteMaulidzar->PlayAnim("jump");
	} else if (yMaulidzar < -30) {
		isJump = false;
		yVelocity = 0;
		yMaulidzar = -30;
		spriteMaulidzar->PlayAnim("run");
	}
}

void Engine::DinoMaulidzar::CalculateScore()
{
	score += speed * 1.2;
}

void Engine::DinoMaulidzar::CollisionDetector() {
	for (Sprite* obs : spritesObstacle) {
		if (obs->GetBoundingBox()->CollideWith(spriteMaulidzar->GetBoundingBox())) {
			isPlay = false;
		}
	}
}

void Engine::DinoMaulidzar::MoveObstacleToScreen(Sprite* s) {
	if(s->GetPosition().x < -setting->screenWidth) {
		s->SetPosition(setting->screenWidth, 0);
	}

	s->SetPosition(s->GetPosition().x - speed * GetGameTime(), 0);
}