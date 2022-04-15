cmake_minimum_required(VERSION 3.10)
project(MoveableCamera)

set(CMAKE_CXX_STANDARD 17)

add_executable(MoveableCamera camera.h  color.h hittable.h hittable_list.h main.cpp ray.h rtweekend.h sphere.h vec3.h material.h)
