#include "main.service.hpp"


main_service::main_service(/* args */)
{
    mg_log_set(MG_LL_DEBUG);                               // Set log level
    mg_mgr_init(&mgr);                                     // Initialise event manager
    mg_http_listen(&mgr, "http://0.0.0.0:8000", fn, this); // Create HTTP listener
    for (;;)
        mg_mgr_poll(&mgr, 1000); // Infinite event loop
    
}

main_service::~main_service()
{
    mg_mgr_free(&mgr);
}

void main_service::fn(struct mg_connection *c, int ev, void *ev_data, void *)
{
    if (ev == MG_EV_HTTP_MSG)
    {
        
        struct mg_http_message *hm = (struct mg_http_message *)ev_data;
        // get
        if (mg_http_match_uri(hm, (personaService.API_NAME + "/get_usuarios").c_str()))
        {
            personaService.obtener_lista_persona(c);
        }
        
        // else if (mg_http_match_uri(hm, "/api/sum"))
        // // post
        // {
        //     struct mg_str json = hm->body;
        //     double num1, num2;
        //     if (mg_json_get_num(json, "$[0]", &num1) && mg_json_get_num(json, "$[1]", &num2))
        //     {
        //         mg_http_reply(c, 200, "Content-Type: application/json\r\n", "{%m:%g}\n", MG_ESC("result"), num1 + num2);
        //     }
        // }
    }

}