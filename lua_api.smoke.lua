-- AUTO-GENERATED Octarine Lua API surface. Do not edit by hand.
-- Introspected from the live sol::state after all bindings install.
-- Regenerate by setting OCTARINE_DUMP_LUA_API=<path> and launching the engine.
---@meta
---@diagnostic disable: lowercase-global, missing-return

---@class ImDrawFlags
ImDrawFlags = {}

---@field Closed any   -- on ImDrawFlags
---@field None any   -- on ImDrawFlags
---@field RoundCornersAll any   -- on ImDrawFlags
---@field RoundCornersBottom any   -- on ImDrawFlags
---@field RoundCornersBottomLeft any   -- on ImDrawFlags
---@field RoundCornersBottomRight any   -- on ImDrawFlags
---@field RoundCornersLeft any   -- on ImDrawFlags
---@field RoundCornersNone any   -- on ImDrawFlags
---@field RoundCornersRight any   -- on ImDrawFlags
---@field RoundCornersTop any   -- on ImDrawFlags
---@field RoundCornersTopLeft any   -- on ImDrawFlags
---@field RoundCornersTopRight any   -- on ImDrawFlags

---@class ImDrawListFlags
ImDrawListFlags = {}

---@field AllowVtxOffset any   -- on ImDrawListFlags
---@field AntiAliasedFill any   -- on ImDrawListFlags
---@field AntiAliasedLines any   -- on ImDrawListFlags
---@field AntiAliasedLinesUseTex any   -- on ImDrawListFlags
---@field None any   -- on ImDrawListFlags

---@class ImFontAtlasFlags
ImFontAtlasFlags = {}

---@field NoBakedLines any   -- on ImFontAtlasFlags
---@field NoMouseCursors any   -- on ImFontAtlasFlags
---@field NoPowerOfTwoHeight any   -- on ImFontAtlasFlags
---@field None any   -- on ImFontAtlasFlags

---@class ImFontFlags
ImFontFlags = {}

---@field LockBakedSizes any   -- on ImFontFlags
---@field NoLoadError any   -- on ImFontFlags
---@field NoLoadGlyphs any   -- on ImFontFlags
---@field None any   -- on ImFontFlags

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
ImGuiBackendFlags = {}

---@field HasGamepad any   -- on ImGuiBackendFlags
---@field HasMouseCursors any   -- on ImGuiBackendFlags
---@field HasMouseHoveredViewport any   -- on ImGuiBackendFlags
---@field HasParentViewport any   -- on ImGuiBackendFlags
---@field HasSetMousePos any   -- on ImGuiBackendFlags
---@field None any   -- on ImGuiBackendFlags
---@field PlatformHasViewports any   -- on ImGuiBackendFlags
---@field RendererHasTextures any   -- on ImGuiBackendFlags
---@field RendererHasViewports any   -- on ImGuiBackendFlags
---@field RendererHasVtxOffset any   -- on ImGuiBackendFlags

---@class ImGuiButtonFlags
ImGuiButtonFlags = {}

---@field AllowOverlap any   -- on ImGuiButtonFlags
---@field EnableNav any   -- on ImGuiButtonFlags
---@field MouseButtonLeft any   -- on ImGuiButtonFlags
---@field MouseButtonMiddle any   -- on ImGuiButtonFlags
---@field MouseButtonRight any   -- on ImGuiButtonFlags
---@field None any   -- on ImGuiButtonFlags

---@class ImGuiChildFlags
ImGuiChildFlags = {}

---@field AlwaysAutoResize any   -- on ImGuiChildFlags
---@field AlwaysUseWindowPadding any   -- on ImGuiChildFlags
---@field AutoResizeX any   -- on ImGuiChildFlags
---@field AutoResizeY any   -- on ImGuiChildFlags
---@field Borders any   -- on ImGuiChildFlags
---@field FrameStyle any   -- on ImGuiChildFlags
---@field NavFlattened any   -- on ImGuiChildFlags
---@field None any   -- on ImGuiChildFlags
---@field ResizeX any   -- on ImGuiChildFlags
---@field ResizeY any   -- on ImGuiChildFlags

---@class ImGuiCol
ImGuiCol = {}

---@field Border any   -- on ImGuiCol
---@field BorderShadow any   -- on ImGuiCol
---@field Button any   -- on ImGuiCol
---@field ButtonActive any   -- on ImGuiCol
---@field ButtonHovered any   -- on ImGuiCol
---@field COUNT any   -- on ImGuiCol
---@field CheckMark any   -- on ImGuiCol
---@field ChildBg any   -- on ImGuiCol
---@field DockingEmptyBg any   -- on ImGuiCol
---@field DockingPreview any   -- on ImGuiCol
---@field DragDropTarget any   -- on ImGuiCol
---@field DragDropTargetBg any   -- on ImGuiCol
---@field FrameBg any   -- on ImGuiCol
---@field FrameBgActive any   -- on ImGuiCol
---@field FrameBgHovered any   -- on ImGuiCol
---@field Header any   -- on ImGuiCol
---@field HeaderActive any   -- on ImGuiCol
---@field HeaderHovered any   -- on ImGuiCol
---@field InputTextCursor any   -- on ImGuiCol
---@field MenuBarBg any   -- on ImGuiCol
---@field ModalWindowDimBg any   -- on ImGuiCol
---@field NavCursor any   -- on ImGuiCol
---@field NavHighlight any   -- on ImGuiCol
---@field NavWindowingDimBg any   -- on ImGuiCol
---@field NavWindowingHighlight any   -- on ImGuiCol
---@field PlotHistogram any   -- on ImGuiCol
---@field PlotHistogramHovered any   -- on ImGuiCol
---@field PlotLines any   -- on ImGuiCol
---@field PlotLinesHovered any   -- on ImGuiCol
---@field PopupBg any   -- on ImGuiCol
---@field ResizeGrip any   -- on ImGuiCol
---@field ResizeGripActive any   -- on ImGuiCol
---@field ResizeGripHovered any   -- on ImGuiCol
---@field ScrollbarBg any   -- on ImGuiCol
---@field ScrollbarGrab any   -- on ImGuiCol
---@field ScrollbarGrabActive any   -- on ImGuiCol
---@field ScrollbarGrabHovered any   -- on ImGuiCol
---@field Separator any   -- on ImGuiCol
---@field SeparatorActive any   -- on ImGuiCol
---@field SeparatorHovered any   -- on ImGuiCol
---@field SliderGrab any   -- on ImGuiCol
---@field SliderGrabActive any   -- on ImGuiCol
---@field Tab any   -- on ImGuiCol
---@field TabActive any   -- on ImGuiCol
---@field TabDimmed any   -- on ImGuiCol
---@field TabDimmedSelected any   -- on ImGuiCol
---@field TabDimmedSelectedOverline any   -- on ImGuiCol
---@field TabHovered any   -- on ImGuiCol
---@field TabSelected any   -- on ImGuiCol
---@field TabSelectedOverline any   -- on ImGuiCol
---@field TabUnfocused any   -- on ImGuiCol
---@field TabUnfocusedActive any   -- on ImGuiCol
---@field TableBorderLight any   -- on ImGuiCol
---@field TableBorderStrong any   -- on ImGuiCol
---@field TableHeaderBg any   -- on ImGuiCol
---@field TableRowBg any   -- on ImGuiCol
---@field TableRowBgAlt any   -- on ImGuiCol
---@field Text any   -- on ImGuiCol
---@field TextDisabled any   -- on ImGuiCol
---@field TextLink any   -- on ImGuiCol
---@field TextSelectedBg any   -- on ImGuiCol
---@field TitleBg any   -- on ImGuiCol
---@field TitleBgActive any   -- on ImGuiCol
---@field TitleBgCollapsed any   -- on ImGuiCol
---@field TreeLines any   -- on ImGuiCol
---@field UnsavedMarker any   -- on ImGuiCol
---@field WindowBg any   -- on ImGuiCol

