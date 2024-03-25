#include "persona.hpp"

persona::persona(/* args */)
{
}

persona::~persona()
{
}

const nlohmann::json persona::getPersonaList()
{
    auto sql = this->connpq->query("select * from usuarios");

    for (const auto &f : sql)
    {
        nlohmann::json fila_json;
        for (const auto &columna : f)
        {
            fila_json[columna.name()] = columna.c_str();
        }
        personaModel.push_back(fila_json);
    }

    return personaModel;
}

void persona::insertaPersona(const nlohmann::json &json)
{

    std::string nombre = json.at("nombre");
    std::string apellido = json.at("apellido");
    std::string edad = json.at("edad");
    std::string correo = json.at("correo");

    std::string sql = "insert into usuarios (nombre, apellido, edad, email) values ("
    "'"+ json["nombre"].get<std::string>() + "',"
    "'"+ json["apellido"].get<std::string>() + "', "
    "'" + json["edad"].get<std::string>() + "', "
    "'" + json["correo"].get<std::string>() + "')";

    std::cout << "\n" << sql << "\n";

    this->connpq->query(sql);
}