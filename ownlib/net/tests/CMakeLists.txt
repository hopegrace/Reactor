add_executable(echo01 echo01.cc)
add_executable(chatserver chatserver.cc)
target_link_libraries(echo01 net)
target_link_libraries(chatserver net)


