#include "AssetManager/SceneAssetScanner.h"

#include <stack>
#include <utility>

namespace {

// Append a string field from a component table to the refs list (with context), if present.
void AddIfString(const sol::table& table, const char* key, const std::string& context,
                 std::vector<AssetReference>& out) {
  const sol::optional<std::string> value = table[key];
  if (value && !value->empty()) out.push_back({*value, context + key});
}

}  // namespace

void SceneAssetScanner::CollectFromComponents(const sol::table& components, const std::string& entityContext,
                                              std::vector<AssetReference>& out) {
  // Field names match the component Lua bindings: sprite.texture_asset_id,
  // text_label.font_id, audio_source.clip_id.
  if (const sol::optional<sol::table> sprite = components["sprite"]; sprite) {
    AddIfString(*sprite, "texture_asset_id", entityContext + " sprite.", out);
  }
  if (const sol::optional<sol::table> text = components["text_label"]; text) {
    AddIfString(*text, "font_id", entityContext + " text_label.", out);
  }
  if (const sol::optional<sol::table> audio = components["audio_source"]; audio) {
    AddIfString(*audio, "clip_id", entityContext + " audio_source.", out);
  }
}

std::vector<AssetReference> SceneAssetScanner::CollectRefs(const sol::table& scene) {
  std::vector<AssetReference> refs;
  if (!scene.valid()) return refs;

  // Scene-level tilemap texture (referenced by the loader, not by a component).
  if (const sol::optional<sol::table> tilemap = scene["tilemap"]; tilemap) {
    AddIfString(*tilemap, "texture_asset_id", "tilemap.", refs);
  }

  // Explicit preload list: anything not in the static entity tree (runtime spawns + load-time
  // injection). Accepts a list of strings.
  if (const sol::optional<sol::table> preload = scene["preload"]; preload) {
    for (const auto& [_, value] : *preload) {
      if (value.is<std::string>()) {
        if (std::string id = value.as<std::string>(); !id.empty()) refs.push_back({std::move(id), "preload list"});
      }
    }
  }

  // Walk the entity forest. Each node may carry `components` and nested `entities`, exactly like
  // LuaEntityLoader::LoadEntityFromLua.
  std::stack<sol::table> nodes;
  if (const sol::optional<sol::table> entities = scene["entities"]; entities) {
    for (const auto& [_, value] : *entities) {
      if (value.is<sol::table>()) nodes.push(value.as<sol::table>());
    }
  }

  while (!nodes.empty()) {
    const sol::table node = nodes.top();
    nodes.pop();

    // Name the entity for diagnostics when it carries one; fall back to a generic label.
    std::string entityContext = "entity";
    if (const sol::optional<std::string> name = node["name"]; name && !name->empty()) {
      entityContext = "entity '" + *name + "'";
    }

    if (const sol::optional<sol::table> components = node["components"]; components) {
      CollectFromComponents(*components, entityContext, refs);
    }
    if (const sol::optional<sol::table> children = node["entities"]; children) {
      for (const auto& [_, value] : *children) {
        if (value.is<sol::table>()) nodes.push(value.as<sol::table>());
      }
    }
  }

  return refs;
}

std::unordered_set<std::string> SceneAssetScanner::Collect(const sol::table& scene) {
  std::unordered_set<std::string> ids;
  for (auto& ref : CollectRefs(scene)) ids.insert(std::move(ref.id));
  return ids;
}
