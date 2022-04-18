cmake_minimum_required(VERSION 3.10)
project(RayTracing)

set(CMAKE_CXX_STANDARD 17)

add_executable(RayTracing vec3.h main.cpp color.h camera.h ray.h utility.h hittable.h sphere.h)
