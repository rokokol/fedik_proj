cmake_minimum_required(VERSION 3.29.6)
project(fedik_proj)

set(CMAKE_CXX_STANDARD 23)


add_executable(fedik_proj main.cpp
        matrix.h
        fedik_proj.cpp
        fedik_proj.h
        vector.h
        vector_lazy.h
        lazy_vector_meta.h
        lazy_tools.h
        main.cpp
        main.cpp
        main.cpp
        array2d.h
        array2d_meta.h
)
