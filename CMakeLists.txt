cmake_minimum_required(VERSION 3.22)
project(chameleonEsp LANGUAGES C CXX)

add_library(chameleonEsp SHARED
    chameleonEsp/CheatManager.cpp
    chameleonEsp/Compatibility.cpp
    chameleonEsp/Drawings.cpp
    chameleonEsp/Main.cpp
    chameleonEsp/Menu.cpp
    chameleonEsp/Settings.cpp
    chameleonEsp/imgui/imgui.cpp
    chameleonEsp/imgui/imgui_demo.cpp
    chameleonEsp/imgui/imgui_draw.cpp
    chameleonEsp/imgui/imgui_impl_dx12.cpp
    chameleonEsp/imgui/imgui_impl_win32.cpp
    chameleonEsp/imgui/imgui_tables.cpp
    chameleonEsp/imgui/imgui_widgets.cpp
    chameleonEsp/kiero/kiero_d3d12.cpp
    chameleonEsp/kiero/kiero_intern.cpp
    chameleonEsp/minhook/src/buffer.c
    chameleonEsp/minhook/src/hde/hde32.c
    chameleonEsp/minhook/src/hde/hde64.c
    chameleonEsp/minhook/src/hook.c
    chameleonEsp/minhook/src/trampoline.c
    chameleonEsp/SDK/Basic.cpp
    chameleonEsp/SDK/BP_FirstPersonCharacter_cLeon_Character_functions.cpp
    chameleonEsp/SDK/BP_FirstPersonCharacter_cLeon_Character_Hunter_functions.cpp
    chameleonEsp/SDK/BP_FirstPersonPlayerState_Online_cLeon_functions.cpp
    chameleonEsp/SDK/CoreUObject_functions.cpp
    chameleonEsp/SDK/Engine_functions.cpp
)

target_compile_features(chameleonEsp PRIVATE cxx_std_20)
target_compile_definitions(chameleonEsp PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX CHAMELEONESP_EXPORTS _WINDOWS _USRDLL)
target_include_directories(chameleonEsp PRIVATE chameleonEsp chameleonEsp/imgui chameleonEsp/minhook/include)

if(MINGW)
    target_compile_options(chameleonEsp PRIVATE -Wall -Wno-unknown-pragmas -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-function -Wno-missing-field-initializers)
    target_link_options(chameleonEsp PRIVATE -static-libgcc -static-libstdc++ -Wl,--enable-stdcall-fixup)
endif()

target_link_libraries(chameleonEsp PRIVATE d3d12 dxgi dxguid d3dcompiler psapi user32 gdi32 shell32)
set_target_properties(chameleonEsp PROPERTIES
    OUTPUT_NAME chameleonEsp
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
