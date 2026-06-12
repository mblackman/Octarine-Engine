#include <SDL3/SDL_main.h>

#include "./Game/Game.h"
#include "./General/Logger.h"

// <SDL3/SDL_main.h> renames this main to SDL_main and supplies the real platform entry point. On
// desktop that wrapper is a thin shim; on Android it is what SDLActivity calls into via JNI (the
// activity dlopens libmain.so and invokes "SDL_main"), so the include is required for the mobile build
// and harmless on desktop.
int main(const int argc, char* argv[]) {
  Logger::Init();

  std::string gamePath;
  std::string startupMode;
  bool useManifest = false;
  int devListenPort = 0;  // 0 = disabled; >0 = bind on that TCP port

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
    } else if (currentArg == "--use-manifest") {
      // Dev override: force the manifest-load branch from a non-shipped binary (a shipped build
      // uses it unconditionally). Lets the asset_manifest.lua path be exercised without packaging.
      useManifest = true;
      Logger::Info("Asset manifest loading forced ON (--use-manifest).");
    } else if (currentArg == "--dev-listen") {
#ifdef OCTARINE_SHIPPED
      Logger::Warn("--dev-listen is stripped from shipped builds; ignoring.");
      if (i + 1 < argc) ++i;  // consume the port argument so it does not parse as gamePath
#else
      if (i + 1 < argc) {
        try {
          devListenPort = std::stoi(argv[++i]);
        } catch (const std::exception&) {
          Logger::Error("Error: --dev-listen requires an integer port (got '" + std::string(argv[i]) + "').");
          return 1;
        }
        if (devListenPort <= 0 || devListenPort > 65535) {
          Logger::Error("Error: --dev-listen port out of range (got " + std::to_string(devListenPort) + ").");
          return 1;
        }
        Logger::Info("Dev-listen requested on port " + std::to_string(devListenPort) + ".");
      } else {
        Logger::Error("Error: --dev-listen flag requires a port argument.");
        return 1;
      }
#endif
    } else if (currentArg.starts_with("-")) {
      Logger::Warn("Unknown command-line argument: " + currentArg);
    } else {
      gamePath = currentArg;
      Logger::Info("Game path set to: " + gamePath);
    }
  }

  // Bake is headless and needs no Game instance, window, or loop — run it and exit with a status
  // a CI gate can read (0 = manifest written, 1 = config/catalog/write failure).
  if (startupMode == "bake") {
    return Game::Bake(gamePath) ? 0 : 1;
  }

  Game game{};
  game.SetStartupMode(startupMode);
  game.SetUseManifest(useManifest);
#ifndef OCTARINE_SHIPPED
  game.SetDevListen(devListenPort);
#else
  (void)devListenPort;
#endif

  if (game.Initialize(gamePath)) {
    game.Run();
  }
  game.Destroy();

  return 0;
}
