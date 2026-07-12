cmake_minimum_required(VERSION 3.16)

project(GeneticPolynomialSearch VERSION 1.0 LANGUAGES CXX)

# Настройки Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск Qt
find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    Charts
)

# Исходные файлы
set(PROJECT_SOURCES
    main.cpp

    controller/controller.h
    controller/controller.cpp

    gui/mainwindow.h
    gui/mainwindow.cpp
    gui/mainwindow.ui

    core/Types.h
    core/Polynomial.h
    core/Polynomial.cpp
    core/GeneticAlgorithm.h
    core/GeneticAlgorithm.cpp
)

# Исполняемый файл
add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
)

# Пути для #include
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/core
        ${CMAKE_CURRENT_SOURCE_DIR}/gui
        ${CMAKE_CURRENT_SOURCE_DIR}/controller
)

# Подключение библиотек Qt
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt6::Widgets
        Qt6::Charts
)