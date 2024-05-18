cmake_minimum_required(VERSION 3.0.0)
project(osusvtool VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(CPM)
CPMAddPackage(
    NAME wxWidgets
    GITHUB_REPOSITORY wxWidgets/wxWidgets
    VERSION 3.2.5
    OPTIONS "wxBUILD_SHARED OFF"
)

find_package(wxWidgets REQUIRED COMPONENTS core base)

file(GLOB_RECURSE SRC_FILES ./src/*.cpp)

add_executable(osusvtool WIN32 ${SRC_FILES})
target_link_libraries(osusvtool PRIVATE wx::core wx::base)

target_include_directories(osusvtool PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)