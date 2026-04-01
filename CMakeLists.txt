cmake_minimum_required(VERSION 4.2)
project(iuptest C)

set(CMAKE_C_STANDARD 11)

include_directories("C:\\Users\\TmkTmk\\CLionProjects\\iup\\include")
link_directories("C:\\Users\\TmkTmk\\CLionProjects\\iup")
link_libraries(iup iupimglib)

add_executable(iuptest main.c)
target_link_libraries(iuptest iup iupimglib)
