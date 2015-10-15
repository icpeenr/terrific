#pragma once

#include "Common.h"

class Game
{
private:
	ex::EventManager m_Events;
	ex::EntityManager m_Entities;
	ex::SystemManager m_Systems;
protected:
	Game();//: m_Entities(m_Events), m_Systems(m_Entities, m_Events){}
	//,B2World(b2Vec2(0.0f, GRAVITY))
public:
	virtual ~Game();
protected:
	virtual void			Initialize();
	virtual void			LoadContent();
	virtual void			Update(float elapsedTime);
	virtual void			Draw();
	virtual void			UnloadContent();
	virtual void			Exit();
public:
	const float 			GetElapsedTime() const;
	const bool				IsExiting() const;
	/*
	ContentManager*			GetContentManager() const;
	ActorManager*			GetActorManager() const;
	CameraManager*			GetCameraManager() const;
	GraphicsDeviceManager*	GetGraphicsDeviceManager() const;
	GuiManager*				GetGuiManager() const;*/
	sf::RenderWindow		m_Window;
private:
	float					m_fElapsedTime;
	bool					m_bIsExiting;
	sf::Font				m_Font;
	sfg::SFGUI				m_Sfgui;
	sfg::Desktop			m_SfguiDesktop;
	sf::Clock				m_Clock;
	


	/*
	ContentManager*			m_pContentManager;
	ActorManager*			m_pActorManager;
	CameraManager*			m_pCameraManager;
	GraphicsDeviceManager*	m_pGraphicsDeviceManager;
	GuiManager*				m_pGuiManager;*/
};