cmake_minimum_required(VERSION 3.27)
project(Backend)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Crow REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM gtkmm-4.0)


set(SOURCES
    src/main.cpp
)

include_directories(include 
                    include/controller
                    include/view
)


add_executable(back_api_c_plus_plus ${SOURCES})

target_link_libraries(back_api_c_plus_plus PUBLIC PRIVATE ${GTKMM_LIBRARIES} Crow::Crow)
target_include_directories(back_api_c_plus_plus PRIVATE ${GTKMM_INCLUDE_DIRS})