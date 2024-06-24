cmake_minimum_required(VERSION 3.5)

# project name init
project(
    snail
	VERSION 0.0.1
    DESCRIPTION "Snail - a small SVG library for simple graphics rendering and experiments."
	LANGUAGES C
)

# setting default build type
set(CMAKE_C_STANDARD 11)
set(default_buid_type "Debug")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)   # for libraries
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin) # for binaries

# finding all headers and sources
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/inc/*.h)

# include directories
include_directories(${PROJECT_SOURCE_DIR}/inc/)
include_directories(${PROJECT_SOURCE_DIR}/third_party/vita/inc) # add vita submodule include directory

# building library/binary
add_subdirectory("third_party/vita") # build vita submodule
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS}) # for libraries
# add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})   # for binaries


