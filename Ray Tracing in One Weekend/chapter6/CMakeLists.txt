cmake_minimum_required(VERSION 3.10)
project(Ray)

# find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 17)

#include_directories(/usr/local/include)

add_executable(Ray main.cpp hittable.h sphere.h hittable_list.h rtweekend.h)
#target_link_libraries(Rasterizer)
