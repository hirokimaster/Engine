#include "engine/Engine.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	std::unique_ptr<Framework> game = std::make_unique<Engine>();
	game->Run();
	return 0;
}