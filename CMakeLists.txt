PROJECT(rbook_cpp)
#SET(CMAKE_C_COMPILER "/usr/bin/gcc")
#SET(CMAKE_CXX_COMPILER "/usr/bin/g++")
#CMAKE_MINIMUM_REQUIRED(VERSION 3.8)

set(CMAKE_CXX_FLAGS "-std=c++17 -g")

#SET(BUILD_SHARED_LIBS OFF)
include_directories(include)
include_directories(third-party)
add_subdirectory(third-party/yaml-cpp)
#list(APPEND EXTRA_LIBS /ext/yaml-cpp)
#message(${EXTRA_LIBS})

aux_source_directory(src SRC)k
add_executable(main main.cpp ${SRC})
target_link_libraries(main PUBLIC yaml-cpp)
