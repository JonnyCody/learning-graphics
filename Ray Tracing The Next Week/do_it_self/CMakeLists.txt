cmake_minimum_required(VERSION 3.10)
project(RayTracing)

set(CMAKE_CXX_STANDARD 17)

add_executable(RayTracing aabb.h bvh.h vec3.h main.cpp color.h camera.h ray.h utility.h hittable.h
        sphere.h hittable_list.h material.h moving_sphere.h texture.h stb_image.h aarect.h box.h constant_medium.h)
