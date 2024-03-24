
#include <iostream>
#include "main.service.hpp"

// g++ -I./include -I./include/service -I./include/model src/main.cpp src/coneccionBd.cpp src/model/persona.cpp src/service/main.service.cpp -o mai  
//$(pkg-config nlohmann_json --cflags) -lmongoose -lpqxx -lpq

int main(void)
{
    main_service s1;

    return 0;
}