cmake_minimum_required(VERSION 3.16)

# nombre de la app
set(APP "app")

project(${APP} VERSION 1.0)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(MAIN_SRC "src/main")
set(UTILS_SRC "src/utils")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(LANG_FILES lang/main_es_419.ts)

set(APP_SRCS
        ${MAIN_SRC}/main.cpp
        ${MAIN_SRC}/app.cpp
        ${MAIN_SRC}/app.h
        ${UTILS_SRC}/utils.h
        ${UTILS_SRC}/utils.cpp
        ${CMAKE_SOURCE_DIR}/ui/app.ui
        ${LANG_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${APP}
        MANUAL_FINALIZATION
        ${APP_SRCS}
    )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${LANG_FILES})
endif()

# agrega macro OS_*sistema*
if (WIN32)
    target_compile_definitions(${APP} PRIVATE OS_WIN)
elseif (LINUX)
    target_compile_definitions(${APP} PRIVATE OS_LINUX)
endif()

target_link_libraries(${APP} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_property(TARGET ${APP} PROPERTY AUTOUIC_SEARCH_PATHS 
    "${CMAKE_SOURCE_DIR}/ui"
)

target_include_directories(${APP} PUBLIC
    ${CMAKE_SOURCE_DIR}/${MAIN_SRC}
    ${CMAKE_SOURCE_DIR}/${UTILS_SRC}
)

include(GNUInstallDirs)
install(TARGETS ${APP}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${APP})
endif()