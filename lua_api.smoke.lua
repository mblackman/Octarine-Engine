-- AUTO-GENERATED Octarine Lua API surface. Do not edit by hand.
-- Introspected from the live sol::state after all bindings install.
-- Refreshed automatically on every editor-mode launch (types/octarine_api.lua);
-- OCTARINE_DUMP_LUA_API=<path> overrides the destination.
---@meta
---@diagnostic disable: lowercase-global, missing-return

---@class ImDrawFlags
---@field Closed any
---@field None any
---@field RoundCornersAll any
---@field RoundCornersBottom any
---@field RoundCornersBottomLeft any
---@field RoundCornersBottomRight any
---@field RoundCornersLeft any
---@field RoundCornersNone any
---@field RoundCornersRight any
---@field RoundCornersTop any
---@field RoundCornersTopLeft any
---@field RoundCornersTopRight any
ImDrawFlags = {}


---@class ImDrawListFlags
---@field AllowVtxOffset any
---@field AntiAliasedFill any
---@field AntiAliasedLines any
---@field AntiAliasedLinesUseTex any
---@field None any
ImDrawListFlags = {}


---@class ImFontAtlasFlags
---@field NoBakedLines any
---@field NoMouseCursors any
---@field NoPowerOfTwoHeight any
---@field None any
ImFontAtlasFlags = {}


---@class ImFontFlags
---@field LockBakedSizes any
---@field NoLoadError any
---@field NoLoadGlyphs any
---@field None any
ImFontFlags = {}


---@class ImGui
ImGui = {}

function ImGui.AlignTextToFramePadding(...) end

function ImGui.ArrowButton(...) end

function ImGui.Begin(...) end

function ImGui.BeginChild(...) end

function ImGui.BeginCombo(...) end

function ImGui.BeginDisabled(...) end

function ImGui.BeginDragDropSource(...) end

function ImGui.BeginDragDropTarget(...) end

function ImGui.BeginGroup(...) end

function ImGui.BeginItemTooltip(...) end

function ImGui.BeginListBox(...) end

function ImGui.BeginMainMenuBar(...) end

function ImGui.BeginMenu(...) end

function ImGui.BeginMenuBar(...) end

function ImGui.BeginPopup(...) end

function ImGui.BeginPopupContextItem(...) end

function ImGui.BeginPopupContextVoid(...) end

function ImGui.BeginPopupContextWindow(...) end

function ImGui.BeginPopupModal(...) end

function ImGui.BeginTabBar(...) end

function ImGui.BeginTabItem(...) end

function ImGui.BeginTable(...) end

function ImGui.BeginTooltip(...) end

function ImGui.Bullet(...) end

function ImGui.BulletText(...) end

function ImGui.Button(...) end

function ImGui.CalcItemWidth(...) end

function ImGui.CalcTextSize(...) end

function ImGui.Checkbox(...) end

function ImGui.CheckboxFlags(...) end

function ImGui.CloseCurrentPopup(...) end

function ImGui.CollapsingHeader(...) end

function ImGui.ColorButton(...) end

function ImGui.ColorConvertFloat4ToU32(...) end

function ImGui.ColorConvertHSVtoRGB(...) end

function ImGui.ColorConvertRGBtoHSV(...) end

function ImGui.ColorConvertU32ToFloat4(...) end

function ImGui.ColorEdit3(...) end

function ImGui.ColorEdit4(...) end

function ImGui.ColorPicker3(...) end

function ImGui.ColorPicker4(...) end

function ImGui.Columns(...) end

function ImGui.Combo(...) end

function ImGui.DebugCheckVersionAndDataLayout(...) end

function ImGui.DebugFlashStyleColor(...) end

function ImGui.DebugStartItemPicker(...) end

function ImGui.DebugTextEncoding(...) end

function ImGui.DestroyPlatformWindows(...) end

function ImGui.DockSpace(...) end

function ImGui.DragFloat(...) end

function ImGui.DragFloat2(...) end

function ImGui.DragFloat3(...) end

function ImGui.DragFloat4(...) end

function ImGui.DragFloatRange2(...) end

function ImGui.DragInt(...) end

function ImGui.DragInt2(...) end

function ImGui.DragInt3(...) end

function ImGui.DragInt4(...) end

function ImGui.DragIntRange2(...) end

function ImGui.Dummy(...) end

function ImGui.End(...) end

function ImGui.EndChild(...) end

function ImGui.EndCombo(...) end

function ImGui.EndDisabled(...) end

function ImGui.EndDragDropSource(...) end

function ImGui.EndDragDropTarget(...) end

function ImGui.EndFrame(...) end

function ImGui.EndGroup(...) end

function ImGui.EndListBox(...) end

function ImGui.EndMainMenuBar(...) end

function ImGui.EndMenu(...) end

function ImGui.EndMenuBar(...) end

function ImGui.EndPopup(...) end

function ImGui.EndTabBar(...) end

function ImGui.EndTabItem(...) end

function ImGui.EndTable(...) end

function ImGui.EndTooltip(...) end

function ImGui.GetClipboardText(...) end

function ImGui.GetColorU32(...) end

function ImGui.GetColumnIndex(...) end

function ImGui.GetColumnOffset(...) end

function ImGui.GetColumnWidth(...) end

function ImGui.GetColumnsCount(...) end

function ImGui.GetContentRegionAvail(...) end

function ImGui.GetContentRegionMax(...) end

function ImGui.GetCursorPos(...) end

function ImGui.GetCursorPosX(...) end

function ImGui.GetCursorPosY(...) end

