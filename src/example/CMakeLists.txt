
set(TARGET demo)

add_executable(${TARGET} ${TARGET}.cpp)

add_dependencies(xodr_road_graph xodr_road_graph-static)

target_compile_options(${TARGET} PRIVATE -Wall)

target_link_libraries(${TARGET} xodr_road_graph-static)

# install(TARGETS ${TARGET}
#   RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
#   LIBRARY DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
#   ARCHIVE DESTINATION "${INSTALL_BIN_DIR}" COMPONENT lib
#   PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
# )
