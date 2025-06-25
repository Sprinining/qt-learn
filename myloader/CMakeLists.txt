cmake_minimum_required(VERSION 3.16)

project(myloader VERSION 0.1 LANGUAGES CXX)

# ❌ 不需要自动生成 UI 代码
set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets UiTools) # ✅ 增加 UiTools

set(PROJECT_SOURCES
    main.cpp
    widget.cpp
    widget.h
    widget.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(myloader
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        resources.qrc
    )
else()
    if(ANDROID)
        add_library(myloader SHARED ${PROJECT_SOURCES})
    else()
        add_executable(myloader ${PROJECT_SOURCES})
    endif()
endif()

# ✅ 链接 Qt::UiTools
target_link_libraries(myloader PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::UiTools)