function ImGui.GetCursorScreenPos(...) end

function ImGui.GetCursorStartPos(...) end

function ImGui.GetFontSize(...) end

function ImGui.GetFontTexUvWhitePixel(...) end

function ImGui.GetFrameCount(...) end

function ImGui.GetFrameHeight(...) end

function ImGui.GetFrameHeightWithSpacing(...) end

function ImGui.GetID(...) end

function ImGui.GetItemFlags(...) end

function ImGui.GetItemID(...) end

function ImGui.GetItemRectMax(...) end

function ImGui.GetItemRectMin(...) end

function ImGui.GetItemRectSize(...) end

function ImGui.GetKeyName(...) end

function ImGui.GetKeyPressedAmount(...) end

function ImGui.GetMouseClickedCount(...) end

function ImGui.GetMouseCursor(...) end

function ImGui.GetMouseDragDelta(...) end

function ImGui.GetMousePos(...) end

function ImGui.GetMousePosOnOpeningCurrentPopup(...) end

function ImGui.GetScrollMaxX(...) end

function ImGui.GetScrollMaxY(...) end

function ImGui.GetScrollX(...) end

function ImGui.GetScrollY(...) end

function ImGui.GetStyleColorName(...) end

function ImGui.GetStyleColorVec4(...) end

function ImGui.GetTextLineHeight(...) end

function ImGui.GetTextLineHeightWithSpacing(...) end

function ImGui.GetTime(...) end

function ImGui.GetTreeNodeToLabelSpacing(...) end

function ImGui.GetVersion(...) end

function ImGui.GetWindowContentRegionMax(...) end

function ImGui.GetWindowContentRegionMin(...) end

function ImGui.GetWindowDockID(...) end

function ImGui.GetWindowDpiScale(...) end

function ImGui.GetWindowHeight(...) end

function ImGui.GetWindowPos(...) end

function ImGui.GetWindowSize(...) end

function ImGui.GetWindowWidth(...) end

function ImGui.Indent(...) end

function ImGui.InputDouble(...) end

function ImGui.InputFloat(...) end

function ImGui.InputFloat2(...) end

function ImGui.InputFloat3(...) end

function ImGui.InputFloat4(...) end

function ImGui.InputInt(...) end

function ImGui.InputInt2(...) end

function ImGui.InputInt3(...) end

function ImGui.InputInt4(...) end

function ImGui.InputText(...) end

function ImGui.InputTextMultiline(...) end

function ImGui.InputTextWithHint(...) end

function ImGui.InvisibleButton(...) end

function ImGui.IsAnyItemActive(...) end

function ImGui.IsAnyItemFocused(...) end

function ImGui.IsAnyItemHovered(...) end

function ImGui.IsAnyMouseDown(...) end

function ImGui.IsItemActivated(...) end

function ImGui.IsItemActive(...) end

function ImGui.IsItemClicked(...) end

function ImGui.IsItemDeactivated(...) end

function ImGui.IsItemDeactivatedAfterEdit(...) end

function ImGui.IsItemEdited(...) end

function ImGui.IsItemFocused(...) end

function ImGui.IsItemHovered(...) end

function ImGui.IsItemToggledOpen(...) end

function ImGui.IsItemToggledSelection(...) end

function ImGui.IsItemVisible(...) end

function ImGui.IsKeyChordPressed(...) end

function ImGui.IsKeyDown(...) end

function ImGui.IsKeyPressed(...) end

function ImGui.IsKeyReleased(...) end

function ImGui.IsMouseClicked(...) end

function ImGui.IsMouseDoubleClicked(...) end

function ImGui.IsMouseDown(...) end

function ImGui.IsMouseDragging(...) end

function ImGui.IsMouseHoveringRect(...) end

function ImGui.IsMousePosValid(...) end

function ImGui.IsMouseReleased(...) end

function ImGui.IsMouseReleasedWithDelay(...) end

function ImGui.IsPopupOpen(...) end

function ImGui.IsRectVisible(...) end

function ImGui.IsWindowAppearing(...) end

function ImGui.IsWindowCollapsed(...) end

function ImGui.IsWindowDocked(...) end

function ImGui.IsWindowFocused(...) end

function ImGui.IsWindowHovered(...) end

function ImGui.LabelText(...) end

function ImGui.ListBox(...) end

function ImGui.LoadIniSettingsFromDisk(...) end

function ImGui.LoadIniSettingsFromMemory(...) end

function ImGui.LogButtons(...) end

function ImGui.LogFinish(...) end

function ImGui.LogText(...) end

function ImGui.LogToClipboard(...) end

function ImGui.LogToFile(...) end

function ImGui.LogToTTY(...) end

function ImGui.MenuItem(...) end

function ImGui.NewFrame(...) end

function ImGui.NewLine(...) end

function ImGui.NextColumn(...) end

function ImGui.OpenPopup(...) end

function ImGui.OpenPopupOnItemClick(...) end

function ImGui.PopButtonRepeat(...) end

function ImGui.PopClipRect(...) end

function ImGui.PopFont(...) end

function ImGui.PopID(...) end

function ImGui.PopItemFlag(...) end

function ImGui.PopItemWidth(...) end

function ImGui.PopStyleColor(...) end

function ImGui.PopStyleVar(...) end

function ImGui.PopTabStop(...) end

function ImGui.PopTextWrapPos(...) end

function ImGui.ProgressBar(...) end

function ImGui.PushButtonRepeat(...) end

function ImGui.PushClipRect(...) end

function ImGui.PushID(...) end

function ImGui.PushItemFlag(...) end

