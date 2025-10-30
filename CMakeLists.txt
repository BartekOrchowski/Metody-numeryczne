cmake_minimum_required(VERSION 3.16)
project(ShockleyModel)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 COMPONENTS Core Gui Widgets Charts REQUIRED)

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/fitting.cpp
    src/plotwidget.cpp
)

set(HEADERS
    src/mainwindow.h
    src/fitting.h
    src/plotwidget.h
)

set(UI
    ui/mainwindow.ui
)

qt_wrap_ui(UI_HEADERS ${UI})

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${UI_HEADERS})

target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Charts)
