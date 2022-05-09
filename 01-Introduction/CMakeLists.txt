cmake_minimum_required(VERSION 3.16)
project(introduction LANGUAGES C)
set(CMAKE_C_STANDARD 11)

add_executable(daytimetcpcli daytimetcpcli.c)
add_executable(daytimetcpsrv daytimetcpsrv.c)
add_executable(daytimetcpcliv6 daytimetcpcliv6.c)
add_executable(daytimetcpsrvv6 daytimetcpsrvv6.c)