cmake_minimum_required(VERSION 3.21)
project(met_numeryczne LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Gdzie leżą .ui (masz w ./ui)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)

# Qt6
find_package(Qt6 6.2 REQUIRED COMPONENTS Core Gui Widgets Charts)

# Automaty dla Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Pliki
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/fitting.cpp
    src/plotwidget.cpp
    ui/mainwindow.ui
)
set(HEADERS
    src/mainwindow.h
    src/fitting.h
    src/plotwidget.h
)

# Executable (Qt6)
qt_add_executable(met_numeryczne
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(met_numeryczne
    PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Charts
)

# Windows bez konsoli
set_target_properties(met_numeryczne PROPERTIES WIN32_EXECUTABLE TRUE)

qt_finalize_executable(met_numeryczne)
