#include "persona.service.hpp"

persona_service::persona_service(/* args */)
{
}

persona_service::~persona_service()
{
}

void persona_service::obtener_lista_persona(struct mg_connection *c){
     mg_http_reply(c, 200, "Access-Control-Allow-Origin: *\r\nContent-Type: application/json\r\n", getPersonaList().dump().c_str());
}
