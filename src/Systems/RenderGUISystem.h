#pragma once

#include <SDL2/SDL.h>
#include <imgui/imgui.h>

#include "../Components/BoxColliderComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"

class RenderGUISystem : public System {
   public:
    RenderGUISystem() {
    }

    ~RenderGUISystem() = default;

    void Update(SDL_Renderer* renderer, std::unique_ptr<Registry>& registry) {
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        SpawnEnemyWindow(registry);

        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
    }

   private:
    void SpawnEnemyWindow(std::unique_ptr<Registry>& registry) {
        if (ImGui::Begin("Spawn enemy")) {
            static int xPos = 0, yPos = 0;
            static float scale = 1.0, rotation = 0.0;
            static int xVelocity = 0, yVelocity = 0;
            const char* sprites[] = {"truck-image", "tank-image"};
            static int spriteSelectedIndex = 0;
            static float projectileAngle = 0.0, projectileFrequency = 1.0, projectileDuration = 5.0;
            static int projectileSpeed = 100, projectileDamage = 10;
            static int maxHealth = 100, startingHealth = 100;

            ImGui::InputInt("Spawn X", &xPos);
            ImGui::InputInt("Spawn Y", &yPos);
            ImGui::InputFloat("Scale", &scale);
            ImGui::InputFloat("Rotation", &rotation);

            ImGui::SeparatorText("Velocity");
            ImGui::InputInt("X Velocity", &xVelocity);
            ImGui::InputInt("Y Velocity", &yVelocity);

            ImGui::Separator();

            const char* combo_preview_value = sprites[spriteSelectedIndex];
            if (ImGui::BeginCombo("Sprite", combo_preview_value)) {
                for (int n = 0; n < IM_ARRAYSIZE(sprites); n++) {
                    const bool is_selected = (spriteSelectedIndex == n);
                    if (ImGui::Selectable(sprites[n], is_selected))
                        spriteSelectedIndex = n;

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            ImGui::SeparatorText("Projectile");
            ImGui::SliderFloat("Projectile Angle", &projectileAngle, 0.0f, 360.0f);
            ImGui::InputFloat("Projectile Frequency (Seconds)", &projectileFrequency);
            ImGui::InputFloat("Projectile Duration (Seconds)", &projectileDuration);
            ImGui::InputInt("Projectile Speed", &projectileSpeed);
            ImGui::InputInt("Projectile Damage", &projectileDamage);

            ImGui::SeparatorText("Health");
            ImGui::InputInt("Max Health", &maxHealth);
            ImGui::InputInt("Starting Health", &startingHealth);

            ImGui::Separator();

            if (ImGui::Button("Spawn")) {
                auto sprite = std::string(sprites[spriteSelectedIndex]);
                auto radians = glm::radians(projectileAngle);
                auto projectileVelocity = glm::vec2(glm::cos(radians), glm::sin(radians)) * (float)projectileSpeed;
                auto projectileFrequencyMs = static_cast<int>(projectileFrequency * 1000);
                auto projectileDurationMs = static_cast<int>(projectileDuration * 1000);

                auto enemy = registry->CreateEntity();
                enemy.Group("enemies");
                enemy.AddComponent<TransformComponent>(glm::vec2(xPos, yPos), glm::vec2(scale, scale), rotation);
                enemy.AddComponent<RigidBodyComponent>(glm::vec2(xVelocity, yVelocity));
                enemy.AddComponent<SpriteComponent>(sprite, 32, 32, 1);
                enemy.AddComponent<BoxColliderComponent>(32, 32);
                enemy.AddComponent<ProjectileEmitterComponent>(projectileVelocity, projectileDurationMs, projectileFrequencyMs, projectileDamage, false);
                enemy.AddComponent<HealthComponent>(maxHealth, startingHealth);
            }
        }
        ImGui::End();
    }
};