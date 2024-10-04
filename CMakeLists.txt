cmake_minimum_required(VERSION 3.10)
project(CGame)


include_directories("~/Documents/raylib-5.0_linux_amd64/include")
link_directories("~/Documents/raylib-5.0_linux_amd64/lib")

add_executable(CGame main.c)

target_link_libraries(CGame PRIVATE raylib m)
