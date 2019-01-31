# MeshToSS

MeshToSS is the softwere originally developed by the fund "Software Seeds Promotion Project for Supporting Information Technology" by RISE (Research Institution for Software Engineering), with the commission to IPA (Information-technology Promotion Agency). This software includes an implementation of the following paper:

Takashi Kanai: "MeshToSS: Converting Subdivision Surfaces from Dense Meshes", Proc. 6th International Workshop on Vision, Modeling and Visualization, pp.325-332, IOS Press, Amsterdam, 2001.

## Features

* Reads polygonal mesh data
* Converts to a Loop subdivision surface
* Outputs a control mesh of a subdivision surface or a subdivided polygonal mesh

## Getting Started

### Windows

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

For command-line version, there is no prerequities to build the software.

## Copyright Holders

2000-2005 IPA (Information-technology Promotion Agency) and Keio University SFC Research Institution

## Developers

* **[Takashi Kanai](https://graphics.c.u-tokyo.ac.jp/hp/en/)** - The University of Tokyo

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

