cmake_minimum_required(VERSION 3.25)

project(xatlas)

file(GLOB_RECURSE SRC *.c *.cpp *.cc)
file(GLOB_RECURSE HEADERS *.h *.inl)

set(SOURCES ${SRC} ${HEADERS})

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#target_link_libraries(${PROJECT_NAME} PUBLIC SDL3::SDL3-static ${Vulkan_LIBRARY})
