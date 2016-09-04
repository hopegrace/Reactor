add_executable(test_strings test_strings.cc)
set(EXECUTABLE_OUTPUT_PATH, ${PROJECT_BINARY_DIR}/bin)
link_directories(${PROJECT_BINARY_DIR}/lib)
target_link_libraries(test_strings base gtest)