function ImGui.PushItemWidth(...) end

function ImGui.PushStyleColor(...) end

function ImGui.PushStyleVar(...) end

function ImGui.PushStyleVarX(...) end

function ImGui.PushStyleVarY(...) end

function ImGui.PushTabStop(...) end

function ImGui.PushTextWrapPos(...) end

function ImGui.RadioButton(...) end

function ImGui.Render(...) end

function ImGui.ResetMouseDragDelta(...) end

function ImGui.SameLine(...) end

function ImGui.SaveIniSettingsToDisk(...) end

function ImGui.Selectable(...) end

function ImGui.Separator(...) end

function ImGui.SeparatorText(...) end

function ImGui.SetClipboardText(...) end

function ImGui.SetColorEditOptions(...) end

function ImGui.SetColumnOffset(...) end

function ImGui.SetColumnWidth(...) end

function ImGui.SetCursorPos(...) end

function ImGui.SetCursorPosX(...) end

function ImGui.SetCursorPosY(...) end

function ImGui.SetCursorScreenPos(...) end

function ImGui.SetItemDefaultFocus(...) end

function ImGui.SetItemKeyOwner(...) end

function ImGui.SetItemTooltip(...) end

function ImGui.SetKeyboardFocusHere(...) end

function ImGui.SetMouseCursor(...) end

function ImGui.SetNavCursorVisible(...) end

function ImGui.SetNextFrameWantCaptureKeyboard(...) end

function ImGui.SetNextFrameWantCaptureMouse(...) end

function ImGui.SetNextItemAllowOverlap(...) end

function ImGui.SetNextItemOpen(...) end

function ImGui.SetNextItemShortcut(...) end

function ImGui.SetNextItemStorageID(...) end

function ImGui.SetNextItemWidth(...) end

function ImGui.SetNextWindowBgAlpha(...) end

function ImGui.SetNextWindowCollapsed(...) end

function ImGui.SetNextWindowContentSize(...) end

function ImGui.SetNextWindowDockID(...) end

function ImGui.SetNextWindowFocus(...) end

function ImGui.SetNextWindowPos(...) end

function ImGui.SetNextWindowScroll(...) end

function ImGui.SetNextWindowSize(...) end

function ImGui.SetNextWindowSizeConstraints(...) end

function ImGui.SetNextWindowViewport(...) end

function ImGui.SetScrollFromPosX(...) end

function ImGui.SetScrollFromPosY(...) end

function ImGui.SetScrollHereX(...) end

function ImGui.SetScrollHereY(...) end

function ImGui.SetScrollX(...) end

function ImGui.SetScrollY(...) end

function ImGui.SetTabItemClosed(...) end

function ImGui.SetTooltip(...) end

function ImGui.SetWindowCollapsed(...) end

function ImGui.SetWindowFocus(...) end

function ImGui.SetWindowFontScale(...) end

function ImGui.SetWindowPos(...) end

function ImGui.SetWindowSize(...) end

function ImGui.Shortcut(...) end

function ImGui.ShowFontSelector(...) end

function ImGui.ShowStyleSelector(...) end

function ImGui.ShowUserGuide(...) end

function ImGui.SliderAngle(...) end

function ImGui.SliderFloat(...) end

function ImGui.SliderFloat2(...) end

function ImGui.SliderFloat3(...) end

function ImGui.SliderFloat4(...) end

function ImGui.SliderInt(...) end

function ImGui.SliderInt2(...) end

function ImGui.SliderInt3(...) end

function ImGui.SliderInt4(...) end

function ImGui.SmallButton(...) end

function ImGui.Spacing(...) end

function ImGui.TabItemButton(...) end

function ImGui.TableAngledHeadersRow(...) end

function ImGui.TableGetColumnCount(...) end

function ImGui.TableGetColumnFlags(...) end

function ImGui.TableGetColumnIndex(...) end

function ImGui.TableGetColumnName(...) end

function ImGui.TableGetHoveredColumn(...) end

function ImGui.TableGetRowIndex(...) end

function ImGui.TableHeader(...) end

function ImGui.TableHeadersRow(...) end

function ImGui.TableNextColumn(...) end

function ImGui.TableNextRow(...) end

function ImGui.TableSetBgColor(...) end

function ImGui.TableSetColumnEnabled(...) end

function ImGui.TableSetColumnIndex(...) end

function ImGui.TableSetupColumn(...) end

function ImGui.TableSetupScrollFreeze(...) end

function ImGui.Text(...) end

function ImGui.TextColored(...) end

function ImGui.TextDisabled(...) end

function ImGui.TextLink(...) end

function ImGui.TextLinkOpenURL(...) end

function ImGui.TextUnformatted(...) end

function ImGui.TextWrapped(...) end

function ImGui.TreeNode(...) end

function ImGui.TreeNodeEx(...) end

function ImGui.TreeNodeGetOpen(...) end

function ImGui.TreePop(...) end

function ImGui.TreePush(...) end

function ImGui.Unindent(...) end

function ImGui.UpdatePlatformWindows(...) end

function ImGui.VSliderFloat(...) end

function ImGui.VSliderInt(...) end


---@class ImGuiBackendFlags
---@field HasGamepad any
---@field HasMouseCursors any
---@field HasMouseHoveredViewport any
---@field HasParentViewport any
---@field HasSetMousePos any
---@field None any
---@field PlatformHasViewports any
---@field RendererHasTextures any
---@field RendererHasViewports any
---@field RendererHasVtxOffset any
ImGuiBackendFlags = {}


