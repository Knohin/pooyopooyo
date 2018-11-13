#pragma once
#include "Pooyo.h"
#include "Board.h"
#include "Input.h"
#include "Scene.h"
#include <SDL_render.h>
#include <vector>

class GameManager
{
private:
	static GameManager* mInstance;
	bool isRunning;
	bool isSceneChanging;
	int nextSceneIndex;
	float alpha;
	float deltaAlpha;

public:
	Scene* curScene;


private:
	GameManager();
	~GameManager();
public:
	void Initialize(char* windowTitle);
	static GameManager& GetInstance();

	bool IsRunning();
	void StopRunning();

	void Update(float deltaTime);
	void Render();

	void ChangeScene(int sceneIndex);
};