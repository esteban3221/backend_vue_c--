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
#include "niveles_almacenamiento.hpp"
#include "niveles_dinero.hpp"
#include <libnotify/notify.h>
#include "wrapbinary.hpp"
#include <future>
#include <curl/curl.h>
#include <atomic>

using Session = crow::SessionMiddleware<crow::InMemoryStore>;
namespace Helper
{
    namespace System
    {
        std::string exec(const char *cmd);
        std::string formatTime(int seconds);
        void showNotify(const char *title, const char *subtitle, const char *type);
    } // namespace System

    namespace User
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

        static const std::vector<Helper::User::Rol> allRoles = {
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
            Rol::Apagar_Equipo};

        crow::status validUser(const crow::request &req, Session::context &session, std::string &userName);
        std::pair<crow::status, std::string> validPermissions(const crow::request &req, Session::context &session, const std::vector<Rol> &vecRol);
    } // namespace User

    // Singleton
    class Validator
    {
    private:
        
        static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);

        std::pair<long, std::string> PostAPIRequest(const std::string &uri, const std::string &deviceID, const std::string &jsonData);

        std::string GetAPIRequest(const std::string &uri, const std::string &deviceID);

        struct Denomination
        {
            int value;
            std::string countryCode;
            bool isInhibited;
            bool isRecyclable;
            int acceptRoute;
            int stored;
        };

        // std::vector<Denomination> parseDenominations(const std::string &jsonData);
        void handleDeviceStatus(const std::string &state, const std::string &deviceID);
        void pollDeviceStatus(const std::string &deviceID);
        void processCommand(const std::string &command, const std::string &deviceID);

    public:

        std::atomic<long> sumInput;
        std::atomic<bool> pollInit;
        const std::string COIN_VALIDATOR;
        const std::string BILL_VALIDATOR;

        std::future<std::pair<long, std::string>> PostAPIRequestAsync(const std::string &uri, const std::string &deviceID, const std::string &jsonData);
        std::future<std::string> GetAPIRequestAsync(const std::string &uri, const std::string &deviceID);

        bool isConected();
        void calculateChange(int changeAmount);
        bool startPay();
        bool stopPay();

        Validator(/* args */);
        ~Validator();
        
    };

} // namespace Helper

namespace Action
{
    enum class Type : unsigned
    {
        VENTA,
        PAGO,
        REFILL,
        TRANSPASO,
        CAMBIO,
        INCOMPLETO,
        PENDIENTE,
        CANCELADO
    };

    static const std::array<std::string, 8> typeVal =
        {{"VENTA",
          "PAGO",
          "REFILL",
          "TRANSPASO",
          "CAMBIO",
          "INCOMPLETO",
          "PENDIENTE",
          "CANCELADO"}};
} // namespace Action