---@class ImGuiColorEditFlags
ImGuiColorEditFlags = {}

---@field AlphaBar any   -- on ImGuiColorEditFlags
---@field AlphaNoBg any   -- on ImGuiColorEditFlags
---@field AlphaOpaque any   -- on ImGuiColorEditFlags
---@field AlphaPreview any   -- on ImGuiColorEditFlags
---@field AlphaPreviewHalf any   -- on ImGuiColorEditFlags
---@field DisplayHSV any   -- on ImGuiColorEditFlags
---@field DisplayHex any   -- on ImGuiColorEditFlags
---@field DisplayRGB any   -- on ImGuiColorEditFlags
---@field Float any   -- on ImGuiColorEditFlags
---@field HDR any   -- on ImGuiColorEditFlags
---@field InputHSV any   -- on ImGuiColorEditFlags
---@field InputRGB any   -- on ImGuiColorEditFlags
---@field NoAlpha any   -- on ImGuiColorEditFlags
---@field NoBorder any   -- on ImGuiColorEditFlags
---@field NoColorMarkers any   -- on ImGuiColorEditFlags
---@field NoDragDrop any   -- on ImGuiColorEditFlags
---@field NoInputs any   -- on ImGuiColorEditFlags
---@field NoLabel any   -- on ImGuiColorEditFlags
---@field NoOptions any   -- on ImGuiColorEditFlags
---@field NoPicker any   -- on ImGuiColorEditFlags
---@field NoSidePreview any   -- on ImGuiColorEditFlags
---@field NoSmallPreview any   -- on ImGuiColorEditFlags
---@field NoTooltip any   -- on ImGuiColorEditFlags
---@field None any   -- on ImGuiColorEditFlags
---@field PickerHueBar any   -- on ImGuiColorEditFlags
---@field PickerHueWheel any   -- on ImGuiColorEditFlags
---@field Uint8 any   -- on ImGuiColorEditFlags

---@class ImGuiComboFlags
ImGuiComboFlags = {}

---@field HeightLarge any   -- on ImGuiComboFlags
---@field HeightLargest any   -- on ImGuiComboFlags
---@field HeightRegular any   -- on ImGuiComboFlags
---@field HeightSmall any   -- on ImGuiComboFlags
---@field NoArrowButton any   -- on ImGuiComboFlags
---@field NoPreview any   -- on ImGuiComboFlags
---@field None any   -- on ImGuiComboFlags
---@field PopupAlignLeft any   -- on ImGuiComboFlags
---@field WidthFitPreview any   -- on ImGuiComboFlags

---@class ImGuiCond
ImGuiCond = {}

---@field Always any   -- on ImGuiCond
---@field Appearing any   -- on ImGuiCond
---@field FirstUseEver any   -- on ImGuiCond
---@field None any   -- on ImGuiCond
---@field Once any   -- on ImGuiCond

---@class ImGuiConfigFlags
ImGuiConfigFlags = {}

---@field DockingEnable any   -- on ImGuiConfigFlags
---@field DpiEnableScaleFonts any   -- on ImGuiConfigFlags
---@field DpiEnableScaleViewports any   -- on ImGuiConfigFlags
---@field IsSRGB any   -- on ImGuiConfigFlags
---@field IsTouchScreen any   -- on ImGuiConfigFlags
---@field NavEnableGamepad any   -- on ImGuiConfigFlags
---@field NavEnableKeyboard any   -- on ImGuiConfigFlags
---@field NavEnableSetMousePos any   -- on ImGuiConfigFlags
---@field NavNoCaptureKeyboard any   -- on ImGuiConfigFlags
---@field NoKeyboard any   -- on ImGuiConfigFlags
---@field NoMouse any   -- on ImGuiConfigFlags
---@field NoMouseCursorChange any   -- on ImGuiConfigFlags
---@field None any   -- on ImGuiConfigFlags
---@field ViewportsEnable any   -- on ImGuiConfigFlags

---@class ImGuiDataType
ImGuiDataType = {}

---@field Bool any   -- on ImGuiDataType
---@field COUNT any   -- on ImGuiDataType
---@field Double any   -- on ImGuiDataType
---@field Float any   -- on ImGuiDataType
---@field S16 any   -- on ImGuiDataType
---@field S32 any   -- on ImGuiDataType
---@field S64 any   -- on ImGuiDataType
---@field S8 any   -- on ImGuiDataType
---@field String any   -- on ImGuiDataType
---@field U16 any   -- on ImGuiDataType
---@field U32 any   -- on ImGuiDataType
---@field U64 any   -- on ImGuiDataType
---@field U8 any   -- on ImGuiDataType

