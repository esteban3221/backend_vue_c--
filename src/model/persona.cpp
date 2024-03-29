#include "persona.hpp"

persona::persona(/* args */)
{
    std::cout << "Model for table Persona init.\n";
}

persona::~persona()
{
    std::cout << "Delete model for table Persona.\n";
}

const nlohmann::json persona::getPersonaList()
{
    personaModel.clear();
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
    if (!json.contains("nombre") || !json.contains("apellido") || !json.contains("edad") || !json.contains("correo") )
        throw std::runtime_error("Formato de json mal formado o con datos faltantes. \n");
    

    std::string sql = "insert into usuarios (nombre, apellido, edad, email) values ("
    "'" + json["nombre"].get<std::string>() + "',"
    "'" + json["apellido"].get<std::string>() + "',"
    "'" + json["edad"].get<std::string>() + "',"
    "'" + json["correo"].get<std::string>() + "')";

    std::cout << "\n" << sql << "\n";

    this->connpq->query(sql);
}