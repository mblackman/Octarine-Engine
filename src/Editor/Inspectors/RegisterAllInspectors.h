#pragma once

#ifdef OCTARINE_WITH_EDITOR

// Populates ComponentInspectorRegistry with every inspectable component.
// Call once during Game::Setup (editor builds only).
void RegisterAllComponentInspectors();

#endif  // OCTARINE_WITH_EDITOR