---@class ImGuiDir
ImGuiDir = {}

---@field _COUNT any   -- on ImGuiDir
---@field _Down any   -- on ImGuiDir
---@field _Left any   -- on ImGuiDir
---@field _None any   -- on ImGuiDir
---@field _Right any   -- on ImGuiDir
---@field _Up any   -- on ImGuiDir

---@class ImGuiDockNodeFlags
ImGuiDockNodeFlags = {}

---@field AutoHideTabBar any   -- on ImGuiDockNodeFlags
---@field KeepAliveOnly any   -- on ImGuiDockNodeFlags
---@field NoDockingInCentralNode any   -- on ImGuiDockNodeFlags
---@field NoDockingOverCentralNode any   -- on ImGuiDockNodeFlags
---@field NoDockingSplit any   -- on ImGuiDockNodeFlags
---@field NoResize any   -- on ImGuiDockNodeFlags
---@field NoSplit any   -- on ImGuiDockNodeFlags
---@field NoUndocking any   -- on ImGuiDockNodeFlags
---@field None any   -- on ImGuiDockNodeFlags
---@field PassthruCentralNode any   -- on ImGuiDockNodeFlags

---@class ImGuiDragDropFlags
ImGuiDragDropFlags = {}

---@field AcceptBeforeDelivery any   -- on ImGuiDragDropFlags
---@field AcceptDrawAsHovered any   -- on ImGuiDragDropFlags
---@field AcceptNoDrawDefaultRect any   -- on ImGuiDragDropFlags
---@field AcceptNoPreviewTooltip any   -- on ImGuiDragDropFlags
---@field AcceptPeekOnly any   -- on ImGuiDragDropFlags
---@field None any   -- on ImGuiDragDropFlags
---@field PayloadAutoExpire any   -- on ImGuiDragDropFlags
---@field PayloadNoCrossContext any   -- on ImGuiDragDropFlags
---@field PayloadNoCrossProcess any   -- on ImGuiDragDropFlags
---@field SourceAllowNullID any   -- on ImGuiDragDropFlags
---@field SourceAutoExpirePayload any   -- on ImGuiDragDropFlags
---@field SourceExtern any   -- on ImGuiDragDropFlags
---@field SourceNoDisableHover any   -- on ImGuiDragDropFlags
---@field SourceNoHoldToOpenOthers any   -- on ImGuiDragDropFlags
---@field SourceNoPreviewTooltip any   -- on ImGuiDragDropFlags

---@class ImGuiFocusedFlags
ImGuiFocusedFlags = {}

---@field AnyWindow any   -- on ImGuiFocusedFlags
---@field ChildWindows any   -- on ImGuiFocusedFlags
---@field DockHierarchy any   -- on ImGuiFocusedFlags
---@field NoPopupHierarchy any   -- on ImGuiFocusedFlags
---@field None any   -- on ImGuiFocusedFlags
---@field RootAndChildWindows any   -- on ImGuiFocusedFlags
---@field RootWindow any   -- on ImGuiFocusedFlags

---@class ImGuiHoveredFlags
ImGuiHoveredFlags = {}

---@field AllowWhenBlockedByActiveItem any   -- on ImGuiHoveredFlags
---@field AllowWhenBlockedByPopup any   -- on ImGuiHoveredFlags
---@field AllowWhenDisabled any   -- on ImGuiHoveredFlags
---@field AllowWhenOverlapped any   -- on ImGuiHoveredFlags
---@field AllowWhenOverlappedByItem any   -- on ImGuiHoveredFlags
---@field AllowWhenOverlappedByWindow any   -- on ImGuiHoveredFlags
---@field AnyWindow any   -- on ImGuiHoveredFlags
---@field ChildWindows any   -- on ImGuiHoveredFlags
---@field DelayNone any   -- on ImGuiHoveredFlags
---@field DelayNormal any   -- on ImGuiHoveredFlags
---@field DelayShort any   -- on ImGuiHoveredFlags
---@field DockHierarchy any   -- on ImGuiHoveredFlags
---@field ForTooltip any   -- on ImGuiHoveredFlags
---@field NoNavOverride any   -- on ImGuiHoveredFlags
---@field NoPopupHierarchy any   -- on ImGuiHoveredFlags
---@field NoSharedDelay any   -- on ImGuiHoveredFlags
---@field None any   -- on ImGuiHoveredFlags
---@field RectOnly any   -- on ImGuiHoveredFlags
---@field RootAndChildWindows any   -- on ImGuiHoveredFlags
---@field RootWindow any   -- on ImGuiHoveredFlags
---@field Stationary any   -- on ImGuiHoveredFlags

---@class ImGuiInputFlags
ImGuiInputFlags = {}

---@field None any   -- on ImGuiInputFlags
---@field Repeat any   -- on ImGuiInputFlags
---@field RouteActive any   -- on ImGuiInputFlags
---@field RouteAlways any   -- on ImGuiInputFlags
---@field RouteFocused any   -- on ImGuiInputFlags
---@field RouteFromRootWindow any   -- on ImGuiInputFlags
---@field RouteGlobal any   -- on ImGuiInputFlags
---@field RouteOverActive any   -- on ImGuiInputFlags
---@field RouteOverFocused any   -- on ImGuiInputFlags
---@field RouteUnlessBgFocused any   -- on ImGuiInputFlags
---@field Tooltip any   -- on ImGuiInputFlags

---@class ImGuiInputTextFlags
ImGuiInputTextFlags = {}

