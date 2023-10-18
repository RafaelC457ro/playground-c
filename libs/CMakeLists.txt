# create a lib for which we can link to
set(PROJECT_LIBS_DIR ${PROJECT_SOURCE_DIR}/libs)

file(GLOB SOURCES_LIB ${PROJECT_LIBS_DIR}/*.c)
file(GLOB HEADERS_LIB  ${PROJECT_LIBS_DIR}/*.h)

# print sources and headers
message(STATUS "SOURCES_LIB: ${SOURCES_LIB}")
message(STATUS "HEADERS_LIB: ${HEADERS_LIB}")

add_library(mylabc ${SOURCES_LIB} ${HEADERS_LIB})