#ifndef DINO_GAME
#define DINOGAME

#include "Game.h"
#include "Setting.h"
#include "Texture.h"
#include "Sprite.h"

namespace Engine {
	class DinoGame :public Engine::Game
	{
	public:
		DinoGame(Setting* setting);
		~DinoGame();
		virtual void Init();
		virtual void Update();
		virtual void Render();
	private:
		Engine::Texture* texture = NULL;
		Engine::Sprite* sprite = NULL;
		vector<Sprite*> platforms;
		vector<Sprite*> backgrounds, middlegrounds;
		void MoveLayer(vector<Sprite*>& bg, float speed);
		void DrawLayer(vector<Sprite*>& bg);
		void AddToLayer(vector<Sprite*>& bg, string name);
		float yVelocity = 0, gravity = 0, x = 150, y = 0, offset = 0;
		bool jump = false, crouch = false;
	};
}

#endif