---@field AllowTabInput any   -- on ImGuiInputTextFlags
---@field AlwaysOverwrite any   -- on ImGuiInputTextFlags
---@field AutoSelectAll any   -- on ImGuiInputTextFlags
---@field CallbackAlways any   -- on ImGuiInputTextFlags
---@field CallbackCharFilter any   -- on ImGuiInputTextFlags
---@field CallbackCompletion any   -- on ImGuiInputTextFlags
---@field CallbackEdit any   -- on ImGuiInputTextFlags
---@field CallbackHistory any   -- on ImGuiInputTextFlags
---@field CallbackResize any   -- on ImGuiInputTextFlags
---@field CharsDecimal any   -- on ImGuiInputTextFlags
---@field CharsHexadecimal any   -- on ImGuiInputTextFlags
---@field CharsNoBlank any   -- on ImGuiInputTextFlags
---@field CharsScientific any   -- on ImGuiInputTextFlags
---@field CharsUppercase any   -- on ImGuiInputTextFlags
---@field CtrlEnterForNewLine any   -- on ImGuiInputTextFlags
---@field DisplayEmptyRefVal any   -- on ImGuiInputTextFlags
---@field ElideLeft any   -- on ImGuiInputTextFlags
---@field EnterReturnsTrue any   -- on ImGuiInputTextFlags
---@field EscapeClearsAll any   -- on ImGuiInputTextFlags
---@field NoHorizontalScroll any   -- on ImGuiInputTextFlags
---@field NoUndoRedo any   -- on ImGuiInputTextFlags
---@field None any   -- on ImGuiInputTextFlags
---@field ParseEmptyRefVal any   -- on ImGuiInputTextFlags
---@field Password any   -- on ImGuiInputTextFlags
---@field ReadOnly any   -- on ImGuiInputTextFlags
---@field WordWrap any   -- on ImGuiInputTextFlags

---@class ImGuiItemFlags
ImGuiItemFlags = {}

---@field AllowDuplicateId any   -- on ImGuiItemFlags
---@field AutoClosePopups any   -- on ImGuiItemFlags
---@field ButtonRepeat any   -- on ImGuiItemFlags
---@field Disabled any   -- on ImGuiItemFlags
---@field NoNav any   -- on ImGuiItemFlags
---@field NoNavDefaultFocus any   -- on ImGuiItemFlags
---@field NoTabStop any   -- on ImGuiItemFlags
---@field None any   -- on ImGuiItemFlags

---@class ImGuiKey
ImGuiKey = {}

---@field ImGuiMod_Alt any   -- on ImGuiKey
---@field ImGuiMod_Ctrl any   -- on ImGuiKey
---@field ImGuiMod_None any   -- on ImGuiKey
---@field ImGuiMod_Shift any   -- on ImGuiKey
---@field ImGuiMod_Shortcut any   -- on ImGuiKey
---@field ImGuiMod_Super any   -- on ImGuiKey
---@field _0 any   -- on ImGuiKey
---@field _1 any   -- on ImGuiKey
---@field _2 any   -- on ImGuiKey
---@field _3 any   -- on ImGuiKey
---@field _4 any   -- on ImGuiKey
---@field _5 any   -- on ImGuiKey
---@field _6 any   -- on ImGuiKey
---@field _7 any   -- on ImGuiKey
---@field _8 any   -- on ImGuiKey
---@field _9 any   -- on ImGuiKey
---@field _A any   -- on ImGuiKey
---@field _Apostrophe any   -- on ImGuiKey
---@field _AppBack any   -- on ImGuiKey
---@field _AppForward any   -- on ImGuiKey
---@field _B any   -- on ImGuiKey
---@field _Backslash any   -- on ImGuiKey
---@field _Backspace any   -- on ImGuiKey
---@field _C any   -- on ImGuiKey
---@field _COUNT any   -- on ImGuiKey
---@field _CapsLock any   -- on ImGuiKey
---@field _Comma any   -- on ImGuiKey
---@field _D any   -- on ImGuiKey
---@field _Delete any   -- on ImGuiKey
---@field _DownArrow any   -- on ImGuiKey
---@field _E any   -- on ImGuiKey
---@field _End any   -- on ImGuiKey
---@field _Enter any   -- on ImGuiKey
---@field _Equal any   -- on ImGuiKey
---@field _Escape any   -- on ImGuiKey
---@field _F any   -- on ImGuiKey
---@field _F1 any   -- on ImGuiKey
---@field _F10 any   -- on ImGuiKey
---@field _F11 any   -- on ImGuiKey
---@field _F12 any   -- on ImGuiKey
---@field _F13 any   -- on ImGuiKey
---@field _F14 any   -- on ImGuiKey
---@field _F15 any   -- on ImGuiKey
---@field _F16 any   -- on ImGuiKey
---@field _F17 any   -- on ImGuiKey
---@field _F18 any   -- on ImGuiKey
---@field _F19 any   -- on ImGuiKey
---@field _F2 any   -- on ImGuiKey
---@field _F20 any   -- on ImGuiKey
---@field _F21 any   -- on ImGuiKey
---@field _F22 any   -- on ImGuiKey
---@field _F23 any   -- on ImGuiKey
---@field _F24 any   -- on ImGuiKey
---@field _F3 any   -- on ImGuiKey
---@field _F4 any   -- on ImGuiKey
---@field _F5 any   -- on ImGuiKey
---@field _F6 any   -- on ImGuiKey
---@field _F7 any   -- on ImGuiKey
---@field _F8 any   -- on ImGuiKey
---@field _F9 any   -- on ImGuiKey
---@field _G any   -- on ImGuiKey
---@field _GamepadBack any   -- on ImGuiKey
---@field _GamepadDpadDown any   -- on ImGuiKey
---@field _GamepadDpadLeft any   -- on ImGuiKey
---@field _GamepadDpadRight any   -- on ImGuiKey
---@field _GamepadDpadUp any   -- on ImGuiKey
---@field _GamepadFaceDown any   -- on ImGuiKey
---@field _GamepadFaceLeft any   -- on ImGuiKey
---@field _GamepadFaceRight any   -- on ImGuiKey
---@field _GamepadFaceUp any   -- on ImGuiKey
---@field _GamepadL1 any   -- on ImGuiKey
---@field _GamepadL2 any   -- on ImGuiKey
---@field _GamepadL3 any   -- on ImGuiKey
---@field _GamepadLStickDown any   -- on ImGuiKey
---@field _GamepadLStickLeft any   -- on ImGuiKey
---@field _GamepadLStickRight any   -- on ImGuiKey
---@field _GamepadLStickUp any   -- on ImGuiKey
---@field _GamepadR1 any   -- on ImGuiKey
---@field _GamepadR2 any   -- on ImGuiKey
---@field _GamepadR3 any   -- on ImGuiKey
---@field _GamepadRStickDown any   -- on ImGuiKey
---@field _GamepadRStickLeft any   -- on ImGuiKey
---@field _GamepadRStickRight any   -- on ImGuiKey
---@field _GamepadRStickUp any   -- on ImGuiKey
---@field _GamepadStart any   -- on ImGuiKey
---@field _GraveAccent any   -- on ImGuiKey
---@field _H any   -- on ImGuiKey
---@field _Home any   -- on ImGuiKey
---@field _I any   -- on ImGuiKey
---@field _Insert any   -- on ImGuiKey
---@field _J any   -- on ImGuiKey
---@field _K any   -- on ImGuiKey
---@field _Keypad0 any   -- on ImGuiKey
---@field _Keypad1 any   -- on ImGuiKey
---@field _Keypad2 any   -- on ImGuiKey
---@field _Keypad3 any   -- on ImGuiKey
---@field _Keypad4 any   -- on ImGuiKey
---@field _Keypad5 any   -- on ImGuiKey
---@field _Keypad6 any   -- on ImGuiKey
---@field _Keypad7 any   -- on ImGuiKey
---@field _Keypad8 any   -- on ImGuiKey
---@field _Keypad9 any   -- on ImGuiKey
---@field _KeypadAdd any   -- on ImGuiKey
---@field _KeypadDecimal any   -- on ImGuiKey
---@field _KeypadDivide any   -- on ImGuiKey
---@field _KeypadEnter any   -- on ImGuiKey
---@field _KeypadEqual any   -- on ImGuiKey
---@field _KeypadMultiply any   -- on ImGuiKey
---@field _KeypadSubtract any   -- on ImGuiKey
---@field _L any   -- on ImGuiKey
---@field _LeftAlt any   -- on ImGuiKey
---@field _LeftArrow any   -- on ImGuiKey
---@field _LeftBracket any   -- on ImGuiKey
---@field _LeftCtrl any   -- on ImGuiKey
---@field _LeftShift any   -- on ImGuiKey
---@field _LeftSuper any   -- on ImGuiKey
---@field _M any   -- on ImGuiKey
---@field _Menu any   -- on ImGuiKey
---@field _Minus any   -- on ImGuiKey
---@field _MouseLeft any   -- on ImGuiKey
---@field _MouseMiddle any   -- on ImGuiKey
---@field _MouseRight any   -- on ImGuiKey
---@field _MouseWheelX any   -- on ImGuiKey
---@field _MouseWheelY any   -- on ImGuiKey
---@field _MouseX1 any   -- on ImGuiKey
---@field _MouseX2 any   -- on ImGuiKey
---@field _N any   -- on ImGuiKey
---@field _NamedKey_BEGIN any   -- on ImGuiKey
---@field _None any   -- on ImGuiKey
---@field _NumLock any   -- on ImGuiKey
---@field _O any   -- on ImGuiKey
---@field _Oem102 any   -- on ImGuiKey
---@field _P any   -- on ImGuiKey
---@field _PageDown any   -- on ImGuiKey
---@field _PageUp any   -- on ImGuiKey
---@field _Pause any   -- on ImGuiKey
---@field _Period any   -- on ImGuiKey
---@field _PrintScreen any   -- on ImGuiKey
---@field _Q any   -- on ImGuiKey
---@field _R any   -- on ImGuiKey
---@field _RightAlt any   -- on ImGuiKey
---@field _RightArrow any   -- on ImGuiKey
---@field _RightBracket any   -- on ImGuiKey
---@field _RightCtrl any   -- on ImGuiKey
---@field _RightShift any   -- on ImGuiKey
---@field _RightSuper any   -- on ImGuiKey
---@field _S any   -- on ImGuiKey
---@field _ScrollLock any   -- on ImGuiKey
---@field _Semicolon any   -- on ImGuiKey
---@field _Slash any   -- on ImGuiKey
---@field _Space any   -- on ImGuiKey
---@field _T any   -- on ImGuiKey
---@field _Tab any   -- on ImGuiKey
---@field _U any   -- on ImGuiKey
---@field _UpArrow any   -- on ImGuiKey
---@field _V any   -- on ImGuiKey
---@field _W any   -- on ImGuiKey
---@field _X any   -- on ImGuiKey
---@field _Y any   -- on ImGuiKey
---@field _Z any   -- on ImGuiKey

