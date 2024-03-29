#pragma once
#include <nlohmann/json.hpp>
#include <memory>
#include "coneccionBd.hpp"

class persona
{
private:
    nlohmann::json  personaModel;
    std::unique_ptr<coneccionBd> connpq = std::make_unique<coneccionBd>("test", "admin", "junomava3842");
public:
    persona(/* args */);
    ~persona();
    const nlohmann::json getPersonaList();
    void insertaPersona(const nlohmann::json &);
    // virtual void eliminaPersona(int ) = 0;
    // virtual void actualizaPersona(int ) = 0;
};

