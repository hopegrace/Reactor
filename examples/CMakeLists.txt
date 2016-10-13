set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(simple_proxy simple_proxy.cc)
target_link_libraries(simple_proxy net base)

