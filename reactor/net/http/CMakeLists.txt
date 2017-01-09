file(GLOB LIB_SOURCE *.cc)
add_library(http SHARED ${LIB_SOURCE})
set_target_properties(http PROPERTIES VERSION 1.0 SOVERSION 1)
