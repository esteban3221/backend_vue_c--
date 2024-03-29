#include "main.service.hpp"

bool main_service::run = true;

main_service::main_service(const std::string &url, const std::string &port)
{
    std::cout << "\nIniciando servicio rest.\n";
    std::signal(SIGINT, signal_handler);

    mg_log_set(MG_LL_INFO);
    mg_mgr_init(&mgr);
    mg_http_listen(&mgr, ("http://" + url + ":" + port + "").c_str(), fn, this);
    while (run)
        mg_mgr_poll(&mgr, 1000);
}

main_service::~main_service()
{
    std::cout << "\nCerrando servicio rest.\n";
    mg_mgr_free(&mgr);
}

void main_service::signal_handler(int signal)
{
    run = false;
}

void main_service::fn(struct mg_connection *c, int ev, void *ev_data, void *)
{
    if (ev == MG_EV_HTTP_MSG)
    {

        struct mg_http_message *hm = (struct mg_http_message *)ev_data;
        if (mg_strcmp(hm->method, mg_str("OPTIONS")) == 0)
        {
            // cabecera completa para peticiones anteriores a POST
            // Methdo Opcion
            mg_http_reply(
                c, 200,
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
            // std::thread([&]() {
            // personaService.obtener_lista_persona(c);
            // }).detach();

            personaService.obtener_lista_persona(c);
        }

        else if (mg_http_match_uri(hm, (personaService.API_NAME + "/add_usuarios").c_str()))
        // post
        {
            personaService.add_perosna(c, hm);
        }
    }
}