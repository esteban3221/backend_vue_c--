#pragma once
#include <algorithm>
#include <array>
#include <cstdio>
#include <memory>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <crow.h>
#include <crow/middlewares/session.h>
#include <model/usuarios_roles.hpp>

using Session = crow::SessionMiddleware<crow::InMemoryStore>;
namespace Helper
{
    namespace System
    {
        enum class Rol
        {
            Venta = 1,
            Pago,
            Carga,
            Retirada,
            Cambio_M,
            Cambio_A,
            Ingresos,
            Enviar_Casette,
            Retirar_Casette,
            Consulta_Efectivo,
            Mov_Pendientes,
            Consulta_Movimientos,
            Cierre_Faltantes,
            Estadisticas,
            Fianza,
            Mostrar_Reportes,
            Configuracion,
            Salir_Escritorio,
            Apagar_Equipo
        };

        extern std::string userName;
        std::string exec(const char *cmd);
        std::string formatTime(int seconds);
        crow::status validUser(const crow::request &req, Session::context &session);
        crow::status validPermissions(const crow::request &req, Session::context &session, const std::vector<Rol> &vecRol);
    } // namespace System
} // namespace Helper
