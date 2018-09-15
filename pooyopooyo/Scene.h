#pragma once

#include <vector>
#include <SDL.h>
#include "Input.h"
#include "Pooyo.h"
#include "Board.h"
class GameManager;
class Scene;

class SceneManager
{
private:
	GameManager * gameManager;
	std::vector<Scene*> scenes;

public:
	static SceneManager* mInstance;

private:
	SceneManager();
	~SceneManager();

public:
	static SceneManager & GetInstance();
	void ChangeScene(int sceneIndex);
};

class Scene
{
protected:
	InputHandler * inputHandler;
	SDL_Renderer * renderer;

public:
	virtual void Initialize() = 0;
	virtual void Release() = 0;

	virtual void HandleInput() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;
};

class PooyoParticle;
class StartMenuScene : public Scene
{
private:
	int BG_OffsetX, BG_OffsetY;
	std::vector<PooyoParticle*> particles;

	SDL_Texture * background, *title, *bottomText;
	SDL_Texture * pooyo[POOYO_COLOR_COUNT];

public:
	~StartMenuScene();

	virtual void Initialize();
	virtual void Release();

	virtual void HandleInput();
	virtual void Update(float deltaTime);
	virtual void Render();

private:
	void spurt();

};

class SoloGameScene : public Scene
{
private:
	SDL_Texture * background;
	SDL_Texture* pooyo[POOYO_COLOR_COUNT];

public:
	Board * board;

public:
	~SoloGameScene();

	virtual void Initialize();
	virtual void Release();

	virtual void HandleInput();
	virtual void Update(float deltaTime);
	virtual void Render();
};