cmake_minimum_required(VERSION 3.0.0)
project(osusvtool VERSION 0.1.0)

find_package(wxWidgets REQUIRED COMPONENTS net core base)
if(wxWidgets_USE_FILE) # not defined in CONFIG mode
    include(${wxWidgets_USE_FILE})
endif()

set(SRC_FILES
    SVapp.cpp
    )

add_executable(osusvtool WIN32 ${SRC_FILES})
target_link_libraries(osusvtool ${wxWidgets_LIBRARIES})
