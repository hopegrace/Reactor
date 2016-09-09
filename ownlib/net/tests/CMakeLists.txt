add_executable(echo01 echo01.cc)
add_executable(chatserver chatserver.cc)
add_executable(chatclient chatclient.cc)
add_executable(chatserver-poll chatserver-poll.cc)
target_link_libraries(echo01 base net)
target_link_libraries(chatserver base net)
target_link_libraries(chatclient base net)
target_link_libraries(chatserver-poll base net)


