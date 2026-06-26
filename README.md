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

### Ubuntu 22.04 + Steam Proton private/offline testing

These steps build the existing Windows x64 DLL on Ubuntu and prepare a Steam Proton run for **private/offline testing only**. This section does not provide anti-cheat bypasses, stealth, evasion, process hiding, or instructions for disrupting public multiplayer games.

#### 1. Install Ubuntu build dependencies

```bash
sudo apt update
sudo apt install --no-install-recommends \
  build-essential \
  cmake \
  ninja-build \
  mingw-w64 \
  g++-mingw-w64-x86-64 \
  wine64 \
  winetricks
```

`wine64` and `winetricks` are useful for inspecting a local test prefix, but the Steam game itself should still be launched through Steam/Proton.

#### 2. Clone the repository and enter it

```bash
git clone <this-repository-url> chameleonEsp
cd chameleonEsp
```

If you already have the repository checked out, run the following commands from the repository root.

#### 3. Configure the MinGW cross-build

```bash
cmake -S . -B build-mingw -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=cmake-mingw-x86_64.cmake \
  -DCMAKE_BUILD_TYPE=Release
```

This uses `x86_64-w64-mingw32-gcc` and `x86_64-w64-mingw32-g++` from the Ubuntu `mingw-w64` packages. If CMake says those compilers are missing, re-check step 1.

#### 4. Build the Windows DLL

```bash
cmake --build build-mingw
```

The expected output is:

```text
build-mingw/bin/chameleonEsp.dll
```

The existing Windows build files (`chameleonEsp.vcxproj`, `chameleonEsp.slnx`, and `build.bat`) are still kept for Visual Studio/MSBuild users.

#### 5. Find the Steam Proton prefix for the game

Steam stores Proton prefixes under `steamapps/compatdata/<appid>/pfx`. Set `STEAM_COMPAT_DATA_PATH` to the prefix you use for your private/offline test install.

Example layout for a default Steam library:

```bash
export STEAM_LIBRARY="$HOME/.steam/steam/steamapps"
find "$STEAM_LIBRARY/compatdata" -maxdepth 2 -type d -name pfx | sort
```

After identifying the correct prefix, set it explicitly. Replace `<appid>` with the Steam app ID for your local test install:

```bash
export STEAM_COMPAT_DATA_PATH="$STEAM_LIBRARY/compatdata/<appid>"
export WINEPREFIX="$STEAM_COMPAT_DATA_PATH/pfx"
```

#### 6. Create a writable config/log directory

Use a normal Linux directory and expose it to Proton through Wine's `Z:` drive mapping:

```bash
mkdir -p "$HOME/.config/chameleonEsp"
export CHAMELEONESP_CONFIG_DIR="Z:\home\$USER\.config\chameleonEsp"
export CHAMELEONESP_LOG_FILE="Z:\home\$USER\.config\chameleonEsp\chameleonEsp.log"
```

The DLL writes `settings.ini` to `CHAMELEONESP_CONFIG_DIR`. If `CHAMELEONESP_LOG_FILE` is unset, it writes `chameleonEsp.log` in the same config directory. Without overrides, it tries `%APPDATA%\chameleonEsp`, `%USERPROFILE%\chameleonEsp`, a Wine-exposed `$HOME`-style path, and finally `C:\chameleonEsp`.

#### 7. Start the game with Proton and logging enabled

In Steam, open the game's **Properties** -> **Launch Options** and add the environment variables before `%command%`:

```bash
CHAMELEONESP_CONFIG_DIR="Z:\home\$USER\.config\chameleonEsp" CHAMELEONESP_LOG_FILE="Z:\home\$USER\.config\chameleonEsp\chameleonEsp.log" %command%
```

For private/offline tests, launch an offline/local session only. The DLL still needs to be loaded into the Windows game process by your own legitimate private testing workflow; this README intentionally does not include anti-cheat bypass, stealth, evasion, or public-multiplayer abuse instructions.

#### 8. Check diagnostics

After a test run, inspect the log from Ubuntu:

```bash
tail -n 200 "$HOME/.config/chameleonEsp/chameleonEsp.log"
```

Useful log entries include:

- process ID, module base, module path, config path, and Wine detection;
- D3D12 device and swapchain discovery;
- command queue capture;
- resolved Kiero method addresses for `ExecuteCommandLists`, `Present`, and `ResizeBuffers`;
- MinHook status for `ExecuteCommandLists`, `Present`, `ResizeBuffers`, and `ProcessEvent`;
- `GetLastError` / `HRESULT` details for Win32 and D3D12 failures;
- whether `GameThread` was positively identified.

#### 9. Known Proton/vkd3d-proton limitations

- D3D12/DXGI hook discovery can differ between native Windows and vkd3d-proton; check the logged method addresses before assuming a hook problem is game logic related.
- Overlay rendering is skipped for frames where required D3D12 objects are unavailable. Resource/device failures are logged and retried later where safe.
- Unreal object scanning remains game-thread-only. If `GetThreadDescription` is unavailable or `GameThread` cannot be positively found, ESP scanning stays disabled rather than running on a render or worker thread.
- Font loading tries Windows fonts and common Wine/Linux-exposed fonts. If none are available, ImGui's default font is used.
- Proton runtime success is not guaranteed by a successful build; validate only in a private/offline prefix that you control.

Do not use this project in public multiplayer sessions or in ways that degrade other players' experiences.
