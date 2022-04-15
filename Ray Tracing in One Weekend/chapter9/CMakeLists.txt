cmake_minimum_required(VERSION 3.10)
project(Metal)

set(CMAKE_CXX_STANDARD 17)


add_executable(Metal camera.h  color.h hittable.h hittable_list.h main.cpp ray.h rtweekend.h sphere.h vec3.h material.h)
