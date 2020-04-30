cmake_minimum_required(VERSION 3.7.0)
project(dinausaurus)

########### dependencies ####################
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
find_package(sfml REQUIRED)
find_package(Catch2 REQUIRED)

########### modern C++ ######################
set (CMAKE_CXX_STANDARD             17)
set (CMAKE_CXX_STANDARD_REQUIRED    ON)
set (CMAKE_CXX_EXTENSIONS           OFF)




add_library(Common src/ground.cpp src/dino.cpp src/animation.cpp src/cactus.cpp)
target_link_libraries(Common PUBLIC sfml::sfml stdc++fs)
target_include_directories(Common PUBLIC src)
target_compile_definitions(Common PUBLIC RESOURCES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/resources" )

########### main #############
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME}  Common)

########### unit tests ########################################
add_executable(test_sfml test/test_sfml.cpp test/main_catch.cpp)
target_link_libraries(test_sfml  Catch2::Catch2 Common)