---@class ImGuiButtonFlags
---@field AllowOverlap any
---@field EnableNav any
---@field MouseButtonLeft any
---@field MouseButtonMiddle any
---@field MouseButtonRight any
---@field None any
ImGuiButtonFlags = {}


---@class ImGuiChildFlags
---@field AlwaysAutoResize any
---@field AlwaysUseWindowPadding any
---@field AutoResizeX any
---@field AutoResizeY any
---@field Borders any
---@field FrameStyle any
---@field NavFlattened any
---@field None any
---@field ResizeX any
---@field ResizeY any
ImGuiChildFlags = {}


---@class ImGuiCol
---@field Border any
---@field BorderShadow any
---@field Button any
---@field ButtonActive any
---@field ButtonHovered any
---@field COUNT any
---@field CheckMark any
---@field ChildBg any
---@field DockingEmptyBg any
---@field DockingPreview any
---@field DragDropTarget any
---@field DragDropTargetBg any
---@field FrameBg any
---@field FrameBgActive any
---@field FrameBgHovered any
---@field Header any
---@field HeaderActive any
---@field HeaderHovered any
---@field InputTextCursor any
---@field MenuBarBg any
---@field ModalWindowDimBg any
---@field NavCursor any
---@field NavHighlight any
---@field NavWindowingDimBg any
---@field NavWindowingHighlight any
---@field PlotHistogram any
---@field PlotHistogramHovered any
---@field PlotLines any
---@field PlotLinesHovered any
---@field PopupBg any
---@field ResizeGrip any
---@field ResizeGripActive any
---@field ResizeGripHovered any
---@field ScrollbarBg any
---@field ScrollbarGrab any
---@field ScrollbarGrabActive any
---@field ScrollbarGrabHovered any
---@field Separator any
---@field SeparatorActive any
---@field SeparatorHovered any
---@field SliderGrab any
---@field SliderGrabActive any
---@field Tab any
---@field TabActive any
---@field TabDimmed any
---@field TabDimmedSelected any
---@field TabDimmedSelectedOverline any
---@field TabHovered any
---@field TabSelected any
---@field TabSelectedOverline any
---@field TabUnfocused any
---@field TabUnfocusedActive any
---@field TableBorderLight any
---@field TableBorderStrong any
---@field TableHeaderBg any
---@field TableRowBg any
---@field TableRowBgAlt any
---@field Text any
---@field TextDisabled any
---@field TextLink any
---@field TextSelectedBg any
---@field TitleBg any
---@field TitleBgActive any
---@field TitleBgCollapsed any
---@field TreeLines any
---@field UnsavedMarker any
---@field WindowBg any
ImGuiCol = {}


---@class ImGuiColorEditFlags
---@field AlphaBar any
---@field AlphaNoBg any
---@field AlphaOpaque any
---@field AlphaPreview any
---@field AlphaPreviewHalf any
---@field DisplayHSV any
---@field DisplayHex any
---@field DisplayRGB any
---@field Float any
---@field HDR any
---@field InputHSV any
---@field InputRGB any
---@field NoAlpha any
---@field NoBorder any
---@field NoColorMarkers any
---@field NoDragDrop any
---@field NoInputs any
---@field NoLabel any
---@field NoOptions any
---@field NoPicker any
---@field NoSidePreview any
---@field NoSmallPreview any
---@field NoTooltip any
---@field None any
---@field PickerHueBar any
---@field PickerHueWheel any
---@field Uint8 any
ImGuiColorEditFlags = {}


---@class ImGuiComboFlags
---@field HeightLarge any
---@field HeightLargest any
---@field HeightRegular any
---@field HeightSmall any
---@field NoArrowButton any
---@field NoPreview any
---@field None any
---@field PopupAlignLeft any
---@field WidthFitPreview any
ImGuiComboFlags = {}


---@class ImGuiCond
---@field Always any
---@field Appearing any
---@field FirstUseEver any
---@field None any
---@field Once any
ImGuiCond = {}


---@class ImGuiConfigFlags
---@field DockingEnable any
---@field DpiEnableScaleFonts any
---@field DpiEnableScaleViewports any
---@field IsSRGB any
---@field IsTouchScreen any
---@field NavEnableGamepad any
---@field NavEnableKeyboard any
---@field NavEnableSetMousePos any
---@field NavNoCaptureKeyboard any
---@field NoKeyboard any
---@field NoMouse any
---@field NoMouseCursorChange any
---@field None any
---@field ViewportsEnable any
ImGuiConfigFlags = {}


---@class ImGuiDataType
---@field Bool any
---@field COUNT any
---@field Double any
---@field Float any
---@field S16 any
---@field S32 any
---@field S64 any
---@field S8 any
---@field String any
---@field U16 any
---@field U32 any
---@field U64 any
---@field U8 any
ImGuiDataType = {}


---@class ImGuiDir
---@field _COUNT any
---@field _Down any
---@field _Left any
---@field _None any
---@field _Right any
---@field _Up any
ImGuiDir = {}


---@class ImGuiDockNodeFlags
---@field AutoHideTabBar any
---@field KeepAliveOnly any
---@field NoDockingInCentralNode any
---@field NoDockingOverCentralNode any
---@field NoDockingSplit any
---@field NoResize any
---@field NoSplit any
---@field NoUndocking any
---@field None any
---@field PassthruCentralNode any
ImGuiDockNodeFlags = {}


