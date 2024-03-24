#include "persona.hpp"
#include <mongoose.h>

class persona_service : private persona
{
private:

public:
    persona_service(/* args */);
    ~persona_service();

    void obtener_lista_persona(struct mg_connection *c);


    const std::string API_NAME = "/api/usuarios";

};