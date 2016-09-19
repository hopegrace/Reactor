set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
#add_executable(chatclient chatclient.cc)
#target_link_libraries(chatclient base net)
add_executable(chatserver chatserver.cc)
target_link_libraries(chatserver net base)

add_executable(echo echo.cc)
target_link_libraries(echo net base)

add_executable(discard discard.cc)
target_link_libraries(discard net base)