---@class ImGuiDragDropFlags
---@field AcceptBeforeDelivery any
---@field AcceptDrawAsHovered any
---@field AcceptNoDrawDefaultRect any
---@field AcceptNoPreviewTooltip any
---@field AcceptPeekOnly any
---@field None any
---@field PayloadAutoExpire any
---@field PayloadNoCrossContext any
---@field PayloadNoCrossProcess any
---@field SourceAllowNullID any
---@field SourceAutoExpirePayload any
---@field SourceExtern any
---@field SourceNoDisableHover any
---@field SourceNoHoldToOpenOthers any
---@field SourceNoPreviewTooltip any
ImGuiDragDropFlags = {}


---@class ImGuiFocusedFlags
---@field AnyWindow any
---@field ChildWindows any
---@field DockHierarchy any
---@field NoPopupHierarchy any
---@field None any
---@field RootAndChildWindows any
---@field RootWindow any
ImGuiFocusedFlags = {}


---@class ImGuiHoveredFlags
---@field AllowWhenBlockedByActiveItem any
---@field AllowWhenBlockedByPopup any
---@field AllowWhenDisabled any
---@field AllowWhenOverlapped any
---@field AllowWhenOverlappedByItem any
---@field AllowWhenOverlappedByWindow any
---@field AnyWindow any
---@field ChildWindows any
---@field DelayNone any
---@field DelayNormal any
---@field DelayShort any
---@field DockHierarchy any
---@field ForTooltip any
---@field NoNavOverride any
---@field NoPopupHierarchy any
---@field NoSharedDelay any
---@field None any
---@field RectOnly any
---@field RootAndChildWindows any
---@field RootWindow any
---@field Stationary any
ImGuiHoveredFlags = {}


---@class ImGuiInputFlags
---@field None any
---@field Repeat any
---@field RouteActive any
---@field RouteAlways any
---@field RouteFocused any
---@field RouteFromRootWindow any
---@field RouteGlobal any
---@field RouteOverActive any
---@field RouteOverFocused any
---@field RouteUnlessBgFocused any
---@field Tooltip any
ImGuiInputFlags = {}


---@class ImGuiInputTextFlags
---@field AllowTabInput any
---@field AlwaysOverwrite any
---@field AutoSelectAll any
---@field CallbackAlways any
---@field CallbackCharFilter any
---@field CallbackCompletion any
---@field CallbackEdit any
---@field CallbackHistory any
---@field CallbackResize any
---@field CharsDecimal any
---@field CharsHexadecimal any
---@field CharsNoBlank any
---@field CharsScientific any
---@field CharsUppercase any
---@field CtrlEnterForNewLine any
---@field DisplayEmptyRefVal any
---@field ElideLeft any
---@field EnterReturnsTrue any
---@field EscapeClearsAll any
---@field NoHorizontalScroll any
---@field NoUndoRedo any
---@field None any
---@field ParseEmptyRefVal any
---@field Password any
---@field ReadOnly any
---@field WordWrap any
ImGuiInputTextFlags = {}


---@class ImGuiItemFlags
---@field AllowDuplicateId any
---@field AutoClosePopups any
---@field ButtonRepeat any
---@field Disabled any
---@field NoNav any
---@field NoNavDefaultFocus any
---@field NoTabStop any
---@field None any
ImGuiItemFlags = {}


---@class ImGuiKey
---@field ImGuiMod_Alt any
---@field ImGuiMod_Ctrl any
---@field ImGuiMod_None any
---@field ImGuiMod_Shift any
---@field ImGuiMod_Shortcut any
---@field ImGuiMod_Super any
---@field _0 any
---@field _1 any
---@field _2 any
---@field _3 any
---@field _4 any
---@field _5 any
---@field _6 any
---@field _7 any
---@field _8 any
---@field _9 any
---@field _A any
---@field _Apostrophe any
---@field _AppBack any
---@field _AppForward any
---@field _B any
---@field _Backslash any
---@field _Backspace any
---@field _C any
---@field _COUNT any
---@field _CapsLock any
---@field _Comma any
---@field _D any
---@field _Delete any
---@field _DownArrow any
---@field _E any
---@field _End any
---@field _Enter any
---@field _Equal any
---@field _Escape any
---@field _F any
---@field _F1 any
---@field _F10 any
---@field _F11 any
---@field _F12 any
---@field _F13 any
---@field _F14 any
---@field _F15 any
---@field _F16 any
---@field _F17 any
---@field _F18 any
---@field _F19 any
---@field _F2 any
---@field _F20 any
---@field _F21 any
---@field _F22 any
---@field _F23 any
---@field _F24 any
---@field _F3 any
---@field _F4 any
---@field _F5 any
---@field _F6 any
---@field _F7 any
---@field _F8 any
---@field _F9 any
---@field _G any
---@field _GamepadBack any
---@field _GamepadDpadDown any
---@field _GamepadDpadLeft any
---@field _GamepadDpadRight any
---@field _GamepadDpadUp any
---@field _GamepadFaceDown any
---@field _GamepadFaceLeft any
---@field _GamepadFaceRight any
---@field _GamepadFaceUp any
---@field _GamepadL1 any
---@field _GamepadL2 any
---@field _GamepadL3 any
---@field _GamepadLStickDown any
---@field _GamepadLStickLeft any
---@field _GamepadLStickRight any
---@field _GamepadLStickUp any
---@field _GamepadR1 any
---@field _GamepadR2 any
---@field _GamepadR3 any
---@field _GamepadRStickDown any
---@field _GamepadRStickLeft any
---@field _GamepadRStickRight any
---@field _GamepadRStickUp any
---@field _GamepadStart any
---@field _GraveAccent any
---@field _H any
---@field _Home any
---@field _I any
---@field _Insert any
---@field _J any
---@field _K any
---@field _Keypad0 any
---@field _Keypad1 any
---@field _Keypad2 any
---@field _Keypad3 any
---@field _Keypad4 any
---@field _Keypad5 any
---@field _Keypad6 any
---@field _Keypad7 any
---@field _Keypad8 any
---@field _Keypad9 any
---@field _KeypadAdd any
---@field _KeypadDecimal any
---@field _KeypadDivide any
---@field _KeypadEnter any
---@field _KeypadEqual any
---@field _KeypadMultiply any
---@field _KeypadSubtract any
---@field _L any
---@field _LeftAlt any
---@field _LeftArrow any
---@field _LeftBracket any
---@field _LeftCtrl any
---@field _LeftShift any
---@field _LeftSuper any
---@field _M any
---@field _Menu any
---@field _Minus any
---@field _MouseLeft any
---@field _MouseMiddle any
---@field _MouseRight any
---@field _MouseWheelX any
---@field _MouseWheelY any
---@field _MouseX1 any
---@field _MouseX2 any
---@field _N any
---@field _NamedKey_BEGIN any
---@field _None any
---@field _NumLock any
---@field _O any
---@field _Oem102 any
---@field _P any
---@field _PageDown any
---@field _PageUp any
---@field _Pause any
---@field _Period any
---@field _PrintScreen any
---@field _Q any
---@field _R any
---@field _RightAlt any
---@field _RightArrow any
---@field _RightBracket any
---@field _RightCtrl any
---@field _RightShift any
---@field _RightSuper any
---@field _S any
---@field _ScrollLock any
---@field _Semicolon any
---@field _Slash any
---@field _Space any
---@field _T any
---@field _Tab any
---@field _U any
---@field _UpArrow any
---@field _V any
---@field _W any
---@field _X any
---@field _Y any
---@field _Z any
ImGuiKey = {}


