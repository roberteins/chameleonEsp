Currently updated for Game Version: **1.9.0**

Internal extension for Meccha Chameleon

> [!IMPORTANT]
> This is a for fun project. Read the disclaimer below.
>
> I will not be assisting with using this in a way that breaks other players' experiences.

Credits to [Dumper7](https://github.com/Encryqed/Dumper-7), [Unreal-Internal-Base](https://github.com/GLX-ILLUSION/Unreal-Internal-Base) and [imgui](https://github.com/ocornut/imgui)

Inject with your favorite injector, I use [Xenos](https://github.com/darthton/xenos)

### Build

Build with these settings in Visual Studio:

Platform Toolset: `v145 for Microsoft C++ Build Tools`

Window SDK Version: `Windows 10.0 SDK (latest)`

C++ Language Standard option: `/std:c++latest`

### Features

- ESP (Box, Skeleton, Lines, Distance, Name, Role)
- Teleport (works only if you are the host as far as I've tested)
- Character Visibility (view other Survivors in Infection mode)
- No Gun Cooldown (Hunters)
- Anti Detection (Survivors)
- Anti Server Kick
- Magnet (Hunters) -> Press **G** to toggle on/off. This will magnetize survivors in front of the hunter.
- Select Survivor to Kill (Hunters)
- Kill All Survivors (Hunters)
- Fov Changer
- Dump Bones (for debugging)

Use **INSERT** to open the menu, and **END** to unload.

### Media

Videos: https://imgur.com/a/VnzkyA0

<img width="1282" height="752" alt="PenguinHotel-Win64-Shipping_kJkLaVgomX" src="https://github.com/user-attachments/assets/c00a08d4-678e-48e5-be38-5c208dcda308" />

## Disclaimer

This project is intended solely for educational and research purposes. I am not responsible for any misuse, bans or other consequences that may arise from using this software. Use at your own risk.

### Linux-hosted build for private Proton/Wine testing

This path is for building the Windows x64 DLL from Ubuntu 22.04 and loading it only in a private/offline Proton or Wine prefix that you control. It does not add anti-cheat bypasses, stealth, process hiding, evasion, or multiplayer-abuse guidance.

Install the build tools:

```bash
sudo apt update
sudo apt install --no-install-recommends cmake ninja-build mingw-w64 g++-mingw-w64-x86-64
```

Configure and build from the repository root:

```bash
cmake -S . -B build-mingw -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=cmake-mingw-x86_64.cmake \
  -DCMAKE_BUILD_TYPE=Release
cmake --build build-mingw
```

The DLL is written to `build-mingw/bin/chameleonEsp.dll` when the cross-build completes. The existing Visual Studio/MSBuild files (`chameleonEsp.vcxproj`, `chameleonEsp.slnx`, and `build.bat`) are still supported for Windows builds.

Runtime paths and logs:

- Set `CHAMELEONESP_CONFIG_DIR` inside the Wine/Proton environment to choose where `settings.ini` and the default log are written. Example: `CHAMELEONESP_CONFIG_DIR="Z:\\home\\$USER\\.config\\chameleonEsp"`.
- Set `CHAMELEONESP_LOG_FILE` to override only the log file path. If unset, logs are written to `chameleonEsp.log` in the config directory.
- Without overrides, the DLL prefers `%APPDATA%\\chameleonEsp`, then `%USERPROFILE%\\chameleonEsp`, then a Wine-exposed `$HOME`-style path, and finally `C:\chameleonEsp`.

Known Proton/vkd3d-proton limitations:

- D3D12/DXGI hook discovery can differ between native Windows and vkd3d-proton; the DLL logs resolved method addresses for `ExecuteCommandLists`, `Present`, and `ResizeBuffers` so failures can be diagnosed without a visible console.
- Rendering is disabled for the frame if required D3D12 objects are unavailable. Device/resource creation failures are logged and retried on later frames where possible.
- Unreal object scanning remains game-thread-only. If `GetThreadDescription` is unavailable or the `GameThread` name cannot be positively found under Proton, ESP scanning stays disabled and the log explains why instead of scanning from a render/worker thread.
- Font loading tries Windows fonts and common Wine/Linux font paths; if none are available, ImGui's default font is used.

Do not use this project in public multiplayer sessions or in ways that degrade other players' experiences.
