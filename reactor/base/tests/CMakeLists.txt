add_executable(test_datetime test_datetime.cc)
set(EXECUTABLE_OUTPUT_PATH, ${PROJECT_BINARY_DIR}/bin)
link_directories(${PROJECT_BINARY_DIR}/lib)
target_link_libraries(test_datetime base gtest)
