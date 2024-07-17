cmake_minimum_required(VERSION 3.25)
project(Backend)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Crow REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(CURL REQUIRED)
pkg_check_modules(GTKMM gtkmm-4.0)
pkg_check_modules(LIBNOTIFY REQUIRED libnotify)
find_package(SQLite3 REQUIRED)
find_library(SQLITE3_LIBRARY sqlite3 REQUIRED)
find_package(jwt-cpp REQUIRED)

include_directories(
    ${jwt-cpp_INCLUDE_DIRS}
    ${GTKMM_INCLUDE_DIRS}
    ${LIBNOTIFY_INCLUDE_DIRS}
)

set(SOURCES
    src/main.cpp
    src/db/sqlite.cpp
    src/db/wrapbinary.cpp
    #========================================

    src/controller/test.controller.cpp
    src/controller/session.controller.cpp
    #src/controller/base.controller.cpp
    src/controller/configuracion.controller.cpp

    src/controller/accion/venta.controller.cpp
    #========================================
    
    src/view/main.controller_view.cpp
    src/view/nip_view.cpp
    src/view/nip_sett_view.cpp
    src/view/config_view.cpp
    #========================================

    src/view/config/view_one.cpp
    src/view/config/view_two.cpp
    src/view/config/view_three.cpp
    src/view/config/view_four.cpp
    src/view/config/view_five.cpp
    src/view/config/view_six.cpp
    src/view/config/view_seven.cpp

    #========================================

    src/model/usuarios.cpp
    src/model/usuarios_roles.cpp
    src/model/niveles_dinero.cpp
    src/model/config.cpp
    src/model/acciones.cpp

    #========================================
    src/shared/config_text.cpp
    src/shared/helper.cpp
    src/shared/niveles_almacenamiento.cpp
    #========================================
    # ${CMAKE_SOURCE_DIR}/lib/Encryption.c
    # ${CMAKE_SOURCE_DIR}/lib/enctest.c
    # ${CMAKE_SOURCE_DIR}/lib/ITLSSPProc.c
    # ${CMAKE_SOURCE_DIR}/lib/Random.c
    # ${CMAKE_SOURCE_DIR}/lib/serialfunc.c
    # ${CMAKE_SOURCE_DIR}/lib/ssp_commands.c
    # ${CMAKE_SOURCE_DIR}/lib/SSPComs.c
    # ${CMAKE_SOURCE_DIR}/lib/SSPDownload.c
)

include_directories(include 
                    inc/
                    include/controller
                    include/controller/accion
                    include/view
                    include/view/config
                    include/db
                    include/model
                    include/shared
)


add_executable(Maxicajero ${SOURCES})

target_link_libraries(Maxicajero 
                        PUBLIC PRIVATE 
                        ${GTKMM_LIBRARIES} 
                        ${LIBNOTIFY_LIBRARIES}
                        Crow::Crow 
                        SQLite::SQLite3 
                        jwt-cpp::jwt-cpp 
                        CURL::libcurl
)
target_include_directories(Maxicajero PRIVATE ${GTKMM_INCLUDE_DIRS})