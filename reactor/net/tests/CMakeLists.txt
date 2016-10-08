set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
#add_executable(chatclient chatclient.cc)
#target_link_libraries(chatclient base net)
add_executable(chatserver chatserver.cc)
target_link_libraries(chatserver net base)

add_executable(echo echo.cc)
target_link_libraries(echo net base)

add_executable(discard discard.cc)
target_link_libraries(discard net base)

add_executable(test_timer_queue test_timer_queue.cc)
target_link_libraries(test_timer_queue net base)

add_executable(test_loop_timer test_loop_timer.cc)
target_link_libraries(test_loop_timer net base)

add_executable(test_acceptor test_acceptor.cc)
target_link_libraries(test_acceptor net base)

add_executable(test_connector test_connector.cc)
target_link_libraries(test_connector net base)

add_executable(test_tcp_client test_tcp_client.cc)
target_link_libraries(test_tcp_client net base)






