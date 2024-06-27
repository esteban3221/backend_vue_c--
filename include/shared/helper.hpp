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

        static const std::vector<Rol> allRoles = {
            Rol::Venta,
            Rol::Pago,
            Rol::Carga,
            Rol::Retirada,
            Rol::Cambio_M,
            Rol::Cambio_A,
            Rol::Ingresos,
            Rol::Enviar_Casette,
            Rol::Retirar_Casette,
            Rol::Consulta_Efectivo,
            Rol::Mov_Pendientes,
            Rol::Consulta_Movimientos,
            Rol::Cierre_Faltantes,
            Rol::Estadisticas,
            Rol::Fianza,
            Rol::Mostrar_Reportes,
            Rol::Configuracion,
            Rol::Salir_Escritorio,
            Rol::Apagar_Equipo
        };


        std::string exec(const char *cmd);
        std::string formatTime(int seconds);
        crow::status validUser(const crow::request &req, Session::context &session, std::string &userName);
        std::pair<crow::status, std::string> validPermissions(const crow::request &req, Session::context &session, const std::vector<Rol> &vecRol);
    } // namespace System
} // namespace Helper
