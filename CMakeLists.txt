cmake_minimum_required(VERSION 3.0.0)
project(osusvtool VERSION 0.1.0)

find_package(wxWidgets REQUIRED COMPONENTS net core base)
if(wxWidgets_USE_FILE) # not defined in CONFIG mode
    include(${wxWidgets_USE_FILE})
endif()

file(GLOB_RECURSE SRC_FILES ./src/*.cpp)

add_executable(osusvtool WIN32 ${SRC_FILES})
target_link_libraries(osusvtool ${wxWidgets_LIBRARIES})

target_include_directories(osusvtool PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)