---@class ImGuiListClipperFlags
ImGuiListClipperFlags = {}

---@field NoSetTableRowCounters any   -- on ImGuiListClipperFlags
---@field None any   -- on ImGuiListClipperFlags

---@class ImGuiMouseButton
ImGuiMouseButton = {}

---@field COUNT any   -- on ImGuiMouseButton
---@field Left any   -- on ImGuiMouseButton
---@field Middle any   -- on ImGuiMouseButton
---@field Right any   -- on ImGuiMouseButton

---@class ImGuiMouseCursor
ImGuiMouseCursor = {}

---@field Arrow any   -- on ImGuiMouseCursor
---@field COUNT any   -- on ImGuiMouseCursor
---@field Hand any   -- on ImGuiMouseCursor
---@field None any   -- on ImGuiMouseCursor
---@field NotAllowed any   -- on ImGuiMouseCursor
---@field Progress any   -- on ImGuiMouseCursor
---@field ResizeAll any   -- on ImGuiMouseCursor
---@field ResizeEW any   -- on ImGuiMouseCursor
---@field ResizeNESW any   -- on ImGuiMouseCursor
---@field ResizeNS any   -- on ImGuiMouseCursor
---@field ResizeNWSE any   -- on ImGuiMouseCursor
---@field TextInput any   -- on ImGuiMouseCursor
---@field Wait any   -- on ImGuiMouseCursor

---@class ImGuiMouseSource
ImGuiMouseSource = {}

---@field _COUNT any   -- on ImGuiMouseSource
---@field _Mouse any   -- on ImGuiMouseSource
---@field _Pen any   -- on ImGuiMouseSource
---@field _TouchScreen any   -- on ImGuiMouseSource

---@class ImGuiMultiSelectFlags
ImGuiMultiSelectFlags = {}

