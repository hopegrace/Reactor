file(GLOB LIB_SOURCE *.cc)
add_library(net SHARED ${LIB_SOURCE})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set_target_properties(net PROPERTIES VERSION 1.0 SOVERSION 1)

add_subdirectory(tests)

