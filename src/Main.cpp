#include <iostream>

#include "./Game/Game.h"
#include "./General/Logger.h"

int main(int argc, char* argv[]) {
    Logger::Init();
    bool isMapEditor = false;

    if (argc > 1 && strcmp(argv[1], "-m") == 0) {
        Logger::Info("Map Editor mode enabled.");
        isMapEditor = true;
    }

    Game game;

    game.Initialize();
    game.Run(isMapEditor);
    game.Destroy();

    return 0;
}
