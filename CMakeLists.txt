cmake_minimum_required(VERSION 3.16)

# nombre de la app
set(APP "jasflk")

project(${APP} VERSION 1.0)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(MAIN_SRC "src/main")
set(UTILS_SRC "src/utils")
set(CRYPT_SRC "src/crypt")
set(FILES_SRC "src/files")

# direcciones de third party
set(THIRDS_LIB_DIR "${CMAKE_SOURCE_DIR}/thirdparty/lib")
set(THIRDS_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/thirdparty/include")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(LANG_FILES lang/main_es_419.ts)

set(APP_SRCS
        ${MAIN_SRC}/main.cpp
        ${MAIN_SRC}/app.cpp
        ${MAIN_SRC}/app.h
        ${MAIN_SRC}/GestorTask.h
        ${MAIN_SRC}/task.h
        ${MAIN_SRC}/task.cpp
        ${MAIN_SRC}/TaskRun.h
        ${MAIN_SRC}/tasks/UsarArgon.h
        ${MAIN_SRC}/tasks/UsarArgon.cpp
        ${MAIN_SRC}/tasks/Usar7zip.h
        ${MAIN_SRC}/tasks/tasksLoad.h
        ${MAIN_SRC}/proc_signal.h
        ${MAIN_SRC}/proc_signal.cpp

        ${UTILS_SRC}/utils.h
        ${UTILS_SRC}/logging.h
        ${UTILS_SRC}/utils.cpp
        ${FILES_SRC}/gestor_arch.h
        ${FILES_SRC}/gestor_arch.cpp
        ${FILES_SRC}/meta_dat.h
        ${CRYPT_SRC}/argon_wrapper.h
        ${CRYPT_SRC}/argon_wrapper.cpp
        ${LANG_FILES}

        # Dialogs
        ${MAIN_SRC}/dialogs/confirm.h
        ${MAIN_SRC}/dialogs/confirm.cpp
        ${MAIN_SRC}/dialogs/psswrd.h
        ${MAIN_SRC}/dialogs/psswrd.cpp

        # UI
        ui/dialogs/confirm.ui
        ${CMAKE_SOURCE_DIR}/ui/app.ui
        ui/dialogs/psswrd.ui
        ui/icons.qrc

)

# App icon for exe
if (WIN32)
    set(ICON ui/icons/app_icon/app.rc)
else ()
    set(ICON)
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${APP}
        WIN32
        MANUAL_FINALIZATION
        ${APP_SRCS}
        ${ICON}
    )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${LANG_FILES})
endif()

# Establece el Build a Debug si no se especifica
if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("-Build: Debug")
    set(CMAKE_BUILD_TYPE Debug)
    target_compile_definitions(${APP} PRIVATE DEBUG_ON)
endif()

# agrega macro OS_*sistema* y autodetecta el tipo de sistema para seleccionar las librerias necesarias
# medida provisional para cargar librerias externas
if (WIN32)
    target_link_libraries(${APP} PUBLIC
        ${THIRDS_LIB_DIR}/win_x64/libcryptopp.lib
        ${THIRDS_LIB_DIR}/win_x64/libspdlog.lib
        ${THIRDS_LIB_DIR}/win_x64/libargon2.lib
    )
    target_compile_definitions(${APP} PRIVATE OS_WIN)  
elseif (LINUX)
        target_link_libraries(${APP} PUBLIC
        ${THIRDS_LIB_DIR}/linux_x64/libcryptopp.a
        ${THIRDS_LIB_DIR}/linux_x64/libspdlog.a
        ${THIRDS_LIB_DIR}/linux_x64/libargon2.a
    )
    target_compile_definitions(${APP} PRIVATE OS_LINUX)
endif()

target_link_libraries(${APP} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(${APP} PRIVATE supc++)

set_property(TARGET ${APP} PROPERTY AUTOUIC_SEARCH_PATHS 
    "${CMAKE_SOURCE_DIR}/ui"
    "${CMAKE_SOURCE_DIR}/ui/dialogs"
)

target_include_directories(${APP} PUBLIC
    # libreria externa
    ${THIRDS_INCLUDE_DIR}

    ${CMAKE_SOURCE_DIR}/${MAIN_SRC}
    ${CMAKE_SOURCE_DIR}/${CRYPT_SRC}
    ${CMAKE_SOURCE_DIR}/${UTILS_SRC}
    ${CMAKE_SOURCE_DIR}/${FILES_SRC}
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
