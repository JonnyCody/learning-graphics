cmake_minimum_required(VERSION 3.10)
project(Dielectrics)

set(CMAKE_CXX_STANDARD 17)

add_executable(Dielectrics camera.h  color.h hittable.h hittable_list.h main.cpp ray.h rtweekend.h sphere.h vec3.h material.h)
