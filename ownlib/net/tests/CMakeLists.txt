#add_executable(echo01 echo01.cc)
#target_link_libraries(echo01 base net)
#add_executable(chatclient chatclient.cc)
#target_link_libraries(chatclient base net)
add_executable(chatserver chatserver.cc)
target_link_libraries(chatserver base net)


