# MeshToSS

MeshToSS is the softwere originally developed by the fund "Software Seeds Promotion Project for Supporting Information Technology" by RISE (Research Institution for Software Engineering), with the commission to IPA (Information-technology Promotion Agency). This software includes an implementation of the following paper:

Takashi Kanai: "MeshToSS: Converting Subdivision Surfaces from Dense Meshes", Proc. 6th International Workshop on Vision, Modeling and Visualization, pp.325-332, IOS Press, Amsterdam, 2001.

**Qt 6 (2026):** A cross-platform **Qt 6** GUI was **newly added in 2026**. It lives under `qt/` in this repository and is described in the [Qt 6 GUI](#qt-6-gui-macos-linux-windows) section below (requirements, and build instructions for macOS, Linux, and Windows).

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

### Windows (MFC)

Double-click MFC\MeshToSS.sln (A solution file for VS2015) and then "build the solution", and if successfully finished, you can find an executable in MFC\Release\MeshToSS.exe .

### Unix (command-line version)

Type the following commands:

```
./configure
make
```

If sucessfully finished, you can find an executable in optmesh/meshtoss.

## Prerequisites

This software uses MFC (Microsoft Foundation Class). Then, to build this software, VS2015 Professional or upper versions are required for Windows application.

For the **Qt GUI**, install **Qt 6.2+** with the modules listed above and a C++17-capable compiler.

For command-line version, there is no prerequities to build the software.

## Copyright Holders

2000-2005 IPA (Information-technology Promotion Agency) and Keio University SFC Research Institution

## Developers

* **[Takashi Kanai](https://graphics.c.u-tokyo.ac.jp/hp/en/)** - The University of Tokyo

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

