# Qt6 GUI (macOS / Linux / Windows).
# From repository root (Homebrew Qt 6 example):
#   cmake -B build -DCMAKE_PREFIX_PATH="$(brew --prefix qt@6)"
#   cmake --build build
# Run (macOS): build/qt/mesh_to_ss_qt.app/Contents/MacOS/mesh_to_ss_qt
# Run (Linux etc.): build/qt/mesh_to_ss_qt

find_package(Qt6 6.2 REQUIRED COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

set(MESHTOSS_OPTMESH_SOURCES
  ../optmesh/file.cxx
  ../optmesh/intsec.cxx
  ../optmesh/tricube.cxx
  ../optmesh/matlib.cxx
  ../optmesh/octree.cxx
  ../optmesh/oppd.cxx
  ../optmesh/ppd.cxx
  ../optmesh/ppdedge.cxx
  ../optmesh/ppdface.cxx
  ../optmesh/ppdlist.cxx
  ../optmesh/ppdnormal.cxx
  ../optmesh/ppdpart.cxx
  ../optmesh/ppdsolid.cxx
  ../optmesh/ppdvertex.cxx
  ../optmesh/ps.cxx
  ../optmesh/mytime.cxx
  ../optmesh/veclib.cxx
  ../optmesh/qem.cxx
  ../optmesh/lodprique.cxx
  ../optmesh/star.cxx
  ../optmesh/simpmesh.cxx
  ../optmesh/subdiv.cxx
)

qt_add_executable(mesh_to_ss_qt
  main.cpp
  meshtoss_swin.cpp
  params_globals.cxx
  ../src/draw.cpp
  ../src/glppd.cpp
  ../src/Material.cpp
  ../src/swin.cpp
  ../src/screen.cxx
  ../src/smf.cxx
  ../src/vw.cpp
  ${MESHTOSS_OPTMESH_SOURCES}
)

target_compile_definitions(mesh_to_ss_qt PRIVATE MESHTOSS_QT_GUI=1)
if(APPLE)
  target_compile_definitions(mesh_to_ss_qt PRIVATE GL_SILENCE_DEPRECATION=1)
endif()

target_include_directories(mesh_to_ss_qt PRIVATE
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/optmesh
)

target_link_libraries(mesh_to_ss_qt PRIVATE
  Qt6::Core
  Qt6::Gui
  Qt6::Widgets
  Qt6::OpenGL
  Qt6::OpenGLWidgets
)

# Linux: gluPerspective / gluSphere etc. live in libGLU (macOS bundles GLU with OpenGL.framework).
if(UNIX AND NOT APPLE)
  set(OpenGL_GL_PREFERENCE GLVND)
  find_package(OpenGL REQUIRED)
  target_link_libraries(mesh_to_ss_qt PRIVATE OpenGL::GLU)
endif()

if(APPLE)
  set_target_properties(mesh_to_ss_qt PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER "jp.ac.example.MeshToSSQt"
    MACOSX_BUNDLE_BUNDLE_VERSION "0.1.0"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
  )
endif()
