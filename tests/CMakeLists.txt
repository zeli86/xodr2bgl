
###
set(TARGET RoadLaneSection)

add_executable(${TARGET}
  ${TARGET}.cpp
  ${CMAKE_SOURCE_DIR}/src/xodr2bg/helper.hpp
  ${CMAKE_SOURCE_DIR}/src/xodr2bg/helper.cpp
  ${CMAKE_SOURCE_DIR}/src/xodr2bg/RoadLaneSectionRow.hpp
  ${CMAKE_SOURCE_DIR}/src/xodr2bg/RoadLaneSectionRow.cpp
)

target_include_directories(${TARGET} PRIVATE ${CMAKE_SOURCE_DIR}/src/xodr2bg )
target_link_libraries(${TARGET} Catch2::Catch2WithMain )

catch_discover_tests(${TARGET}
  REPORTER "JUnit::out=${TARGET}-junit.xml" "console::out=-::colour-mode=ansi"
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/test_results
)

###
set(TARGET RoadNet)

add_executable(${TARGET} ${TARGET}.cpp)

add_dependencies(${TARGET} xodr_road_graph-static)

target_include_directories(${TARGET} PRIVATE ${CMAKE_SOURCE_DIR}/src/xodr2bg/include )

target_link_libraries(${TARGET} Catch2::Catch2 xodr_road_graph-static)

catch_discover_tests(${TARGET}
  EXTRA_ARGS "--base_dir" "${CMAKE_SOURCE_DIR}/xodr/" "--build_dir" "${CMAKE_BINARY_DIR}"
  REPORTER "JUnit::out=${TARGET}-junit.xml" "console::out=-::colour-mode=ansi"
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/test_results
)