---@field BoxSelect1d any   -- on ImGuiMultiSelectFlags
---@field BoxSelect2d any   -- on ImGuiMultiSelectFlags
---@field BoxSelectNoScroll any   -- on ImGuiMultiSelectFlags
---@field ClearOnClickVoid any   -- on ImGuiMultiSelectFlags
---@field ClearOnEscape any   -- on ImGuiMultiSelectFlags
---@field NavWrapX any   -- on ImGuiMultiSelectFlags
---@field NoAutoClear any   -- on ImGuiMultiSelectFlags
---@field NoAutoClearOnReselect any   -- on ImGuiMultiSelectFlags
---@field NoAutoSelect any   -- on ImGuiMultiSelectFlags
---@field NoRangeSelect any   -- on ImGuiMultiSelectFlags
---@field NoSelectAll any   -- on ImGuiMultiSelectFlags
---@field NoSelectOnRightClick any   -- on ImGuiMultiSelectFlags
---@field None any   -- on ImGuiMultiSelectFlags
---@field ScopeRect any   -- on ImGuiMultiSelectFlags
---@field ScopeWindow any   -- on ImGuiMultiSelectFlags
---@field SelectOnAuto any   -- on ImGuiMultiSelectFlags
---@field SelectOnClick any   -- on ImGuiMultiSelectFlags
---@field SelectOnClickAlways any   -- on ImGuiMultiSelectFlags
---@field SelectOnClickRelease any   -- on ImGuiMultiSelectFlags
---@field SingleSelect any   -- on ImGuiMultiSelectFlags

---@class ImGuiPopupFlags
ImGuiPopupFlags = {}

---@field AnyPopup any   -- on ImGuiPopupFlags
---@field AnyPopupId any   -- on ImGuiPopupFlags
---@field AnyPopupLevel any   -- on ImGuiPopupFlags
---@field MouseButtonLeft any   -- on ImGuiPopupFlags
---@field MouseButtonMiddle any   -- on ImGuiPopupFlags
---@field MouseButtonRight any   -- on ImGuiPopupFlags
---@field NoOpenOverExistingPopup any   -- on ImGuiPopupFlags
---@field NoOpenOverItems any   -- on ImGuiPopupFlags
---@field NoReopen any   -- on ImGuiPopupFlags
---@field None any   -- on ImGuiPopupFlags

---@class ImGuiSelectableFlags
ImGuiSelectableFlags = {}

---@field AllowDoubleClick any   -- on ImGuiSelectableFlags
---@field AllowOverlap any   -- on ImGuiSelectableFlags
---@field Disabled any   -- on ImGuiSelectableFlags
---@field DontClosePopups any   -- on ImGuiSelectableFlags
---@field Highlight any   -- on ImGuiSelectableFlags
---@field NoAutoClosePopups any   -- on ImGuiSelectableFlags
---@field None any   -- on ImGuiSelectableFlags
---@field SelectOnNav any   -- on ImGuiSelectableFlags
---@field SpanAllColumns any   -- on ImGuiSelectableFlags

---@class ImGuiSelectionRequestType
ImGuiSelectionRequestType = {}

---@field _None any   -- on ImGuiSelectionRequestType
---@field _SetAll any   -- on ImGuiSelectionRequestType
---@field _SetRange any   -- on ImGuiSelectionRequestType

---@class ImGuiSliderFlags
ImGuiSliderFlags = {}

---@field AlwaysClamp any   -- on ImGuiSliderFlags
---@field ClampOnInput any   -- on ImGuiSliderFlags
---@field ClampZeroRange any   -- on ImGuiSliderFlags
---@field ColorMarkers any   -- on ImGuiSliderFlags
---@field Logarithmic any   -- on ImGuiSliderFlags
---@field NoInput any   -- on ImGuiSliderFlags
---@field NoRoundToFormat any   -- on ImGuiSliderFlags
---@field NoSpeedTweaks any   -- on ImGuiSliderFlags
---@field None any   -- on ImGuiSliderFlags
---@field WrapAround any   -- on ImGuiSliderFlags

---@class ImGuiSortDirection
ImGuiSortDirection = {}

---@field _Ascending any   -- on ImGuiSortDirection
---@field _Descending any   -- on ImGuiSortDirection
---@field _None any   -- on ImGuiSortDirection

---@class ImGuiStyleVar
ImGuiStyleVar = {}

---@field Alpha any   -- on ImGuiStyleVar
---@field ButtonTextAlign any   -- on ImGuiStyleVar
---@field COUNT any   -- on ImGuiStyleVar
---@field CellPadding any   -- on ImGuiStyleVar
---@field ChildBorderSize any   -- on ImGuiStyleVar
---@field ChildRounding any   -- on ImGuiStyleVar
---@field DisabledAlpha any   -- on ImGuiStyleVar
---@field DockingSeparatorSize any   -- on ImGuiStyleVar
---@field FrameBorderSize any   -- on ImGuiStyleVar
---@field FramePadding any   -- on ImGuiStyleVar
---@field FrameRounding any   -- on ImGuiStyleVar
---@field GrabMinSize any   -- on ImGuiStyleVar
---@field GrabRounding any   -- on ImGuiStyleVar
---@field ImageBorderSize any   -- on ImGuiStyleVar
---@field ImageRounding any   -- on ImGuiStyleVar
---@field IndentSpacing any   -- on ImGuiStyleVar
---@field ItemInnerSpacing any   -- on ImGuiStyleVar
---@field ItemSpacing any   -- on ImGuiStyleVar
---@field PopupBorderSize any   -- on ImGuiStyleVar
---@field PopupRounding any   -- on ImGuiStyleVar
---@field ScrollbarPadding any   -- on ImGuiStyleVar
---@field ScrollbarRounding any   -- on ImGuiStyleVar
---@field ScrollbarSize any   -- on ImGuiStyleVar
---@field SelectableTextAlign any   -- on ImGuiStyleVar
---@field SeparatorSize any   -- on ImGuiStyleVar
---@field SeparatorTextAlign any   -- on ImGuiStyleVar
---@field SeparatorTextBorderSize any   -- on ImGuiStyleVar
---@field SeparatorTextPadding any   -- on ImGuiStyleVar
---@field TabBarBorderSize any   -- on ImGuiStyleVar
---@field TabBarOverlineSize any   -- on ImGuiStyleVar
---@field TabBorderSize any   -- on ImGuiStyleVar
---@field TabMinWidthBase any   -- on ImGuiStyleVar
---@field TabMinWidthShrink any   -- on ImGuiStyleVar
---@field TabRounding any   -- on ImGuiStyleVar
---@field TableAngledHeadersAngle any   -- on ImGuiStyleVar
---@field TableAngledHeadersTextAlign any   -- on ImGuiStyleVar
---@field TreeLinesRounding any   -- on ImGuiStyleVar
---@field TreeLinesSize any   -- on ImGuiStyleVar
---@field WindowBorderSize any   -- on ImGuiStyleVar
---@field WindowMinSize any   -- on ImGuiStyleVar
---@field WindowPadding any   -- on ImGuiStyleVar
---@field WindowRounding any   -- on ImGuiStyleVar
---@field WindowTitleAlign any   -- on ImGuiStyleVar

