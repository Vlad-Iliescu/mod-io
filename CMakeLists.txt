cmake_minimum_required(VERSION 3.9)
project(mod_io)

set(CMAKE_CXX_STANDARD 11)

add_executable(mod_io
        i2c.c
        i2c.h
        logger.c
        logger.h
        main.c
        main.h)
