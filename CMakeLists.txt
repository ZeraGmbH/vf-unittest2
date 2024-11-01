cmake_minimum_required(VERSION 3.1)

project(vf-unittest LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(VfUnittest_VERSION_MAJOR "0")
set(VfUnittest_VERSION_MINOR "0")
set(VfUnittest_VERSION_PATCH "1")
set(PROJECT_VERSION "${VfUnittest_VERSION_MAJOR}.${VfUnittest_VERSION_MINOR}.${VfUnittest_VERSION_PATCH}")

include(FeatureSummary)
include(GNUInstallDirs)

find_package(Qt5 COMPONENTS Quick Core Qml Sql Network Test QuickTest CONFIG REQUIRED)
find_package(VfLogger REQUIRED)

#set st_main.qml path for qt unittest
add_definitions(-DQUICK_TEST_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

file(GLOB SOURCES 
    ${PROJECT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${PROJECT_SOURCE_DIR}/*.h
    )

file(GLOB RESOURCES 
    ${PROJECT_SOURCE_DIR}/*.qrc
    )

add_executable(VfUnittest
    ${SOURCES}
    ${HEADER}
    ${RESOURCES}
    )

target_link_libraries(VfUnittest 
    PRIVATE
    Qt5::Core
    Qt5::Qml
    Qt5::Sql
    Qt5::Network
    Qt5::Quick
    Qt5::Test
    Qt5::QuickTest 
    Zera::VfEvent
    Zera::VfComponent
    Zera::VfStorage
    Zera::VfNet2
    Zera::VfTcp
    Zera::VfQml
    VeinMeta::VfLogger
    )


install(TARGETS VfUnittest
    EXPORT VfUnittestExport
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
