#pragma once
#include "binaryfile.hpp"
#include <type_traits>
#include <variant>

namespace BinaryDB
{

    // archivo binario local
    // opt/maxicajero/src/data
    /*
    id      desc

    1       nip
    2       razon social
    3       direccion
    4       rfc
    5       Contacto
    6       Agradecimiento
    7       status impresion
    8       status inalambrica
    9       Mostrar Notificaciones
    10      Mostrar Mensaje de agradeciemiento
    11      Mostrar fecha
    12      Mostrar direccion
    13      Mostrar rfc
    14      Mostrar vendedor
    15      Mostrar contacto

    16      Ruta Logo Inicio
    17      Mensaje de inicio
    18      Ruta de Carrousel de Img.
    19      Temporizador de carrousel
    */
    extern std::binaryfile BDB;

    inline void init_()
    {
        BDB.clear();
        BDB.adding_string("123456"); // nip de fabrica
        BDB.adding_string("");       // razon social
        BDB.adding_string("");       // direccion
        BDB.adding_string("");       // rfc
        BDB.adding_string("");       // Contacto
        BDB.adding_string("");       // Agradecimiento

        BDB.adding_number<int>(true); // status impresion
        BDB.adding_number<int>(true); // status inalambrica
        BDB.adding_number<int>(true); // Mostrar Notificaciones

        BDB.adding_number<int>(true); // Mostrar Mensaje de agradeciemiento
        BDB.adding_number<int>(true); // Mostrar fecha
        BDB.adding_number<int>(true); // Mostrar direccion
        BDB.adding_number<int>(true); // Mostrar rfc
        BDB.adding_number<int>(true); // Mostrar vendedor
        BDB.adding_number<int>(true); // Mostrar contacto

        BDB.adding_string("");           // Ruta Logo Inicio
        BDB.adding_string("Bienvenido"); // Mensaje de inicio
        BDB.adding_string("");           // Ruta de Carrousel de Img.
        BDB.adding_number<int>(5);       // Temporizador de carrousel

        BDB.save();
        std::cout << "Se escribio archivo binario" << std::endl;
    }

    template <typename T>
    inline void insert_(const int &id, const T &data)
    {

        if (std::is_same<T, int>::value || std::is_same<T, float>::value)
            BDB.adding_number<T>(id, data);
        else if (std::is_same<T, std::string>::value)
            BDB.adding_string(id, data);
        else
            throw "T no es ni un entero ni una cadena de caracteres.";

        BDB.save();
    }

    template <typename T>
    inline void update_(const int &id, const T &data)
    {
        if constexpr (std::is_same_v<T, int> || std::is_same_v<T, float> || std::is_same_v<T, bool>)
            BDB.update_number<T>(id, data);
        else if constexpr (std::is_same_v<T, std::string>)
            BDB.update_string(id, data);
        else
            throw "T no es ni un entero/flotante ni una cadena de caracteres.";
        BDB.save();
    }

    template <typename T>
    inline T select_number_(const int &id)
    {
        return BDB.get_number<T>(id);
    }

    inline std::string select_string_(const int &id)
    {
        return BDB.get_string(id);
    }

} // namespace BinaryDB