---@class ImGuiListClipperFlags
---@field NoSetTableRowCounters any
---@field None any
ImGuiListClipperFlags = {}


---@class ImGuiMouseButton
---@field COUNT any
---@field Left any
---@field Middle any
---@field Right any
ImGuiMouseButton = {}


---@class ImGuiMouseCursor
---@field Arrow any
---@field COUNT any
---@field Hand any
---@field None any
---@field NotAllowed any
---@field Progress any
---@field ResizeAll any
---@field ResizeEW any
---@field ResizeNESW any
---@field ResizeNS any
---@field ResizeNWSE any
---@field TextInput any
---@field Wait any
ImGuiMouseCursor = {}


---@class ImGuiMouseSource
---@field _COUNT any
---@field _Mouse any
---@field _Pen any
---@field _TouchScreen any
ImGuiMouseSource = {}


---@class ImGuiMultiSelectFlags
---@field BoxSelect1d any
---@field BoxSelect2d any
---@field BoxSelectNoScroll any
---@field ClearOnClickVoid any
---@field ClearOnEscape any
---@field NavWrapX any
---@field NoAutoClear any
---@field NoAutoClearOnReselect any
---@field NoAutoSelect any
---@field NoRangeSelect any
---@field NoSelectAll any
---@field NoSelectOnRightClick any
---@field None any
---@field ScopeRect any
---@field ScopeWindow any
---@field SelectOnAuto any
---@field SelectOnClick any
---@field SelectOnClickAlways any
---@field SelectOnClickRelease any
---@field SingleSelect any
ImGuiMultiSelectFlags = {}


---@class ImGuiPopupFlags
---@field AnyPopup any
---@field AnyPopupId any
---@field AnyPopupLevel any
---@field MouseButtonLeft any
---@field MouseButtonMiddle any
---@field MouseButtonRight any
---@field NoOpenOverExistingPopup any
---@field NoOpenOverItems any
---@field NoReopen any
---@field None any
ImGuiPopupFlags = {}


---@class ImGuiSelectableFlags
---@field AllowDoubleClick any
---@field AllowOverlap any
---@field Disabled any
---@field DontClosePopups any
---@field Highlight any
---@field NoAutoClosePopups any
---@field None any
---@field SelectOnNav any
---@field SpanAllColumns any
ImGuiSelectableFlags = {}


---@class ImGuiSelectionRequestType
---@field _None any
---@field _SetAll any
---@field _SetRange any
ImGuiSelectionRequestType = {}


---@class ImGuiSliderFlags
---@field AlwaysClamp any
---@field ClampOnInput any
---@field ClampZeroRange any
---@field ColorMarkers any
---@field Logarithmic any
---@field NoInput any
---@field NoRoundToFormat any
---@field NoSpeedTweaks any
---@field None any
---@field WrapAround any
ImGuiSliderFlags = {}


---@class ImGuiSortDirection
---@field _Ascending any
---@field _Descending any
---@field _None any
ImGuiSortDirection = {}