---@class ImGuiTabBarFlags
ImGuiTabBarFlags = {}

---@field AutoSelectNewTabs any   -- on ImGuiTabBarFlags
---@field DrawSelectedOverline any   -- on ImGuiTabBarFlags
---@field FittingPolicyMixed any   -- on ImGuiTabBarFlags
---@field FittingPolicyResizeDown any   -- on ImGuiTabBarFlags
---@field FittingPolicyScroll any   -- on ImGuiTabBarFlags
---@field FittingPolicyShrink any   -- on ImGuiTabBarFlags
---@field NoCloseWithMiddleMouseButton any   -- on ImGuiTabBarFlags
---@field NoTabListScrollingButtons any   -- on ImGuiTabBarFlags
---@field NoTooltip any   -- on ImGuiTabBarFlags
---@field None any   -- on ImGuiTabBarFlags
---@field Reorderable any   -- on ImGuiTabBarFlags
---@field TabListPopupButton any   -- on ImGuiTabBarFlags

---@class ImGuiTabItemFlags
ImGuiTabItemFlags = {}

---@field Leading any   -- on ImGuiTabItemFlags
---@field NoAssumedClosure any   -- on ImGuiTabItemFlags
---@field NoCloseWithMiddleMouseButton any   -- on ImGuiTabItemFlags
---@field NoPushId any   -- on ImGuiTabItemFlags
---@field NoReorder any   -- on ImGuiTabItemFlags
---@field NoTooltip any   -- on ImGuiTabItemFlags
---@field None any   -- on ImGuiTabItemFlags
---@field SetSelected any   -- on ImGuiTabItemFlags
---@field Trailing any   -- on ImGuiTabItemFlags
---@field UnsavedDocument any   -- on ImGuiTabItemFlags

---@class ImGuiTableBgTarget
ImGuiTableBgTarget = {}

---@field CellBg any   -- on ImGuiTableBgTarget
---@field None any   -- on ImGuiTableBgTarget
---@field RowBg0 any   -- on ImGuiTableBgTarget
---@field RowBg1 any   -- on ImGuiTableBgTarget

---@class ImGuiTableColumnFlags
ImGuiTableColumnFlags = {}

---@field AngledHeader any   -- on ImGuiTableColumnFlags
---@field DefaultHide any   -- on ImGuiTableColumnFlags
---@field DefaultSort any   -- on ImGuiTableColumnFlags
---@field Disabled any   -- on ImGuiTableColumnFlags
---@field IndentDisable any   -- on ImGuiTableColumnFlags
---@field IndentEnable any   -- on ImGuiTableColumnFlags
---@field IsEnabled any   -- on ImGuiTableColumnFlags
---@field IsHovered any   -- on ImGuiTableColumnFlags
---@field IsSorted any   -- on ImGuiTableColumnFlags
---@field IsVisible any   -- on ImGuiTableColumnFlags
---@field NoClip any   -- on ImGuiTableColumnFlags
---@field NoHeaderLabel any   -- on ImGuiTableColumnFlags
---@field NoHeaderWidth any   -- on ImGuiTableColumnFlags
---@field NoHide any   -- on ImGuiTableColumnFlags
---@field NoReorder any   -- on ImGuiTableColumnFlags
---@field NoResize any   -- on ImGuiTableColumnFlags
---@field NoSort any   -- on ImGuiTableColumnFlags
---@field NoSortAscending any   -- on ImGuiTableColumnFlags
---@field NoSortDescending any   -- on ImGuiTableColumnFlags
---@field None any   -- on ImGuiTableColumnFlags
---@field PreferSortAscending any   -- on ImGuiTableColumnFlags
---@field PreferSortDescending any   -- on ImGuiTableColumnFlags
---@field WidthFixed any   -- on ImGuiTableColumnFlags
---@field WidthStretch any   -- on ImGuiTableColumnFlags

---@class ImGuiTableFlags
ImGuiTableFlags = {}

---@field Borders any   -- on ImGuiTableFlags
---@field BordersH any   -- on ImGuiTableFlags
---@field BordersInner any   -- on ImGuiTableFlags
---@field BordersInnerH any   -- on ImGuiTableFlags
---@field BordersInnerV any   -- on ImGuiTableFlags
---@field BordersOuter any   -- on ImGuiTableFlags
---@field BordersOuterH any   -- on ImGuiTableFlags
---@field BordersOuterV any   -- on ImGuiTableFlags
---@field BordersV any   -- on ImGuiTableFlags
---@field ContextMenuInBody any   -- on ImGuiTableFlags
---@field Hideable any   -- on ImGuiTableFlags
---@field HighlightHoveredColumn any   -- on ImGuiTableFlags
---@field NoBordersInBody any   -- on ImGuiTableFlags
---@field NoBordersInBodyUntilResize any   -- on ImGuiTableFlags
---@field NoClip any   -- on ImGuiTableFlags
---@field NoHostExtendX any   -- on ImGuiTableFlags
---@field NoHostExtendY any   -- on ImGuiTableFlags
---@field NoKeepColumnsVisible any   -- on ImGuiTableFlags
---@field NoPadInnerX any   -- on ImGuiTableFlags
---@field NoPadOuterX any   -- on ImGuiTableFlags
---@field NoSavedSettings any   -- on ImGuiTableFlags
---@field None any   -- on ImGuiTableFlags
---@field PadOuterX any   -- on ImGuiTableFlags
---@field PreciseWidths any   -- on ImGuiTableFlags
---@field Reorderable any   -- on ImGuiTableFlags
---@field Resizable any   -- on ImGuiTableFlags
---@field RowBg any   -- on ImGuiTableFlags
---@field ScrollX any   -- on ImGuiTableFlags
---@field ScrollY any   -- on ImGuiTableFlags
---@field SizingFixedFit any   -- on ImGuiTableFlags
---@field SizingFixedSame any   -- on ImGuiTableFlags
---@field SizingStretchProp any   -- on ImGuiTableFlags
---@field SizingStretchSame any   -- on ImGuiTableFlags
---@field SortMulti any   -- on ImGuiTableFlags
---@field SortTristate any   -- on ImGuiTableFlags
---@field Sortable any   -- on ImGuiTableFlags

