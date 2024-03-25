#include "persona.hpp"
#include <mongoose.h>
#include <iostream>
#include <nlohmann/json.hpp>

class persona_service : private persona
{
private:

public:
    persona_service(/* args */);
    ~persona_service();

    void obtener_lista_persona(struct mg_connection *c);
    void add_perosna(struct mg_connection *c,struct mg_http_message *hm);


    const std::string API_NAME = "/api/usuarios";

};