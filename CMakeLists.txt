cmake_minimum_required(VERSION 3.15)
project(MyProject)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define las fuentes de tu proyecto
set(SOURCES
    src/main.cpp
    src/coneccionBd.cpp
    src/model/persona.cpp
    src/service/main.service.cpp
)

# Añade las rutas de inclusión
include_directories(include include/service include/model)

# Añade las bibliotecas necesarias
find_package(nlohmann_json REQUIRED)
find_package(PQXX REQUIRED libpqxx)
find_package(PQ REQUIRED)

# Crea el ejecutable
add_executable(back_api_c_plus_plus ${SOURCES})

# Enlaza las bibliotecas
target_link_libraries(back_api_c_plus_plus PRIVATE ${PQXX_LIBRARIES} ${PQ_LIBRARIES} ${nlohmann_json_LIBRARIES} mongoose)