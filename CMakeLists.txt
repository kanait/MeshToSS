cmake_minimum_required(VERSION 3.16)
project(MeshToSS LANGUAGES CXX)

option(MESHTOSS_BUILD_QT_GUI "Build the Qt 6 GUI (mesh_to_ss_qt)" ON)

add_subdirectory(optmesh)
if(MESHTOSS_BUILD_QT_GUI)
  add_subdirectory(qt)
endif()
