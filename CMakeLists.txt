cmake_minimum_required(VERSION 3.22)

project(Liner2D)

set(CMAKE_PREFIX_PATH C:\\Qt\\5.15.2\\msvc2019_64\\lib\\cmake)
#======================= INCLUSION OF Qt =======================#
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH $ENV{QTDIR})
find_package(OpenGL)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5Widgets REQUIRED)

#======================= INCLUSION OF Our Code ======================#
set(FORMS_DIR "${CMAKE_SOURCE_DIR}/forms")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

include_directories(${FORMS_DIR})
include_directories(${INCLUDE_DIR})
include_directories(${SOURCE_DIR})

file(GLOB_RECURSE SOURCES
        "${FORMS_DIR}/*.ui"
        "${INCLUDE_DIR}/*.h"
        "${SOURCE_DIR}/*.cpp"
        )

# Add the forms directory to the AUTOUIC search paths
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} ${FORMS_DIR})

# Add the executable
if (WIN32)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_executable(Liner2D main.cpp ${SOURCES})
    else()
        add_executable(Liner2D WIN32 main.cpp ${SOURCES})
    endif ()

elseif(UNIX)
    add_executable(Liner2D main.cpp ${SOURCES})
endif()

# Add the target includes
target_include_directories(Liner2D PRIVATE ${FORMS_DIR})
target_include_directories(Liner2D PRIVATE ${INCLUDE_DIR})
target_include_directories(Liner2D PRIVATE ${SOURCE_DIR})

#===================== LINKING LIBRARIES =======================#
target_link_libraries( Liner2D Qt5::OpenGL)
target_link_libraries( Liner2D Qt5::Xml)
target_link_libraries( Liner2D Qt5::Widgets)
target_link_libraries( Liner2D ${QT_LIBRARIES} ${OPENGL_LIBRARIES})