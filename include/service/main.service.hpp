#pragma once
#include <mongoose.h>
#include "persona.service.hpp"
#include <thread>
#include <csignal>

class main_service
{
private:
    static void fn(struct mg_connection *c, int ev, void *ev_data, void *);
    struct mg_mgr mgr;
    static inline persona_service personaService;

    static void signal_handler(int signal);

    // otra manera de llamar el api
    //  static const char * json (){
    //      persona Persona;
    //      return Persona.getPersonaList().dump().c_str();
    //      };

public:
    static bool run;
    main_service(const std::string &url = "0.0.0.0", const std::string &port = "8000");
    ~main_service();
};