---@class ImGuiStyleVar
---@field Alpha any
---@field ButtonTextAlign any
---@field COUNT any
---@field CellPadding any
---@field ChildBorderSize any
---@field ChildRounding any
---@field DisabledAlpha any
---@field DockingSeparatorSize any
---@field FrameBorderSize any
---@field FramePadding any
---@field FrameRounding any
---@field GrabMinSize any
---@field GrabRounding any
---@field ImageBorderSize any
---@field ImageRounding any
---@field IndentSpacing any
---@field ItemInnerSpacing any
---@field ItemSpacing any
---@field PopupBorderSize any
---@field PopupRounding any
---@field ScrollbarPadding any
---@field ScrollbarRounding any
---@field ScrollbarSize any
---@field SelectableTextAlign any
---@field SeparatorSize any
---@field SeparatorTextAlign any
---@field SeparatorTextBorderSize any
---@field SeparatorTextPadding any
---@field TabBarBorderSize any
---@field TabBarOverlineSize any
---@field TabBorderSize any
---@field TabMinWidthBase any
---@field TabMinWidthShrink any
---@field TabRounding any
---@field TableAngledHeadersAngle any
---@field TableAngledHeadersTextAlign any
---@field TreeLinesRounding any
---@field TreeLinesSize any
---@field WindowBorderSize any
---@field WindowMinSize any
---@field WindowPadding any
---@field WindowRounding any
---@field WindowTitleAlign any
ImGuiStyleVar = {}


---@class ImGuiTabBarFlags
---@field AutoSelectNewTabs any
---@field DrawSelectedOverline any
---@field FittingPolicyMixed any
---@field FittingPolicyResizeDown any
---@field FittingPolicyScroll any
---@field FittingPolicyShrink any
---@field NoCloseWithMiddleMouseButton any
---@field NoTabListScrollingButtons any
---@field NoTooltip any
---@field None any
---@field Reorderable any
---@field TabListPopupButton any
ImGuiTabBarFlags = {}


---@class ImGuiTabItemFlags
---@field Leading any
---@field NoAssumedClosure any
---@field NoCloseWithMiddleMouseButton any
---@field NoPushId any
---@field NoReorder any
---@field NoTooltip any
---@field None any
---@field SetSelected any
---@field Trailing any
---@field UnsavedDocument any
ImGuiTabItemFlags = {}


---@class ImGuiTableBgTarget
---@field CellBg any
---@field None any
---@field RowBg0 any
---@field RowBg1 any
ImGuiTableBgTarget = {}


---@class ImGuiTableColumnFlags
---@field AngledHeader any
---@field DefaultHide any
---@field DefaultSort any
---@field Disabled any
---@field IndentDisable any
---@field IndentEnable any
---@field IsEnabled any
---@field IsHovered any
---@field IsSorted any
---@field IsVisible any
---@field NoClip any
---@field NoHeaderLabel any
---@field NoHeaderWidth any
---@field NoHide any
---@field NoReorder any
---@field NoResize any
---@field NoSort any
---@field NoSortAscending any
---@field NoSortDescending any
---@field None any
---@field PreferSortAscending any
---@field PreferSortDescending any
---@field WidthFixed any
---@field WidthStretch any
ImGuiTableColumnFlags = {}


---@class ImGuiTableFlags
---@field Borders any
---@field BordersH any
---@field BordersInner any
---@field BordersInnerH any
---@field BordersInnerV any
---@field BordersOuter any
---@field BordersOuterH any
---@field BordersOuterV any
---@field BordersV any
---@field ContextMenuInBody any
---@field Hideable any
---@field HighlightHoveredColumn any
---@field NoBordersInBody any
---@field NoBordersInBodyUntilResize any
---@field NoClip any
---@field NoHostExtendX any
---@field NoHostExtendY any
---@field NoKeepColumnsVisible any
---@field NoPadInnerX any
---@field NoPadOuterX any
---@field NoSavedSettings any
---@field None any
---@field PadOuterX any
---@field PreciseWidths any
---@field Reorderable any
---@field Resizable any
---@field RowBg any
---@field ScrollX any
---@field ScrollY any
---@field SizingFixedFit any
---@field SizingFixedSame any
---@field SizingStretchProp any
---@field SizingStretchSame any
---@field SortMulti any
---@field SortTristate any
---@field Sortable any
ImGuiTableFlags = {}


---@class ImGuiTableRowFlags
---@field Headers any
---@field None any
ImGuiTableRowFlags = {}


---@class ImGuiTreeNodeFlags
---@field AllowOverlap any
---@field Bullet any
---@field CollapsingHeader any
---@field DefaultOpen any
---@field DrawLinesFull any
---@field DrawLinesNone any
---@field DrawLinesToNodes any
---@field FramePadding any
---@field Framed any
---@field LabelSpanAllColumns any
---@field Leaf any
---@field NavLeftJumpsBackHere any
---@field NavLeftJumpsToParent any
---@field NoAutoOpenOnLog any
---@field NoTreePushOnOpen any
---@field None any
---@field OpenOnArrow any
---@field OpenOnDoubleClick any
---@field Selected any
---@field SpanAllColumns any
---@field SpanAvailWidth any
---@field SpanFullWidth any
---@field SpanLabelWidth any
---@field SpanTextWidth any
ImGuiTreeNodeFlags = {}


---@class ImGuiViewportFlags
---@field CanHostOtherWindows any
---@field IsFocused any
---@field IsMinimized any
---@field IsPlatformMonitor any
---@field IsPlatformWindow any
---@field NoAutoMerge any
---@field NoDecoration any
---@field NoFocusOnAppearing any
---@field NoFocusOnClick any
---@field NoInputs any
---@field NoRendererClear any
---@field NoTaskBarIcon any
---@field None any
---@field OwnedByApp any
---@field TopMost any
ImGuiViewportFlags = {}


---@class ImGuiWindowFlags
---@field AlwaysAutoResize any
---@field AlwaysHorizontalScrollbar any
---@field AlwaysVerticalScrollbar any
---@field HorizontalScrollbar any
---@field MenuBar any
---@field NoBackground any
---@field NoBringToFrontOnFocus any
---@field NoCollapse any
---@field NoDecoration any
---@field NoDocking any
---@field NoFocusOnAppearing any
---@field NoInputs any
---@field NoMouseInputs any
---@field NoMove any
---@field NoNav any
---@field NoNavFocus any
---@field NoNavInputs any
---@field NoResize any
---@field NoSavedSettings any
---@field NoScrollWithMouse any
---@field NoScrollbar any
---@field NoTitleBar any
---@field None any
---@field UnsavedDocument any
ImGuiWindowFlags = {}


