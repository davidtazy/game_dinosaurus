cmake_minimum_required(VERSION 3.7.0)
project(dinausaurus)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)



find_package(sfml REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME}  sfml::sfml)

########### unit tests ########################################
find_package(Catch2 REQUIRED)
add_executable(test_sfml test/test_sfml.cpp test/main_catch.cpp)
target_link_libraries(test_sfml  Catch2::Catch2 sfml::sfml)
