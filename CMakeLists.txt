cmake_minimum_required(VERSION 4.1.2)
project(iuptest C)


set(CMAKE_C_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
include_directories(lib/iup/include)
link_directories(lib/iup)
link_libraries(iup iupimglib)

add_executable(iuptest main.c etc/iup.rc
        fmgr_win.c
        fmgr_win.h)
target_link_libraries(iuptest iup iupimglib)
