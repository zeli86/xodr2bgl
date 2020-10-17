# Copyright 2019 Želimir Marojević
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt
# or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# For more information, see http://www.boost.org

set(TARGET_NAME_SHARED xodr2bgl)
set(TARGET_NAME_STATIC xodr2bgl-static)

set(SRC_FILES 
  stdafx.h
  enums.h
  types.h
  JunctionRow.h
  JunctionRow.cpp
  RoadLaneSectionRow.h
  RoadLaneSectionRow.cpp
  Ixodr2bgl.h
  Ixodr2bgl.cpp
  xodr2bglBuilder.h
  xodr2bglBuilder.cpp
  ../ThirdParty/pugixml-1.10/src/pugiconfig.hpp
  ../ThirdParty/pugixml-1.10/src/pugixml.hpp
  ../ThirdParty/pugixml-1.10/src/pugixml.cpp
)

### Static library
add_library(${TARGET_NAME_STATIC} STATIC ${SRC_FILES} )
target_compile_definitions( ${TARGET_NAME_STATIC} PUBLIC BUILDING_STATIC )

target_compile_options(${TARGET_NAME_STATIC} PRIVATE -Wall)

target_link_libraries( ${TARGET_NAME_STATIC} Boost::headers Boost::graph )

install(TARGETS ${TARGET_NAME_STATIC}
  EXPORT RoadInformation
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  ARCHIVE DESTINATION "${INSTALL_BIN_DIR}" COMPONENT lib
  PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
)

### Shared library
add_library(${TARGET_NAME_SHARED} SHARED ${SRC_FILES} )

set_property(TARGET ${TARGET_NAME_SHARED} PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET ${TARGET_NAME_SHARED} PROPERTY CXX_VISIBILITY_PRESET hidden)
set_property(TARGET ${TARGET_NAME_SHARED} PROPERTY VISIBILITY_INLINES_HIDDEN ON)
target_compile_definitions( ${TARGET_NAME_SHARED} PUBLIC BUILDING_SHARED )
target_compile_options(${TARGET_NAME_SHARED} PRIVATE -Wall)

target_link_libraries( ${TARGET_NAME_SHARED} Boost::headers Boost::graph )

target_link_options(${TARGET_NAME_SHARED} PUBLIC $<$<CXX_COMPILER_ID:MSVC>:/FORCE:MULTIPLE>)
target_link_options(${TARGET_NAME_SHARED} PUBLIC $<$<CXX_COMPILER_ID:GNU>:LINKER:--exclude-libs,ALL>)

install(TARGETS ${TARGET_NAME_SHARED}
  EXPORT RoadInformation
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  ARCHIVE DESTINATION "${INSTALL_BIN_DIR}" COMPONENT lib
  PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
)

### Test program
set(TARGET_TESTPROG testprog)

add_executable( ${TARGET_TESTPROG}
    main.cpp
)

add_dependencies(${TARGET_TESTPROG} ${TARGET_NAME_SHARED})

target_compile_options(${TARGET_TESTPROG} PRIVATE -Wall)

target_link_libraries(${TARGET_TESTPROG} Boost::headers Boost::graph ${TARGET_NAME_STATIC})
target_link_libraries(${TARGET_TESTPROG} Boost::headers Boost::graph ${TARGET_NAME_SHARED})

install(TARGETS ${TARGET_TESTPROG}
  EXPORT RoadInformation
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  ARCHIVE DESTINATION "${INSTALL_BIN_DIR}" COMPONENT lib
  PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
)

unset(${TARGET_NAME_SHARED})
unset(${TARGET_NAME_STATIC})
unset(${TARGET_TESTPROG})
