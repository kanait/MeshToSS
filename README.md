# MeshToSS

MeshToSS is the softwere originally developed by the fund "Software Seeds Promotion Project for Supporting Information Technology" by RISE (Research Institution for Software Engineering), with the commission to IPA (Information-technology Promotion Agency). This software includes an implementation of the following paper:

Takashi Kanai: "MeshToSS: Converting Subdivision Surfaces from Dense Meshes", Proc. 6th International Workshop on Vision, Modeling and Visualization, pp.325-332, IOS Press, Amsterdam, 2001.

**Qt 6 (2026):** A cross-platform **Qt 6** GUI was **newly added in 2026**. It lives under `qt/` in this repository and is described in the [Qt 6 GUI](#qt-6-gui-macos-linux-windows) section below (requirements, and build instructions for macOS, Linux, and Windows).

The original **command-line** tool **`meshtoss`** is built with **CMake** from `optmesh/`; see [Command-line tool (`meshtoss`)](#command-line-tool-meshtoss).

## Features

* Reads polygonal mesh data
* Converts to a Loop subdivision surface
* Outputs a control mesh of a subdivision surface or a subdivided polygonal mesh

## Getting Started

### Qt 6 GUI (macOS, Linux, Windows)

The repository includes a **Qt 6** front end (`qt/`) that shares the mesh and OpenGL drawing code with the original application via `MESHTOSS_QT_GUI`.

| Item | Requirement |
|------|-------------|
| **Qt** | **6.2 or later** (6.x). Used modules: **Core**, **Gui**, **Widgets**, **OpenGL**, **OpenGLWidgets**. |
| **C++** | C++17 |
| **CMake** | 3.16 or later |

Point CMake at your Qt 6 installation with **`CMAKE_PREFIX_PATH`**: the directory that contains **`lib/cmake/Qt6`** (Linux: often the kit root such as `~/Qt/6.x.x/gcc_64`; Windows: e.g. `C:\Qt\6.x.x\msvc2022_64`). If Qt is installed as system packages, CMake may find Qt 6 without setting this.

Build from the **repository root** (the directory that contains the top-level `CMakeLists.txt`).

#### macOS

With [Homebrew](https://brew.sh/) Qt 6 (`qt@6`):

```sh
cmake -B build -DCMAKE_PREFIX_PATH="$(brew --prefix qt@6)"
cmake --build build
```

Run: `build/qt/mesh_to_ss_qt.app/Contents/MacOS/mesh_to_ss_qt`

#### Linux

Install a C++17 compiler, CMake, OpenGL development headers, and Qt 6 (package names vary by distribution).

**Debian / Ubuntu** (example):

```sh
sudo apt install build-essential cmake libgl1-mesa-dev qt6-base-dev
cmake -B build
cmake --build build -j$(nproc)
```

If CMake does not find Qt, set the prefix explicitly (adjust the path if you use the Qt Online Installer):

```sh
cmake -B build -DCMAKE_PREFIX_PATH="$HOME/Qt/6.8.0/gcc_64"
cmake --build build -j$(nproc)
```

Run: `build/qt/mesh_to_ss_qt`

**Fedora** (example): `sudo dnf install cmake gcc-c++ mesa-libGL-devel qt6-qtbase-devel` — then the same `cmake -B build` / `cmake --build` steps as above.

#### Windows

1. Install **Visual Studio** (2019 or newer) with the **Desktop development with C++** workload.
2. Install **Qt 6** for **MSVC 64-bit** (e.g. `msvc2022_64` or `msvc2019_64` matching your compiler).
3. From a **x64** developer shell (or any shell where `cmake` and the compiler are on `PATH`), run (replace the Qt path with yours):

```bat
cmake -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH=C:\Qt\6.8.0\msvc2022_64
cmake --build build --config Release
```

Run: `build\qt\Release\mesh_to_ss_qt.exe`

With **Ninja** instead of Visual Studio generators:

```bat
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=C:\Qt\6.8.0\msvc2022_64
cmake --build build
```

Run: `build\qt\mesh_to_ss_qt.exe`

### Command-line tool (`meshtoss`)

The **`meshtoss`** executable converts polygonal mesh data in **PPD** form (see the usage text printed when you run it without the required arguments). It is produced by the **same top-level CMake** build as the Qt GUI and links the `optmesh/` sources only (**no Qt** and **no OpenGL**).

From the **repository root** (after installing a **C++17** compiler and **CMake 3.16+**):

```sh
cmake -B build
cmake --build build -j$(nproc)
```

Typical output paths:

| Generator | `meshtoss` location |
|-----------|---------------------|
| Unix **Makefile** or **Ninja** | `build/optmesh/meshtoss` |
| Visual Studio (multi-config) | `build\optmesh\Release\meshtoss.exe` (or `Debug`) |

To build **only** `meshtoss` and **skip** the Qt GUI (so **Qt is not required**):

```sh
cmake -B build -DMESHTOSS_BUILD_QT_GUI=OFF
cmake --build build -j$(nproc)
```

**Windows** (Visual Studio generator), building everything including Qt:

```bat
cmake -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH=C:\Qt\6.8.0\msvc2022_64
cmake --build build --config Release
```

Then run `build\optmesh\Release\meshtoss.exe` (and `build\qt\Release\mesh_to_ss_qt.exe` if the Qt GUI was enabled).

### Windows (MFC)

The **MFC** project under `MFC/` is set up for **Visual Studio 2026**. **Only the x64 platform is supported** (Win32 / 32-bit configurations are not maintained). Open **`MFC\MeshToSS.sln`**, pick a configuration such as **Release** | **x64**, and **Build Solution**. When the build succeeds, the executable is typically `MFC\x64\Release\MeshToSS.exe` (exact path depends on your configuration names and output settings).

## Prerequisites

This software uses **MFC** (Microsoft Foundation Class). The **MFC** build targets **Visual Studio 2026** (Windows desktop development with C++ and **MFC** components installed) and **supports x64 only**.

For the **Qt GUI**, install **Qt 6.2+** with the modules listed above and a C++17-capable compiler.

For **`meshtoss`**, only **CMake 3.16+** and a **C++17** compiler are required (use **`-DMESHTOSS_BUILD_QT_GUI=OFF`** if you do not want to install Qt).

## Copyright Holders

2000-2005 IPA (Information-technology Promotion Agency) and Keio University SFC Research Institution

## Developers

* **[Takashi Kanai](https://graphics.c.u-tokyo.ac.jp/hp/en/)** - The University of Tokyo

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

