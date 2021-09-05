file(GLOB LIBS_CMAKE_FILES *.cmake)

add_custom_target(libs_misc_files SOURCES
    ${LIBS_CMAKE_FILES}
)

add_subdirectory(libcommon)
add_subdirectory(libserver)
add_subdirectory(libclient)
#add_subdirectory(server)
add_subdirectory(visualserver)
#add_subdirectory(client)
add_subdirectory(simpleclient)
