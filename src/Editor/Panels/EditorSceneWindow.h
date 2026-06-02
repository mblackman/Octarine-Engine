#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <SDL3/SDL.h>

class Game;

namespace octarine::editor::panels {

/// Scene View window: draws the off-screen game texture, aspect-fits + centers it, and writes the
/// resulting screen rect back into ViewportInfo so input systems can map cursor coordinates into
/// the shrunken game viewport.
void DrawSceneWindow(Game* game, SDL_Texture* gameTexture, bool* p_open);

}  // namespace octarine::editor::panels

#endif  // OCTARINE_WITH_EDITOR
