#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

#define SFML_KEY_LAST sf::Keyboard::KeyCount

class KeyboardManager
{
public:
	KeyboardManager();
	~KeyboardManager();
public:
	bool IsKeyUp(int key);
	bool IsKeyDown(int key);
	bool IsKeyHeld(int key);
	bool IsKeyPressed(int key);
	bool IsKeyReleased(int key);
public:
	void ProcessEvent(sf::Event event);
	void Update();
private:
	bool m_bCurrentState[SFML_KEY_LAST];
	bool m_bPreviousState[SFML_KEY_LAST];
public:
	static KeyboardManager* GetInstance();
private:
	static KeyboardManager* m_pInstance;
};