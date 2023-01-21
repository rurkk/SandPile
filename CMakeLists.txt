cmake_minimum_required(VERSION 3.12)

project(
        lab3
        VERSION 1.0
        DESCRIPTION "C++ Year 2022 Lab2"
        LANGUAGES CXX
)

SET(TARGET_H
        image.h
        parser.h
        )

SET(TARGET_SRC
        main.cpp
        image.cpp
        parser.cpp
        )

add_executable(main.cpp image.cpp parser.cpp "main.cpp" parser.h)
