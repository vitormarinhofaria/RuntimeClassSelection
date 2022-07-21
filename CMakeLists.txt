cmake_minimum_required(VERSION 3.0)

project(RuntimeClassSelection CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(EXECUTABLE_NAME RuntimeClassSelection)
add_executable(${EXECUTABLE_NAME} main.cpp NoVirtual.hpp WithVirtual.hpp Templated.hpp)

#add_subdirectory(optick)
add_subdirectory(entt)
target_link_libraries(${EXECUTABLE_NAME} EnTT::EnTT )
target_include_directories(${EXECUTABLE_NAME} PRIVATE entt/src optick/src)