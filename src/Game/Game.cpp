#include "Game.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_sdlrenderer2.h>

#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <sol/sol.hpp>
#include <string>
#include <vector>

#include "../ECS/ECS.h"
#include "../Events/KeyInputEvent.h"
#include "../General/Logger.h"
#include "../MapEditor/MapEditor.h"
#include "../Renderer/RenderQueue.h"
#include "../Renderer/Renderer.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CameraFollowSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/DisplayHealthSystem.h"
#include "../Systems/DrawColliderSystem.h"
#include "../Systems/KeyboardControlSystem.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/ProjectileEmitSystem.h"
#include "../Systems/ProjectileLifecycleSystem.h"
#include "../Systems/RenderGUISystem.h"
#include "../Systems/RenderPrimitiveSystem.h"
#include "../Systems/RenderSpriteSystem.h"
#include "../Systems/RenderTextSystem.h"
#include "../Systems/ScriptSystem.h"
#include "../Systems/UIButtonSystem.h"
#include "./LevelLoader.h"

int Game::windowWidth;
int Game::windowHeight;
int Game::mapWidth;
int Game::mapHeight;

Game::Game() : window_(nullptr),
               sdl_renderer_(nullptr),
               camera_(),
               show_colliders_(false),
               milliseconds_previous_frame_(),
               render_queue_() {
    registry_ = std::make_unique<Registry>();
    asset_manager_ = std::make_unique<AssetManager>();
    event_bus_ = std::make_unique<EventBus>();
    renderer_ = std::make_unique<Renderer>();
    Logger::Info("Game Constructor called.");
}

Game::~Game() {
    Logger::Info("Game Destructor called.");
}

void Game::Initialize() {
    auto result = SDL_Init(SDL_INIT_EVERYTHING);

    if (result != 0) {
        Logger::Error("SDL_Init Error: " + std::string(SDL_GetError()));
        return;
    }

    if (TTF_Init() != 0) {
        Logger::Error("TTF_Init Error: " + std::string(TTF_GetError()));
        return;
    }

    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    windowWidth = displayMode.w;
    windowHeight = displayMode.h;

    window_ = SDL_CreateWindow(
        "Potato Face",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS);

    if (!window_) {
        Logger::Error("SDL_CreateWindow Error: " + std::string(SDL_GetError()));
        return;
    }

    sdl_renderer_ = SDL_CreateRenderer(window_, -1, 0);

    if (!sdl_renderer_) {
        Logger::Error("SDL_CreateRenderer Error: " + std::string(SDL_GetError()));
        return;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForSDLRenderer(window_, sdl_renderer_);
    ImGui_ImplSDLRenderer2_Init(sdl_renderer_);

    camera_.x = 0;
    camera_.y = 0;
    camera_.w = windowWidth;
    camera_.h = windowHeight;

    SDL_SetRenderDrawColor(sdl_renderer_, 21, 21, 21, 255);

    s_is_running_ = true;
}

void Game::Destroy() {
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(sdl_renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

void Game::Run(bool isMapEditor) {
    Setup(isMapEditor);

    while (s_is_running_) {
        ProcessInput();
        Update();
        Render();
    }
}

void Game::Setup(bool isMapEditor) {
    registry_->AddSystem<CameraFollowSystem>();
    registry_->AddSystem<ProjectileEmitSystem>();
    registry_->AddSystem<ProjectileLifecycleSystem>();
    registry_->AddSystem<DisplayHealthSystem>();
    registry_->AddSystem<DamageSystem>();
    registry_->AddSystem<MovementSystem>();

    registry_->AddSystem<RenderSpriteSystem>();
    registry_->AddSystem<RenderTextSystem>();
    registry_->AddSystem<RenderPrimitiveSystem>();
    registry_->AddSystem<RenderGUISystem>();
    registry_->AddSystem<AnimationSystem>();
    registry_->AddSystem<CollisionSystem>();
    registry_->AddSystem<DrawColliderSystem>();
    registry_->AddSystem<KeyboardControlSystem>();
    registry_->AddSystem<ScriptSystem>();
    registry_->AddSystem<UIButtonSystem>();

    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::io);
    lua["game_window_width"] = windowWidth;
    lua["game_window_height"] = windowHeight;
    registry_->GetSystem<ScriptSystem>().CreateLuaBindings(lua);

    if (isMapEditor) {
        MapEditor editor;
        editor.Load(lua, registry_, asset_manager_, sdl_renderer_);
    } else {
        LevelLoader loader;
        loader.LoadLevel(lua, registry_, asset_manager_, sdl_renderer_, 1);
    }
}

void Game::ProcessInput() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        auto& io = ImGui::GetIO();
        int mouseX, mouseY;
        const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
        io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
        io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
        io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

        switch (event.type) {
            case SDL_QUIT:
                s_is_running_ = false;
                break;

            case SDL_KEYDOWN:
            case SDL_KEYUP: {
                KeyInputEvent keyInputEvent = GetKeyInputEvent(&event.key);
                event_bus_->EmitEvent<KeyInputEvent>(keyInputEvent);
                break;
            }
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP: {
                SDL_MouseButtonEvent mouseButtonEvent = event.button;
                event_bus_->EmitEvent<MouseInputEvent>(mouseButtonEvent);
                break;
            }
            default:
                break;
        }
    }
}

