set(TIMESTAMP YEAR "%Y")
#set(TARGET_ICON_PATH "whisper.ico")




set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt5 COMPONENTS
    Core
    Gui
    Network
    Qml
    Quick
#    Quickcontrols2
    REQUIRED
)

set(QT_LIBS
    Qt5::Core
    Qt5::Gui
    Qt5::Network
    Qt5::Qml
    Qt5::Quick
#    Qt5::Quickcontrols2
)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#execute_process(COMMAND git describe --always OUTPUT_VARIABLE GIT_VERSION_DESCRIPTION OUTPUT_STRIP_TRAILING_WHITESPACE)
#message("GIT_VERSION_DESCRIPTION: ${GIT_VERSION_DESCRIPTION}")

set(libPrefix "lib")
set(SRCDIR "${CMAKE_SOURCE_DIR}")