---@class ImTextureFormat
---@field _Alpha8 any
---@field _RGBA32 any
ImTextureFormat = {}


---@class ImTextureStatus
---@field _Destroyed any
---@field _OK any
---@field _WantCreate any
---@field _WantDestroy any
---@field _WantUpdates any
ImTextureStatus = {}


---@class ImVec2
ImVec2 = {}


---@class ImVec4
ImVec4 = {}


function acquire_scene_assets(...) end

---@class animation_component
animation_component = {}


---@class audio_listener_component
audio_listener_component = {}


---@class audio_source_component
audio_source_component = {}


function blam(...) end

---@class box_collider_component
box_collider_component = {}


---@class camera_follow_component
camera_follow_component = {}


function clear_scene(...) end

---@class color
color = {}


---@class entity
entity = {}


function find_entity_by_name(...) end

function fire_projectile(...) end

function get_asset_path(...) end

function get_camera_position(...) end

function get_game_map_dimensions(...) end

function get_name(...) end

function get_position(...) end

---@class health_component
health_component = {}


---@class input
input = {}

function input.bind(...) end

function input.is_action_down(...) end

function input.is_action_pressed(...) end

function input.is_action_released(...) end

function input.is_focused(...) end

function input.is_hovered(...) end

function input.is_key_down(...) end

function input.is_key_pressed(...) end

function input.is_key_released(...) end

function input.is_mouse_down(...) end

function input.is_mouse_pressed(...) end

function input.is_mouse_released(...) end

function input.mouse_position(...) end

function input.mouse_wheel(...) end

function input.on_key_down(...) end

function input.on_key_up(...) end

function input.on_mouse_down(...) end

function input.on_mouse_up(...) end

function input.on_mouse_wheel(...) end

function input.unbind(...) end


function load_asset(...) end

function load_entity(...) end

function load_scene(...) end

function log(...) end

function log_e(...) end

function log_i(...) end

function log_w(...) end

---@class name_component
name_component = {}


function play_sound(...) end

---@class position_component
position_component = {}


---@class project
project = {}

function project.path(...) end

function project.write_file(...) end

function project.write_table(...) end


---@class projectile_emitter_component
projectile_emitter_component = {}


function quit_game(...) end

function read_file_lines(...) end

---@class registry
registry = {}

function registry.get_animation(...) end

function registry.get_audio_listener(...) end

function registry.get_audio_source(...) end

function registry.get_box_collider(...) end

function registry.get_camera_follow(...) end

function registry.get_health(...) end

function registry.get_name(...) end

function registry.get_parent(...) end

function registry.get_position(...) end

function registry.get_projectile_emitter(...) end

function registry.get_rigidbody(...) end

function registry.get_rotation(...) end

function registry.get_scale(...) end

function registry.get_script(...) end

function registry.get_sprite(...) end

function registry.get_square(...) end

function registry.get_text_label(...) end

function registry.get_ui_anchor(...) end

function registry.get_ui_button(...) end

function registry.get_ui_canvas(...) end

function registry.get_ui_rect(...) end

function registry.get_ui_z_index(...) end

function registry.has_animation(...) end

function registry.has_audio_listener(...) end

function registry.has_audio_source(...) end

function registry.has_box_collider(...) end

function registry.has_camera_follow(...) end

function registry.has_health(...) end

function registry.has_name(...) end

function registry.has_position(...) end

function registry.has_projectile_emitter(...) end

function registry.has_rigidbody(...) end

function registry.has_rotation(...) end

function registry.has_scale(...) end

function registry.has_script(...) end

function registry.has_sprite(...) end

function registry.has_square(...) end

function registry.has_text_label(...) end

function registry.has_ui_anchor(...) end

function registry.has_ui_button(...) end

function registry.has_ui_canvas(...) end

function registry.has_ui_rect(...) end

function registry.has_ui_z_index(...) end

function registry.set_parent(...) end


function reload_scene(...) end

---@class rigidbody_component
rigidbody_component = {}


---@class rotation_component
rotation_component = {}


---@class scale_component
scale_component = {}


---@class script_component
script_component = {}


function set_game_map_dimensions(...) end

function set_name(...) end

function set_perf_overlay(...) end

function set_position(...) end

function set_sprite_src_rect(...) end

---@class sprite_component
sprite_component = {}


---@class square_primitive_component
square_primitive_component = {}


function stop_scene(...) end

---@class storage
storage = {}

function storage.exists(...) end

function storage.list(...) end

function storage.read(...) end

function storage.read_table(...) end

function storage.remove(...) end

function storage.write(...) end

function storage.write_table(...) end


---@class text_label_component
text_label_component = {}


function toggle_perf_overlay(...) end

---@class ui
ui = {}

function ui.anchor(...) end

function ui.canvas(...) end

function ui.flex(...) end

function ui.z_index(...) end


---@class ui_anchor_component
ui_anchor_component = {}


---@class ui_button_component
ui_button_component = {}


---@class ui_canvas_component
ui_canvas_component = {}


---@class ui_rect_component
ui_rect_component = {}


---@class ui_z_index_component
ui_z_index_component = {}


---@class vec2
vec2 = {}


