#ifdef __linux__
#include <SDL3/SDL.h>

extern "C" {

// Stubs for missing IBus symbols in some static SDL3 builds on Linux.
// These are internal SDL3 functions that might be missing if DBus/IBus support
// was partially disabled or inconsistently compiled in the static library.

bool SDL_IBus_Init(void) { return false; }

void SDL_IBus_Quit(void) {}

void SDL_IBus_SetFocus(bool focused) { (void)focused; }

void SDL_IBus_Reset(void) {}

bool SDL_IBus_ProcessKeyEvent(Uint32 keysym, Uint32 keycode, bool down) {
  (void)keysym;
  (void)keycode;
  (void)down;
  return false;
}

void SDL_IBus_UpdateTextInputArea(const SDL_Rect *rect) { (void)rect; }

void SDL_IBus_PumpEvents(void) {}
}
#endif
