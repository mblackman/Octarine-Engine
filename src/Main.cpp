#include "./Game/Game.h"
#include "./General/Logger.h"

int main(const int argc, char *argv[]) {
  Logger::Init();

  std::string gamePath;
  std::string startupMode;

  // Parse command-line arguments
  for (int i = 1; i < argc; ++i) {
    std::string currentArg = argv[i];

    if (currentArg == "-m" || currentArg == "--startup-mode") {
      if (i + 1 < argc) {
        startupMode = argv[++i];
        Logger::Info("Startup mode set to: " + startupMode);
      } else {
        Logger::Error("Error: " + currentArg + " flag requires a mode argument.");
        return 1;
      }
    } else if (currentArg.starts_with("-")) {
      Logger::Warn("Unknown command-line argument: " + currentArg);
    } else {
      gamePath = currentArg;
      Logger::Info("Game path set to: " + gamePath);
    }
  }

  Game game{};
  game.SetStartupMode(startupMode);

  if (game.Initialize(gamePath)) {
    game.Run();
  }
  game.Destroy();

  return 0;
}
