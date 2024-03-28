# Copyright 2024 Želimir Marojević
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt
# or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# For more information, see http://www.boost.org

set(TARGET_NAME_SHARED xodr_road_graph)
set(TARGET_NAME_STATIC xodr_road_graph-static)

set(SRC_FILES 
  enums.hpp
  types.hpp
  types.cpp
  color_maps.hpp
  #Dijkstra.hpp
  #Dijkstra.cpp
  dfs.hpp
  dfs.cpp
  JunctionRow.hpp
  JunctionRow.cpp
  RoadLaneSectionRow.hpp
  RoadLaneSectionRow.cpp
  CXodrRoadGraph.cpp
  CXodrRoadGraphImpl.hpp
  CXodrRoadGraphImpl.cpp
  helper.hpp
  helper.cpp
)

### Static library
add_library(${TARGET_NAME_STATIC} STATIC ${SRC_FILES} )
target_compile_definitions( ${TARGET_NAME_STATIC} PUBLIC BUILDING_STATIC )

target_compile_options(${TARGET_NAME_STATIC} PRIVATE -Wall)

target_include_directories(${TARGET_NAME_STATIC} PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries( ${TARGET_NAME_STATIC} Boost::headers Boost::graph  Boost::log_setup Boost::log pugixml::shared)

# install(TARGETS ${TARGET_NAME_STATIC}
#   EXPORT xodr_road_graph
#   RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
#   LIBRARY DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
#   ARCHIVE DESTINATION "${INSTALL_BIN_DIR}" COMPONENT lib
#   PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
# )

### Shared library
add_library(${TARGET_NAME_SHARED} SHARED)
target_sources(${TARGET_NAME_SHARED} PRIVATE ${SRC_FILES})
target_sources(${TARGET_NAME_SHARED} PUBLIC
  FILE_SET HEADERS
  BASE_DIRS include
  FILES include/Xodr2bgFilter.hpp include/CXodrRoadGraph.hpp
)

set_property(TARGET ${TARGET_NAME_SHARED} PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET ${TARGET_NAME_SHARED} PROPERTY CXX_VISIBILITY_PRESET hidden)
set_property(TARGET ${TARGET_NAME_SHARED} PROPERTY VISIBILITY_INLINES_HIDDEN ON)

target_compile_definitions( ${TARGET_NAME_SHARED} PUBLIC SHARED_VARIANT)
target_compile_options(${TARGET_NAME_SHARED} PRIVATE -Wall)

target_link_libraries(${TARGET_NAME_SHARED} PUBLIC Boost::headers Boost::graph Boost::log_setup Boost::log pugixml::shared)

target_link_options(${TARGET_NAME_SHARED} PUBLIC $<$<CXX_COMPILER_ID:MSVC>:/FORCE:MULTIPLE>)
target_link_options(${TARGET_NAME_SHARED} PUBLIC $<$<CXX_COMPILER_ID:GNU>:LINKER:--exclude-libs,ALL>)

target_include_directories(${TARGET_NAME_SHARED} PUBLIC 
  #$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
)
generate_export_header(${TARGET_NAME_SHARED})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/xodr_road_graph_export.h DESTINATION include)

install(TARGETS ${TARGET_NAME_SHARED}
  EXPORT xodr_road_graph
  FILE_SET HEADERS
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  ARCHIVE DESTINATION "${INSTALL_BIN_DIR}" COMPONENT lib
  PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
)

install(EXPORT xodr_road_graph NAMESPACE xodr2bg:: DESTINATION lib)

# install(EXPORT xodr_road_graph
# DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MyProj
# NAMESPACE MyProj::
# FILE MyProjConfig.cmake
# )

