file(GLOB LIB_SOURCE 
	DateTime.cc 
	SimpleLogger.cc 
	strings/strings.cc)
add_library(base SHARED ${LIB_SOURCE})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set_target_properties(base PROPERTIES VERSION 1.0 SOVERSION 1)

add_subdirectory(strings)
add_subdirectory(tests)
