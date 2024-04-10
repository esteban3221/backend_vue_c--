cmake_minimum_required(VERSION 3.25)
project(Backend)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Crow REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM gtkmm-4.0)
find_package(SQLite3 REQUIRED)
find_library(SQLITE3_LIBRARY sqlite3 REQUIRED)


set(SOURCES
    src/main.cpp
    src/db/sqlite.cpp

    src/controller/test.controller.cpp
    src/controller/session.controller.cpp
    
    #src/controller/base.controller.cpp
    src/view/main.controller_view.cpp
    
)

include_directories(include 
                    include/controller
                    include/view
                    include/db
)


add_executable(back_api_c_plus_plus ${SOURCES})

target_link_libraries(back_api_c_plus_plus PUBLIC PRIVATE ${GTKMM_LIBRARIES} Crow::Crow SQLite::SQLite3)
target_include_directories(back_api_c_plus_plus PRIVATE ${GTKMM_INCLUDE_DIRS})