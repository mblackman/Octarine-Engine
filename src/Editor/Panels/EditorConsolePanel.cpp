#include "Editor/Panels/EditorConsolePanel.h"

#ifdef OCTARINE_WITH_EDITOR

#include <cfloat>
#include <sol/sol.hpp>
#include <string>
#include <vector>

#include "General/Logger.h"
#include "imgui.h"

namespace octarine::editor::panels {

void DrawLuaConsoleWindow(sol::state& lua) {
  static char inputBuffer[256] = "";
  static std::vector<std::string> commandHistory;
  static int commandHistoryIndex = -1;
  static bool scrollToBottom = true;
  static bool reclaimFocus = false;

  ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
  if (!ImGui::Begin("Lua Console", nullptr, ImGuiWindowFlags_NoScrollbar)) {
    ImGui::End();
    return;
  }

  if (ImGui::Button("Clear")) {
    Logger::ClearHistory();
  }
  ImGui::SameLine();
  ImGui::TextDisabled("Type Lua commands below. Use Up/Down for history.");

  const float footerHeightToReserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
  ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footerHeightToReserve), false, ImGuiWindowFlags_HorizontalScrollbar);

  Logger::ForEachHistory([](const std::string& line) {
    ImVec4 color = ImVec4(1.0F, 1.0F, 1.0F, 1.0F);
    if (line.find("[Error]") != std::string::npos) {
      color = ImVec4(1.0F, 0.4F, 0.4F, 1.0F);
    } else if (line.find("[Warn]") != std::string::npos) {
      color = ImVec4(1.0F, 1.0F, 0.4F, 1.0F);
    } else if (line.find("[Info]") != std::string::npos) {
      color = ImVec4(0.4F, 1.0F, 0.4F, 1.0F);
    } else if (line.find("> ") == 0) {
      color = ImVec4(0.4F, 0.8F, 1.0F, 1.0F);
    }
    ImGui::PushStyleColor(ImGuiCol_Text, color);
    ImGui::TextUnformatted(line.c_str());
    ImGui::PopStyleColor();
  });

  // Snap to bottom on an explicit request (open / new command) or when already pinned to the
  // bottom so fresh logs keep scrolling in. Scrolling up disengages the auto-follow.
  if (scrollToBottom || ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
    ImGui::SetScrollHereY(1.0f);
  }
  scrollToBottom = false;
  ImGui::EndChild();

  ImGui::Separator();

  struct CallbackData {
    std::vector<std::string>* history;
    int* index;
  } cbData{&commandHistory, &commandHistoryIndex};

  auto wrappedCallback = [](ImGuiInputTextCallbackData* data) -> int {
    auto* cb = static_cast<CallbackData*>(data->UserData);
    if (data->EventFlag == ImGuiInputTextFlags_CallbackHistory) {
      if (data->EventKey == ImGuiKey_UpArrow) {
        if (*cb->index == -1) {
          *cb->index = static_cast<int>(cb->history->size()) - 1;
        } else if (*cb->index > 0) {
          (*cb->index)--;
        }
      } else if (data->EventKey == ImGuiKey_DownArrow) {
        if (*cb->index != -1) {
          if (*cb->index < static_cast<int>(cb->history->size()) - 1) {
            (*cb->index)++;
          } else {
            *cb->index = -1;
          }
        }
      }
      if (*cb->index != -1) {
        data->DeleteChars(0, data->BufTextLen);
        data->InsertChars(0, (*cb->history)[static_cast<size_t>(*cb->index)].c_str());
      } else {
        data->DeleteChars(0, data->BufTextLen);
      }
    }
    return 0;
  };

  ImGuiInputTextFlags inputTextFlags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackHistory;
  ImGui::SetNextItemWidth(-FLT_MIN);
  if (ImGui::InputText("##Input", inputBuffer, IM_ARRAYSIZE(inputBuffer), inputTextFlags, wrappedCallback, &cbData)) {
    std::string command(inputBuffer);
    if (!command.empty()) {
      Logger::LogLua("> " + command);
      commandHistory.push_back(command);
      commandHistoryIndex = -1;

      auto result = lua.safe_script(command, sol::script_pass_on_error);
      if (!result.valid()) {
        const sol::error err = result;
        Logger::ErrorLua(std::string(err.what()));
      }

      inputBuffer[0] = '\0';
      scrollToBottom = true;
      reclaimFocus = true;
    }
  }

  // Only refocus the input right after submitting a command. Don't steal focus whenever the
  // window is focused — that prevented scrolling the log with the keyboard.
  if (reclaimFocus) {
    ImGui::SetKeyboardFocusHere(-1);
    reclaimFocus = false;
  }

  ImGui::End();
}

}  // namespace octarine::editor::panels

#endif  // OCTARINE_WITH_EDITOR
