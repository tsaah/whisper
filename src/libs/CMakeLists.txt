file(GLOB LIBS_CMAKE_FILES *.cmake)

add_custom_target(libs_misc_files SOURCES
    ${LIBS_CMAKE_FILES}
)

add_subdirectory(common)
add_subdirectory(server)
add_subdirectory(client)