---@class ImGuiTableRowFlags
ImGuiTableRowFlags = {}

---@field Headers any   -- on ImGuiTableRowFlags
---@field None any   -- on ImGuiTableRowFlags

---@class ImGuiTreeNodeFlags
ImGuiTreeNodeFlags = {}

---@field AllowOverlap any   -- on ImGuiTreeNodeFlags
---@field Bullet any   -- on ImGuiTreeNodeFlags
---@field CollapsingHeader any   -- on ImGuiTreeNodeFlags
---@field DefaultOpen any   -- on ImGuiTreeNodeFlags
---@field DrawLinesFull any   -- on ImGuiTreeNodeFlags
---@field DrawLinesNone any   -- on ImGuiTreeNodeFlags
---@field DrawLinesToNodes any   -- on ImGuiTreeNodeFlags
---@field FramePadding any   -- on ImGuiTreeNodeFlags
---@field Framed any   -- on ImGuiTreeNodeFlags
---@field LabelSpanAllColumns any   -- on ImGuiTreeNodeFlags
---@field Leaf any   -- on ImGuiTreeNodeFlags
---@field NavLeftJumpsBackHere any   -- on ImGuiTreeNodeFlags
---@field NavLeftJumpsToParent any   -- on ImGuiTreeNodeFlags
---@field NoAutoOpenOnLog any   -- on ImGuiTreeNodeFlags
---@field NoTreePushOnOpen any   -- on ImGuiTreeNodeFlags
---@field None any   -- on ImGuiTreeNodeFlags
---@field OpenOnArrow any   -- on ImGuiTreeNodeFlags
---@field OpenOnDoubleClick any   -- on ImGuiTreeNodeFlags
---@field Selected any   -- on ImGuiTreeNodeFlags
---@field SpanAllColumns any   -- on ImGuiTreeNodeFlags
---@field SpanAvailWidth any   -- on ImGuiTreeNodeFlags
---@field SpanFullWidth any   -- on ImGuiTreeNodeFlags
---@field SpanLabelWidth any   -- on ImGuiTreeNodeFlags
---@field SpanTextWidth any   -- on ImGuiTreeNodeFlags

---@class ImGuiViewportFlags
ImGuiViewportFlags = {}

---@field CanHostOtherWindows any   -- on ImGuiViewportFlags
---@field IsFocused any   -- on ImGuiViewportFlags
---@field IsMinimized any   -- on ImGuiViewportFlags
---@field IsPlatformMonitor any   -- on ImGuiViewportFlags
---@field IsPlatformWindow any   -- on ImGuiViewportFlags
---@field NoAutoMerge any   -- on ImGuiViewportFlags
---@field NoDecoration any   -- on ImGuiViewportFlags
---@field NoFocusOnAppearing any   -- on ImGuiViewportFlags
---@field NoFocusOnClick any   -- on ImGuiViewportFlags
---@field NoInputs any   -- on ImGuiViewportFlags
---@field NoRendererClear any   -- on ImGuiViewportFlags
---@field NoTaskBarIcon any   -- on ImGuiViewportFlags
---@field None any   -- on ImGuiViewportFlags
---@field OwnedByApp any   -- on ImGuiViewportFlags
---@field TopMost any   -- on ImGuiViewportFlags

---@class ImGuiWindowFlags
ImGuiWindowFlags = {}

---@field AlwaysAutoResize any   -- on ImGuiWindowFlags
---@field AlwaysHorizontalScrollbar any   -- on ImGuiWindowFlags
---@field AlwaysVerticalScrollbar any   -- on ImGuiWindowFlags
---@field HorizontalScrollbar any   -- on ImGuiWindowFlags
---@field MenuBar any   -- on ImGuiWindowFlags
---@field NoBackground any   -- on ImGuiWindowFlags
---@field NoBringToFrontOnFocus any   -- on ImGuiWindowFlags
---@field NoCollapse any   -- on ImGuiWindowFlags
---@field NoDecoration any   -- on ImGuiWindowFlags
---@field NoDocking any   -- on ImGuiWindowFlags
---@field NoFocusOnAppearing any   -- on ImGuiWindowFlags
---@field NoInputs any   -- on ImGuiWindowFlags
---@field NoMouseInputs any   -- on ImGuiWindowFlags
---@field NoMove any   -- on ImGuiWindowFlags
---@field NoNav any   -- on ImGuiWindowFlags
---@field NoNavFocus any   -- on ImGuiWindowFlags
---@field NoNavInputs any   -- on ImGuiWindowFlags
---@field NoResize any   -- on ImGuiWindowFlags
---@field NoSavedSettings any   -- on ImGuiWindowFlags
---@field NoScrollWithMouse any   -- on ImGuiWindowFlags
---@field NoScrollbar any   -- on ImGuiWindowFlags
---@field NoTitleBar any   -- on ImGuiWindowFlags
---@field None any   -- on ImGuiWindowFlags
---@field UnsavedDocument any   -- on ImGuiWindowFlags

---@class ImTextureFormat
ImTextureFormat = {}

---@field _Alpha8 any   -- on ImTextureFormat
---@field _RGBA32 any   -- on ImTextureFormat

---@class ImTextureStatus
ImTextureStatus = {}

---@field _Destroyed any   -- on ImTextureStatus
---@field _OK any   -- on ImTextureStatus
---@field _WantCreate any   -- on ImTextureStatus
---@field _WantDestroy any   -- on ImTextureStatus
---@field _WantUpdates any   -- on ImTextureStatus

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

function registry.get_ui_button(...) end

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

function registry.has_ui_button(...) end


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

function set_position(...) end

function set_sprite_src_rect(...) end

---@class sprite_component
sprite_component = {}


---@class square_primitive_component
square_primitive_component = {}


function stop_scene(...) end

---@class text_label_component
text_label_component = {}


---@class ui_button_component
ui_button_component = {}


---@class vec2
vec2 = {}


