#include <mongoose.h>
#include "persona.service.hpp"

class main_service
{
private:
    static void fn(struct mg_connection *c, int ev, void *ev_data, void *);
    struct mg_mgr mgr;
    static inline persona_service personaService;
    
    //otra manera de llamar el api
    // static const char * json (){ 
    //     persona Persona;
    //     return Persona.getPersonaList().dump().c_str();
    //     };

public:
    main_service(/* args */);
    ~main_service();
};
