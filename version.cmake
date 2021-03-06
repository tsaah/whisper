execute_process(COMMAND git branch --show-current WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}" OUTPUT_VARIABLE GIT_BRANCH OUTPUT_STRIP_TRAILING_WHITESPACE )
message("GIT_BRANCH: ${GIT_BRANCH}")

execute_process(COMMAND git describe --always --tags --candidates 1 --long WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}" OUTPUT_VARIABLE GIT_DESCRIPTION OUTPUT_STRIP_TRAILING_WHITESPACE )

execute_process(COMMAND git rev-parse HEAD WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}" OUTPUT_VARIABLE GIT_COMMIT_ID OUTPUT_STRIP_TRAILING_WHITESPACE )
message("GIT_COMMIT_ID: ${GIT_COMMIT_ID}")


string(REGEX MATCH "([A-Z0-9_\.]+)-([0-9]+)-[a-z0-9]+" MATCH_OUTPUT "${GIT_DESCRIPTION}")
set(GIT_TAG ${CMAKE_MATCH_1})
message("GIT_TAG: ${GIT_TAG}")

set(GIT_TAG_DISTANCE ${CMAKE_MATCH_2})
message("GIT_TAG_DISTANCE: ${GIT_TAG_DISTANCE}")

string(REGEX MATCH "([A-Z]+)_([0-9]+)\.([0-9]+)\.([0-9]+)_?([A-Z0-9]+)?" MATCH_OUTPUT "${GIT_TAG}")
set(GIT_TAG_PREFIX "${CMAKE_MATCH_1}")
message("GIT_TAG_PREFIX: ${GIT_TAG_PREFIX}")

set(TARGET_VERSION_MAJOR "${CMAKE_MATCH_2}")
set(TARGET_VERSION_MINOR "${CMAKE_MATCH_3}")
set(TARGET_VERSION_PATCH "${CMAKE_MATCH_4}")
set(TARGET_VERSION_BUILD ${GIT_TAG_DISTANCE})


set(GIT_TAG_VERSION_SUFFIX "${CMAKE_MATCH_5}")
message("GIT_TAG_VERSION_SUFFIX: ${GIT_TAG_VERSION_SUFFIX}")

set(VERSION "${TARGET_VERSION_MAJOR}.${TARGET_VERSION_MINOR}.${TARGET_VERSION_PATCH}.${TARGET_VERSION_BUILD}")

if(GIT_BRANCH MATCHES "^release/")
    set(IS_RELEASE "release")
else()
    set(IS_RELEASE "not-release")
endif()

string(TIMESTAMP YEAR "%Y")
string(TIMESTAMP CMAKE_TIMESTAMP "%d-%m-%Y %H:%M:%S")
message("CMAKE_TIMESTAMP: ${CMAKE_TIMESTAMP}")



set(TARGET_COMPANY "Whisper inc")
set(TARGET_PRODUCT "Whisper ${GIT_TAG} ${IS_RELEASE} ${CMAKE_TIMESTAMP} ${GIT_COMMIT_ID}")
set(TARGET_DESCRIPTION "Yet another messenger.")
set(TARGET_COPYRIGHT "(c)${YEAR} Whisper inc")
set(TARGET_VERSION_REPO "Whisper")
set(TARGET_ICON_PATH "${SRCDIR}/whisper.ico")
set(TARGET_VERSION_REPO "")

message("VERSION: ${VERSION}")

set(TARGET_PRODUCT_VERSION_MAJOR "${TARGET_VERSION_MAJOR}")
set(TARGET_PRODUCT_VERSION_MINOR "${TARGET_VERSION_MINOR}")
set(TARGET_PRODUCT_VERSION_PATCH "${TARGET_VERSION_PATCH}")
set(TARGET_PRODUCT_VERSION_BUILD "${TARGET_VERSION_BUILD}")





