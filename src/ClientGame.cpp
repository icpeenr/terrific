#include "ClientGame.h"
#include "KeyboardManager.h"

ClientGame::ClientGame()
{
}

ClientGame::~ClientGame()
{
	//delete m_pBsp;
}

ClientGame* ClientGame::GetInstance()
{
	return m_pInstance;
}

void ClientGame::LoadContent()
{
	Game::LoadContent();

}

void ClientGame::Initialize()
{
	Game::Initialize();


	/*QuakeCamera* quakeCamera = new QuakeCamera();

	GetCameraManager()->Add(quakeCamera);
	GetCameraManager()->SetCurrentCamera(quakeCamera);

	MouseManager::GetInstance()->SetVisible(false);*/
}

void ClientGame::Update(float elapsedTime)
{
	
	if(KeyboardManager::GetInstance()->IsKeyPressed(sf::Keyboard::Key::Space))
	{
		Exit();
	}

	/*
	if(MouseManager::GetInstance()->IsButtonDown(MOUSEBUTTON_LEFT))
	{
		Camera* pCamera = GetCameraManager()->GetCurrentCamera();
		Vector3f vBegin = pCamera->GetPosition();
		Vector3f vForward;
		Vector3f vEnd;

		Matrix matrix;
		MatrixHelper::LoadIdentity(matrix);
		MatrixHelper::SetRotation(matrix, pCamera->GetAbsoluteRotation());
		MatrixHelper::GetForward(matrix, vForward);

		vEnd = vBegin + (vForward * 64.0f);

		m_vTraceLocation = m_pBsp->TraceRay(vBegin, vEnd);
	}
	*/
	Game::Update(elapsedTime);
}

void ClientGame::Draw()
{
	m_Window.clear();
	glClearColor(225, 225, 225, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	
	Game::Draw();
}

void ClientGame::UnloadContent()
{
	Game::UnloadContent();
}

void ClientGame::Run()
{
	float previousElapsedTime = 0.0f;

	float currentElapsedTime = GetElapsedTime();

	float elapsedTime = 0.0f;

	while(m_Window.isOpen() && !IsExiting())
	{
		previousElapsedTime = currentElapsedTime;

		currentElapsedTime = GetElapsedTime();

		elapsedTime = currentElapsedTime - previousElapsedTime;

		Update(elapsedTime);

		sf::sleep(sf::milliseconds(4));

		Draw();
	}
}

ClientGame* ClientGame::m_pInstance = new ClientGame();