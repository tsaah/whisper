cmake_minimum_required(VERSION 3.19)

project(whisper)

include(variables.cmake)
include(version.cmake)
include(macros.cmake)

add_custom_target(whisper_misc_files SOURCES
    variables.cmake
    macros.cmake
    version.cmake
    version.rc.in
    .gitignore
)

#add_definitions(-DUNICODE)

#if(PLATFORM_WINDOWS)
#    add_definitions(-DWIN32)
#    add_definitions(-D_WIN32)
#endif()

add_subdirectory(src)

