cmake_minimum_required(VERSION 3.7.0)
project(dinausaurus)


############ default build type ############
set(default_build_type "Debug")
 
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

########### dependencies ####################
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
find_package(sfml REQUIRED)
find_package(Catch2 REQUIRED)

########### modern C++ ######################
set (CMAKE_CXX_STANDARD             17)
set (CMAKE_CXX_STANDARD_REQUIRED    ON)
set (CMAKE_CXX_EXTENSIONS           OFF)

########### code ###########################
add_library(Common  STATIC
    src/ground.cpp 
    src/dino.cpp 
    src/animation.cpp 
    src/cactus.cpp 
    src/game.cpp)
target_link_libraries(Common PUBLIC sfml::sfml stdc++fs)
target_include_directories(Common PUBLIC src)
target_compile_definitions(Common PUBLIC RESOURCES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/resources" )

########### main #############
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME}  Common)

########### unit tests ########################################

find_program(KCOV kcov)
message("kcov: ${KCOV}")
add_executable(test_sfml test/test_sfml.cpp test/test_game.cpp test/main_catch.cpp)
target_link_libraries(test_sfml  Catch2::Catch2 Common)

if( KCOV )
    enable_testing()
    
    add_test(
        NAME dino_coverage 
        COMMAND ${KCOV}  --exclude-pattern=/usr/,${CMAKE_CURRENT_LIST_DIR}/src/fsm --include-path=${CMAKE_CURRENT_LIST_DIR}/src  cov $<TARGET_FILE:test_sfml>  
        )
else()
    message(FATAL_ERROR "dsfsdfsdf")
endif()