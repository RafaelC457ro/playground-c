cmake_minimum_required(VERSION 3.16)

project(cplayground VERSION 1.0.0 LANGUAGES C)

# target to C99
set(CMAKE_C_STANDARD 99)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(PROJECT_SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(PROJECT_LIBS_DIR ${PROJECT_SOURCE_DIR}/libs)

# The following folder will be included
include_directories(PROJECT_SOURCE_DIR)
include_directories(${PROJECT_LIBS_DIR})
add_subdirectory(${PROJECT_LIBS_DIR})

find_package(SDL2 CONFIG REQUIRED)
find_package(FFMPEG REQUIRED)
   
file(GLOB SOURCES ${PROJECT_SRC_DIR}/*.c)
file(GLOB HEADERS  ${PROJECT_SRC_DIR}/*.h) 
file(GLOB HEADERS_LIB  ${PROJECT_LIBS_DIR}/*.h)

foreach(SRC_FILE ${SOURCES})
    # Get the relative path of the source file inside the "src" directory
    file(RELATIVE_PATH RELATIVE_PATH_SRC ${CMAKE_CURRENT_SOURCE_DIR} ${SRC_FILE})
    
    # Extract the filename without the extension
    get_filename_component(EXECUTABLE_NAME ${RELATIVE_PATH_SRC} NAME_WE)

    # Create an executable for each C source file
    add_executable(${EXECUTABLE_NAME} ${SRC_FILE} ${HEADERS} ${HEADERS_LIB})
    # target link libraries
    target_link_libraries(${EXECUTABLE_NAME} mylabc SDL2::SDL2main SDL2::SDL2-static ${FFMPEG_LIBRARIES})
    target_include_directories(${EXECUTABLE_NAME} PRIVATE ${FFMPEG_INCLUDE_DIRS})
    target_link_directories(${EXECUTABLE_NAME} PRIVATE ${FFMPEG_LIBRARY_DIRS})
endforeach()






