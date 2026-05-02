#include <iostream>

#include "./Game/Game.h"
#include "./General/Logger.h"

int main(int argc, char* argv[]) {
  Logger::Init();

  std::string gamePath;

  // Parse command-line arguments
  for (int i = 1; i < argc; ++i) {
    std::string currentArg = argv[i];

    if (currentArg == "-p" || currentArg == "--path") {
      if (i + 1 < argc) {
        gamePath = argv[++i];
        Logger::Info("Game path set to: " + gamePath);
      } else {
        Logger::Error(
            "Error: " + currentArg + " flag requires a path argument.");
        return 1;
      }
    } else {
      Logger::Warn("Unknown command-line argument: " + currentArg);
    }
  }

  Game game(gamePath);

  game.Initialize();
  game.Run();
  game.Destroy();

  return 0;
}
