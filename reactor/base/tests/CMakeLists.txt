add_executable(test_path test_path.cc)
link_directories(${PROJECT_BINARY_DIR}/lib)
target_link_libraries(test_path base gtest)
