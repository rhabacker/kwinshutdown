project(kshutdown)

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

set(QT_MIN_VERSION "5.9.0")
set(KF5_MIN_VERSION "5.55.0")

find_package(ECM 1.0.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)
include(ECMAddAppIcon)

# Find Qt modules
find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core    # QCommandLineParser, QStringLiteral
    DBus
    Widgets # QApplication
)

# Find KDE modules
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    CoreAddons      # KAboutData
    I18n            # KLocalizedString
    WidgetsAddons   # KMessageBox
)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

# use kded debug category for now
add_definitions(-DKDE_DEFAULT_DEBUG_AREA=7020)

set (SOURCES
    main.cpp
    shutdownwatcher.cpp
)

#ecm_add_app_icon(SOURCES ICONS "${KDE_INSTALL_DIR}/share/icons/oxygen/*/*/actions/application-exit.png")

add_executable(kwinshutdown ${SOURCES})

set_target_properties(kwinshutdown PROPERTIES OUTPUT_NAME kwinshutdown5)

target_link_libraries(kwinshutdown
    Qt5::DBus
    Qt5::Widgets
    KF5::CoreAddons
    KF5::I18n
    KF5::WidgetsAddons
)

install(TARGETS kwinshutdown ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS kwinshutdown.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
