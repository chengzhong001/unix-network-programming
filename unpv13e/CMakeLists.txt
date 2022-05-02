cmake_minimum_required(VERSION 3.16)
project(introduction LANGUAGES C)
set(CMAKE_C_STANDARD 11)


add_executable(daytimetcpcli daytimetcpcli.c error.c)
add_executable(daytimetcpcli6 daytimetcpcliv6.c error.c)
add_executable(daytimetcpsrv daytimetcpsrv.c wrapsock.c wrapunix.c error.c)


