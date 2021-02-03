cmake_minimum_required(VERSION 3.17)
project(SEED C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SEED main.c const.h seed.h seed.c kisa_seed.h)

include_directories(Users/dundu/Desktop/libtommath-1.2.0)
find_library(LIBTOMMATH libtommath.a Users/dundu/Desktop/libtommath-1.2.0)

target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBTOMMATH})
