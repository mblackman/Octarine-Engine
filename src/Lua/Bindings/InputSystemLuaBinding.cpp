#include "Lua/Bindings/InputSystemLuaBinding.h"

#include <SDL3/SDL.h>

#include <algorithm>
#include <cctype>
#include <glm/glm.hpp>
#include <string>

#include "Components/ViewportInfo.h"
#include "ECS/Registry.h"
#include "Systems/InputSystem.h"

namespace
{
    int ToMouseButton(const sol::object& btn)
    {
        if (btn.is<int>()) return btn.as<int>();
        if (btn.is<std::string>())
        {
            const std::string name = InputSystem::MakeKey(btn.as<std::string>());
            if (name == "left") return SDL_BUTTON_LEFT;
            if (name == "middle") return SDL_BUTTON_MIDDLE;
            if (name == "right") return SDL_BUTTON_RIGHT;
            if (name == "x1") return SDL_BUTTON_X1;
            if (name == "x2") return SDL_BUTTON_X2;
        }
        return 0;
    }
} // namespace

void LuaSystemBinding<InputSystem>::bind(sol::state& lua, InputSystem& system)
{
    lua["input"] = lua.create_table();
    sol::table input = lua["input"];

    input.set_function("is_key_down", [&system](const std::string& name) { return system.IsKeyDown(name); });
    input.set_function("is_key_pressed", [&system](const std::string& name) { return system.IsKeyPressed(name); });
    input.set_function("is_key_released", [&system](const std::string& name) { return system.IsKeyReleased(name); });

    input.set_function("is_mouse_down", [&system](sol::object btn) { return system.IsMouseDown(ToMouseButton(btn)); });
    input.set_function("is_mouse_pressed",
                       [&system](sol::object btn) { return system.IsMousePressed(ToMouseButton(btn)); });
    input.set_function("is_mouse_released",
                       [&system](sol::object btn) { return system.IsMouseReleased(ToMouseButton(btn)); });
    input.set_function("is_hovered", [&system]()
    {
        return system.GetRegistry() ? system.GetRegistry()->Get<ViewportInfo>().isHovered : true;
    });
    input.set_function("is_focused", [&system]()
    {
        return system.GetRegistry() ? system.GetRegistry()->Get<ViewportInfo>().isFocused : true;
    });
    input.set_function("mouse_position",
                       [&system]() { return glm::vec2(system.MouseX(), system.MouseY()); });
    input.set_function("mouse_wheel",
                       [&system]() { return glm::vec2(system.WheelDx(), system.WheelDy()); });

    input.set_function("bind",
                       [&system](const std::string& action, const std::string& key) { system.Bind(action, key); });
    input.set_function("unbind", [&system](const std::string& action, sol::optional<std::string> key)
    {
        if (key)
        {
            system.Unbind(action, *key);
        }
        else
        {
            system.UnbindAction(action);
        }
    });
    input.set_function("is_action_down", [&system](const std::string& a) { return system.IsActionDown(a); });
    input.set_function("is_action_pressed", [&system](const std::string& a) { return system.IsActionPressed(a); });
    input.set_function("is_action_released", [&system](const std::string& a) { return system.IsActionReleased(a); });

    input.set_function("on_key_down",
                       [&system](sol::protected_function fn) { system.AddOnKeyDown(std::move(fn)); });
    input.set_function("on_key_up", [&system](sol::protected_function fn) { system.AddOnKeyUp(std::move(fn)); });
    input.set_function("on_mouse_down",
                       [&system](sol::protected_function fn) { system.AddOnMouseDown(std::move(fn)); });
    input.set_function("on_mouse_up",
                       [&system](sol::protected_function fn) { system.AddOnMouseUp(std::move(fn)); });
    input.set_function("on_mouse_wheel",
                       [&system](sol::protected_function fn) { system.AddOnMouseWheel(std::move(fn)); });
}
