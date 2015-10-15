#include "KeyboardManager.h"
#include <memory.h> // memset and memcpy
#include <iostream> // debug

KeyboardManager::KeyboardManager()
{
	memset(m_bCurrentState, 0, SFML_KEY_LAST);
}

KeyboardManager::~KeyboardManager()
{
}

bool KeyboardManager::IsKeyDown(int key)
{
	return m_bCurrentState[key];
}

bool KeyboardManager::IsKeyUp(int key)
{
	return !m_bCurrentState[key];
}

bool KeyboardManager::IsKeyHeld(int key)
{
	return m_bCurrentState[key] && m_bPreviousState[key];
}

bool KeyboardManager::IsKeyPressed(int key)
{
	return m_bCurrentState[key] && !m_bPreviousState[key];
}

bool KeyboardManager::IsKeyReleased(int key)
{
	return !m_bCurrentState[key] && m_bPreviousState[key];
}

void KeyboardManager::ProcessEvent(sf::Event event)
{
	//memcpy(m_bPreviousState, m_bCurrentState, SFML_KEY_LAST);

	m_bCurrentState[(int)event.key.code] = true;
/*
	for(int a = 0; a < SFML_KEY_LAST; a++)
	{
		//m_bCurrentState[a] = ((event.key.code) == (sf::Keyboard::Key)a);

		if(m_bCurrentState[a])
		{
			m_bCurrentState[a] = true;
		}
	}*/
	}

	void KeyboardManager::Update()
	{
		memcpy(m_bPreviousState, m_bCurrentState, SFML_KEY_LAST);


		for(int i = 0; i < SFML_KEY_LAST; i++)
		{
			m_bCurrentState[i] = sf::Keyboard::isKeyPressed((sf::Keyboard::Key)i);

			if(m_bCurrentState[i])
			{
				m_bCurrentState[i] = true;
			}
		}
	}

	
	KeyboardManager* KeyboardManager::GetInstance()
	{
		if(m_pInstance == NULL)
		{
			m_pInstance = new KeyboardManager();
		}

		return m_pInstance;
	}

	KeyboardManager* KeyboardManager::m_pInstance = NULL;