# Command-line MeshToSS / optmesh tool (no GUI, no Qt).

set(MESHTOSS_OPTMESH_LIB_SOURCES
  file.cxx
  intsec.cxx
  tricube.cxx
  matlib.cxx
  octree.cxx
  oppd.cxx
  ppd.cxx
  ppdedge.cxx
  ppdface.cxx
  ppdlist.cxx
  ppdnormal.cxx
  ppdpart.cxx
  ppdsolid.cxx
  ppdvertex.cxx
  ps.cxx
  mytime.cxx
  veclib.cxx
  qem.cxx
  lodprique.cxx
  star.cxx
  simpmesh.cxx
  subdiv.cxx
)

add_executable(meshtoss
  meshtoss.cxx
  ${MESHTOSS_OPTMESH_LIB_SOURCES}
)

target_include_directories(meshtoss PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(meshtoss PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)

if(MSVC)
  target_compile_definitions(meshtoss PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
