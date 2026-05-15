#pragma once

#ifdef OCTARINE_WITH_EDITOR

namespace octarine::editor::layouts {

// Bundled default layout, applied on first launch when no imgui.ini exists.
// To regenerate: arrange the editor as desired, "Layout -> Save Current As..."
// to write {prefdir}/layouts/<name>.layout, then paste that file's contents
// into the raw-string literal below.
inline constexpr const char* kDefaultLayoutContents = R"OCTARINE_LAYOUT(showSceneWindow=true
showSceneManagement=true
showHierarchy=true
showAssetBrowser=true
showLuaConsole=true
showProfiler=false
[imgui]
[Window][Debug##Default]
Pos=60,60
Size=593,593
Collapsed=0

[Window][FPS]
Pos=1190,364
Size=419,144
Collapsed=0

[Window][Entity Info]
Pos=1024,74
Size=142,48
Collapsed=0

[Window][WindowOverViewport_11111111]
Pos=0,38
Size=1920,1042
Collapsed=0

[Window][Engine Options]
Pos=1330,38
Size=590,794
Collapsed=0
DockId=0x00000003,1

[Window][Scene]
Pos=412,24
Size=1085,648
Collapsed=0
DockId=0x0000000A,0

[Window][Mode Selector]
Pos=406,58
Size=565,1056
Collapsed=0

[Window][Spawn Enemy]
Pos=1901,24
Size=421,1334
Collapsed=0

[Window][Asset Browser]
Pos=0,755
Size=1327,325
Collapsed=0
DockId=0x00000009,1

[Window][Performance Profiler]
Pos=0,755
Size=1327,325
Collapsed=0
DockId=0x00000009,1

[Window][Hierarchy / Entity Inspector]
Pos=0,38
Size=516,714
Collapsed=0
DockId=0x00000008,0

[Window][Editor Settings]
Pos=1330,38
Size=590,794
Collapsed=0
DockId=0x00000003,0

[Window][Lua Console]
Pos=0,755
Size=1327,325
Collapsed=0
DockId=0x00000009,0

[Window][Scene View]
Pos=519,38
Size=808,714
Collapsed=0
DockId=0x0000000A,0

[Window][Scene Management]
Pos=1330,835
Size=590,245
Collapsed=0
DockId=0x00000004,0

[Window][Project Selector]
Pos=60,60
Size=400,240
Collapsed=0

[Window][Restart Required]
Pos=60,60
Size=16,40
Collapsed=0

[Window][Performance]
Pos=1279,68
Size=197,78
Collapsed=0

[Window][Hierarchy]
Pos=0,24
Size=410,648
Collapsed=0
DockId=0x00000005,0

[Window][Inspector]
Pos=0,24
Size=410,648
Collapsed=0
DockId=0x00000005,1

[Window][Save Layout Preset]
Pos=817,448
Size=286,184
Collapsed=0

[Docking][Data]
DockSpace         ID=0x08BD597D Window=0x1BBC0F80 Pos=0,38 Size=1920,1042 Split=X
  DockNode        ID=0x00000001 Parent=0x08BD597D SizeRef=1327,1042 Split=X
    DockNode      ID=0x00000005 Parent=0x00000001 SizeRef=410,648 Selected=0xBABDAE5E
    DockNode      ID=0x00000006 Parent=0x00000001 SizeRef=1085,648 Split=Y Selected=0x15E43931
      DockNode    ID=0x00000007 Parent=0x00000006 SizeRef=1327,331 Split=X Selected=0x15E43931
        DockNode  ID=0x00000008 Parent=0x00000007 SizeRef=516,714 Selected=0x18C28BF1
        DockNode  ID=0x0000000A Parent=0x00000007 SizeRef=808,714 CentralNode=1 Selected=0x15E43931
      DockNode    ID=0x00000009 Parent=0x00000006 SizeRef=1327,325 Selected=0x9F0C8275
  DockNode        ID=0x00000002 Parent=0x08BD597D SizeRef=590,1042 Split=Y Selected=0x684E5796
    DockNode      ID=0x00000003 Parent=0x00000002 SizeRef=590,794 Selected=0x19B88ACB
    DockNode      ID=0x00000004 Parent=0x00000002 SizeRef=590,245 Selected=0xDBCE772C
)OCTARINE_LAYOUT";

}  // namespace octarine::editor::layouts

#endif  // OCTARINE_WITH_EDITOR
