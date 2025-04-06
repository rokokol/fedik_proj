cmake_minimum_required(VERSION 3.29.6)
project(fedik_proj)

set(CMAKE_CXX_STANDARD 23)
add_compile_options(-mavx)

add_executable(fedik_proj main.cpp
        matrix.h
        lazy_tools.h
        main.cpp
        main.cpp
        main.cpp
        array2d.h
        matrix_lazy.h
        matrix_operations.h
        matrix_operation.h
        fedik_proj.h
        avx_vector.h
        avx_matrix.h
)
