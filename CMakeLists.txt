project(kshutdown)

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt4 4.4.3 REQUIRED QtCore QtGui QtXml QtTest)

# search packages used by KDE
find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)

# The FindKDE4.cmake module sets _KDE4_PLATFORM_DEFINITIONS with
# definitions like _GNU_SOURCE that are needed on each platform.
set(CMAKE_REQUIRED_DEFINITIONS ${_KDE4_PLATFORM_DEFINITIONS} -DQT_STRICT_ITERATORS)

add_definitions (
    ${KDE4_DEFINITIONS}
)

include_directories(
    ${KDE4_INCLUDES}
)

# use kded debug category for now
add_definitions(-DKDE_DEFAULT_DEBUG_AREA=7020)

set (SOURCES
    main.cpp
    shutdownwatcher.cpp
)
kde4_add_app_icon(SOURCES "${KDE4_INSTALL_DIR}/share/icons/oxygen/*/actions/application-exit.png")

kde4_add_executable(kwinshutdown ${SOURCES})

target_link_libraries(kwinshutdown ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS})

########### install ###############

install( TARGETS kwinshutdown DESTINATION  ${BIN_INSTALL_DIR} )
install( PROGRAMS kwinshutdown.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )
