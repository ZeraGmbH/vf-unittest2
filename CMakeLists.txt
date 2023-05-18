cmake_minimum_required(VERSION 3.1)

#set project name and language
project(vf-unittest LANGUAGES CXX)

#Automoc and RCC ON. Needed in QT projects
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#Set project Version
set(VfUnittest_VERSION_MAJOR "0")
set(VfUnittest_VERSION_MINOR "0")
set(VfUnittest_VERSION_PATCH "1")

set(PROJECT_VERSION "${VfUnittest_VERSION_MAJOR}.${VfUnittest_VERSION_MINOR}.${VfUnittest_VERSION_PATCH}")

#Adding cmake functions like feature_summary()
include(FeatureSummary)
include(GNUInstallDirs)


#Find dependecies
find_package(Qt5 COMPONENTS Quick Core Qml Sql Network Test QuickTest CONFIG REQUIRED)
find_package(VfEvent REQUIRED)
find_package(VfHelpers REQUIRED)
find_package(VfComponent REQUIRED)
find_package(VfStorageHash REQUIRED)
find_package(VfQml REQUIRED)
find_package(VfNet2 REQUIRED)
find_package(VfLogger REQUIRED)
find_package(VfTcp REQUIRED)

#set st_main.qml path for qt unittest
add_definitions(-DQUICK_TEST_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

#sum up project Files 
file(GLOB SOURCES 
    ${PROJECT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${PROJECT_SOURCE_DIR}/*.h
    )

file(GLOB RESOURCES 
    ${PROJECT_SOURCE_DIR}/*.qrc
    )

#create executable
add_executable(VfUnittest
    ${SOURCES}
    ${HEADER}
    ${RESOURCES}
    )

#link dependencies
target_link_libraries(VfUnittest 
    PRIVATE
    Qt5::Core
    Qt5::Qml
    Qt5::Sql
    Qt5::Network
    Qt5::Quick
    Qt5::Test
    Qt5::QuickTest 
    VeinMeta::VfEvent
    VeinMeta::VfHelpers
    VeinMeta::VfComponent
    VeinMeta::VfStorageHash
    VeinMeta::VfQml
    VeinMeta::VfNet2
    VeinMeta::VfLogger
    VeinMeta::VfTcp
    )	



#install unittest to bin 
install(TARGETS VfUnittest
    EXPORT VfUnittestExport
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

# spawn out some info on configuration
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)