void Game::Update() {
    // If we are too fast, waste some time until we reach the frame time
    int timeToWait = kMillisecondsPerFrame - (SDL_GetTicks() - milliseconds_previous_frame_);
    if (timeToWait > 0 && timeToWait <= kMillisecondsPerFrame) {
        SDL_Delay(timeToWait);
    }

    // Subscribe to events
    event_bus_->Reset();
    registry_->GetSystem<DamageSystem>().SubscribeToEvents(event_bus_);
    registry_->GetSystem<KeyboardControlSystem>().SubscribeToEvents(event_bus_);
    registry_->GetSystem<ProjectileEmitSystem>().SubscribeToEvents(event_bus_);
    registry_->GetSystem<MovementSystem>().SubscribeToEvents(event_bus_);
    registry_->GetSystem<UIButtonSystem>().SubscribeToEvents(event_bus_);
    registry_->GetSystem<ScriptSystem>().SubscribeToEvents(event_bus_);
    SubscribeToEvents(event_bus_);

    // Calculate delta time
    double deltaTime = (SDL_GetTicks() - milliseconds_previous_frame_) / 1000.0;

    milliseconds_previous_frame_ = SDL_GetTicks();

    registry_->GetSystem<MovementSystem>().Update(deltaTime);
    registry_->GetSystem<AnimationSystem>().Update();
    registry_->GetSystem<CollisionSystem>().Update(event_bus_);
    registry_->GetSystem<DamageSystem>().Update();
    registry_->GetSystem<KeyboardControlSystem>().Update();
    registry_->GetSystem<CameraFollowSystem>().Update(camera_);
    registry_->GetSystem<ProjectileEmitSystem>().Update(registry_);
    registry_->GetSystem<ProjectileLifecycleSystem>().Update();
    registry_->GetSystem<DisplayHealthSystem>().Update(registry_);
    registry_->GetSystem<ScriptSystem>().Update(deltaTime, SDL_GetTicks());
    registry_->Update();
}

void Game::Render() {
    SDL_SetRenderDrawColor(sdl_renderer_, 21, 21, 21, 255);
    SDL_RenderClear(sdl_renderer_);

    // Render the game
    render_queue_.Clear();
    registry_->GetSystem<RenderSpriteSystem>().Update(render_queue_, camera_);
    registry_->GetSystem<RenderTextSystem>().Update(render_queue_);
    registry_->GetSystem<RenderPrimitiveSystem>().Update(render_queue_);

    render_queue_.Sort();
    renderer_->Render(render_queue_, sdl_renderer_, camera_, asset_manager_);

    if (show_colliders_) {
        registry_->GetSystem<DrawColliderSystem>().Update(sdl_renderer_, camera_);
        registry_->GetSystem<RenderGUISystem>().Update(sdl_renderer_, registry_);
    }

    SDL_RenderPresent(sdl_renderer_);
}

void Game::SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
    eventBus->SubscribeEvent<Game, KeyInputEvent>(this, &Game::OnKeyInputEvent);
}

void Game::OnKeyInputEvent(KeyInputEvent& event) {
    if (!event.isPressed) {
        return;
    }

    switch (event.inputKey) {
        case SDLK_ESCAPE:
            s_is_running_ = false;
            break;
        case SDLK_F5:
            show_colliders_ = !show_colliders_;
            break;
        default:
            break;
    }
}

KeyInputEvent Game::GetKeyInputEvent(SDL_KeyboardEvent* event) {
    bool isPressed = event->state == SDL_PRESSED;
    return KeyInputEvent(
        event->keysym.sym,
        static_cast<SDL_Keymod>(event->keysym.mod),
        isPressed);
}
