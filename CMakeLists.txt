cmake_minimum_required(VERSION 3.15)
project(Backend)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PostgreSQL REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(nlohmann_json REQUIRED)

pkg_check_modules(PQXX REQUIRED IMPORTED_TARGET libpqxx)

# Define las fuentes de tu proyecto
set(SOURCES
    src/main.cpp
    src/coneccionBd.cpp
    src/model/persona.cpp
    src/service/main.service.cpp
    src/service/persona.service.cpp
)

include_directories(include 
                    include/service 
                    include/model
)


add_executable(back_api_c_plus_plus ${SOURCES})

target_link_libraries(back_api_c_plus_plus PUBLIC PkgConfig::PQXX PostgreSQL::PostgreSQL PRIVATE ${nlohmann_json_LIBRARIES} mongoose)