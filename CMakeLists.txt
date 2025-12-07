cmake_minimum_required(VERSION 3.5)

project(notes_app LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_executable(notes_app
    src/main.cpp
    src/note.cpp
    src/mainwindow.cpp
    src/note.hpp
    src/mainwindow.hpp
)

target_link_libraries(notes_app PRIVATE Qt6::Core Qt6::Widgets)