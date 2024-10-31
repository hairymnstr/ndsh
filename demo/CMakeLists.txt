cmake_minimum_required(VERSION 3.22)

set(PROJECT_NAME ndsh_demo)

project(${PROJECT_NAME})
add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
                    ${CMAKE_SOURCE_DIR}/ndsh_demo.c
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/.
)

find_package(ndsh REQUIRED PATHS ${CMAKE_SOURCE_DIR}/../)
