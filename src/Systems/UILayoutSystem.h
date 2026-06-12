#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "Components/UIAnchorComponent.h"
#include "Components/UICanvasComponent.h"
#include "Components/UIRectComponent.h"
#include "Components/UIZIndexComponent.h"
#include "ECS/Iterable.h"
#include "ECS/Query.h"
#include "ECS/Registry.h"
#include "Game/GameConfig.h"

class UILayoutSystem {
 public:
  void operator()(const ContextFacade& ctx, const Iterable& /*iter*/) {
    auto* registry = ctx.GetRegistry();
    EnsureInitialized(registry);
    canvasQuery_->Update();

    const auto& gameConfig = registry->Get<GameConfig>();

    canvasQuery_->ForEach([&](const Entity canvasEntity, const UICanvasComponent& canvas) {
      const float cw = (canvas.width > 0.f) ? canvas.width : static_cast<float>(gameConfig.windowWidth);
      const float ch = (canvas.height > 0.f) ? canvas.height : static_cast<float>(gameConfig.windowHeight);
      const UIRectComponent canvasRect{0.f, 0.f, cw, ch, canvas.baseLayer};
      WriteRect(registry, canvasEntity, canvasRect);
      ResolveChildren(registry, canvasEntity, canvasRect);
    });
  }

 private:
  using CanvasQuery = ComponentQuery<UICanvasComponent>;

  void EnsureInitialized(Registry* registry) {
    if (canvasQuery_) return;
    canvasQuery_ = registry->CreateQuery<UICanvasComponent>();
  }

  static void WriteRect(Registry* registry, const Entity entity, const UIRectComponent& rect) {
    if (registry->HasComponent<UIRectComponent>(entity)) {
      registry->GetComponent<UIRectComponent>(entity) = rect;
    } else {
      registry->AddComponent(entity, rect);
    }
  }

  void ResolveChildren(Registry* registry, const Entity root, const UIRectComponent& rootRect) const {
    std::vector<std::pair<Entity, UIRectComponent>> pending;
    pending.emplace_back(root, rootRect);
    while (!pending.empty()) {
      auto [parent, parentRect] = pending.back();
      pending.pop_back();
      registry->ForEachChild(parent, [&](const Entity child) {
        UIRectComponent childRect = parentRect;  // inherit layer from parent
        bool modified = false;
        if (registry->HasComponent<UIAnchorComponent>(child)) {
          const auto& anchor = registry->GetComponent<UIAnchorComponent>(child);
          const float pw = parentRect.Width();
          const float ph = parentRect.Height();
          childRect.left = parentRect.left + pw * anchor.anchorLeft + anchor.offsetLeft;
          childRect.top = parentRect.top + ph * anchor.anchorTop + anchor.offsetTop;
          childRect.right = parentRect.left + pw * anchor.anchorRight + anchor.offsetRight;
          childRect.bottom = parentRect.top + ph * anchor.anchorBottom + anchor.offsetBottom;
          modified = true;
        }
        if (registry->HasComponent<UIZIndexComponent>(child)) {
          childRect.layer += registry->GetComponent<UIZIndexComponent>(child).z;
          modified = true;
        }
        if (modified) {
          WriteRect(registry, child, childRect);
        }
        pending.emplace_back(child, childRect);
      });
    }
  }

  std::unique_ptr<CanvasQuery> canvasQuery_;
};
