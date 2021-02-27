cmake_minimum_required(VERSION 3.2)
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

include_directories(. ../../src/)

file(GLOB PROJECT_HEADERS *.h) 
file(GLOB PROJECT_SOURCES *.cpp)

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})

add_executable(
    ${PROJECT_NAME} 
    ${PROJECT_SOURCES} 
)

target_link_libraries(
	${PROJECT_NAME}
	twinkle
)