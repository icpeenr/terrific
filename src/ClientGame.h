#pragma once

#include "Common.h"
#include "Game.h"

class ClientGame : public Game
{
private:
	ClientGame();
public:
	~ClientGame();
public:
	static ClientGame*	GetInstance();
public:
	void LoadContent();
	void Initialize();
	void Run();
	void UnloadContent();
private:
	void Update(float elapsedTime);
	void Draw();
private:
	static ClientGame*	m_pInstance;
};
