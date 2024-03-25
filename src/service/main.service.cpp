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
        if (mg_strcmp(hm->method, mg_str("OPTIONS")) == 0)
        {
            //cabecera completa para peticiones anteriores a POST 
            //Methdo Opcion
            mg_http_reply(c, 200,
                          "Access-Control-Allow-Headers: content-type\r\n"
                          "Access-Control-Allow-Methods: POST\r\n"
                          "Access-Control-Allow-Origin: *\r\n"
                          "Access-Control-Expose-Headers: *\r\n"
                          "Cache-Control: no-cache, no-store, max-age=0, must-revalidate\r\n"
                          "Expires: 0\r\n"
                          "Pragma: no-cache\r\n"
                          "Vary: Origin\r\n"
                          "Vary: Access-Control-Request-Method\r\n"
                          "Vary: Access-Control-Request-Headers\r\n"
                          "X-Content-Type-Options: nosniff\r\n"
                          "X-XSS-Protection: 1; mode=block\r\n"
                          "X-Frame-Options: DENY\r\n",
                          "");
        }
        //=============================USUARIOS=================================
        // get
        else if (mg_http_match_uri(hm, (personaService.API_NAME + "/get_usuarios").c_str()))
        {
            personaService.obtener_lista_persona(c);
        }

        else if (mg_http_match_uri(hm, (personaService.API_NAME + "/add_usuarios").c_str()))
        // post
        {
            personaService.add_perosna(c, hm);
        }
    }
}