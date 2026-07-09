cmake_minimum_required(VERSION 3.16)

project(GeneticPolynomialSearch VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)


set(PROJECT_SOURCES
    gui/src/main.cpp
    
    gui/form/mainwindow.ui
    gui/headers/mainwindow.h
    gui/src/mainwindow.cpp
    
    core/Polynomial.h
    core/Polynomial.cpp
    core/GeneticAlgorithm.h
    core/GeneticAlgorithm.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/headers
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/gui/form"
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
