cmake_minimum_required(VERSION 2.8.9...3.25)
project(news C CXX)

set(CURL 1)
set(UTIL_ONLY 1)
add_subdirectory(carob)

set(EXE news)

get_filename_component(HERE "${CMAKE_CURRENT_LIST_FILE}" PATH)

add_executable(${EXE}
    ${HERE}/src/hn.cpp
    ${HERE}/src/hn.h
    ${HERE}/src/main.cpp
)

add_executable(${EXE}-test
    ${HERE}/src/hn.cpp
    ${HERE}/src/hn.h
    ${HERE}/test/hn.cpp
    ${HERE}/test/main.cpp
)

include_directories(${CAROB_INCLUDE_DIRECTORIES} src)
target_link_libraries(${EXE} cutil)
target_link_libraries(${EXE}-test cutil)

if(NOT ${CMAKE_VERSION} VERSION_LESS 3.8.0)
    get_target_property(EXE_SOURCES ${EXE} SOURCES)
    source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/src
        FILES ${EXE_SOURCES}
    )
endif()
