set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(MiniProxy MiniProxy.cc)
target_link_libraries(MiniProxy net base)

add_executable(WebServer WebServer.cc)
target_link_libraries(WebServer http net base)

