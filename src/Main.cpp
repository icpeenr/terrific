#include "ClientGame.cpp"


int main() {

	ClientGame* game = ClientGame::GetInstance();

	game->Initialize();
	game->LoadContent();
	game->Run();
	game->UnloadContent();

	delete game;

	return 0;
}