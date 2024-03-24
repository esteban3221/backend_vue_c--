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