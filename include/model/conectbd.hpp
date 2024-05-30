#pragma once
#include "sqlite.hpp"
#include <memory>

class connectSqlite
{
private:

public:
    std::shared_ptr<SQLite3::SQLite> sqlite3 = std::make_shared<SQLite3::SQLite>("maxicajero.db");
    connectSqlite(/* args */)
    {
        sqlite3->open();
        if (not this->sqlite3->is_created())
        {
            this->sqlite3->command("CREATE TABLE usuarios ("
                                   "id INTEGER PRIMARY KEY,"
                                   "username TEXT NOT NULL UNIQUE,"
                                   "password TEXT"
                                   ")");
            this->sqlite3->command("CREATE TABLE roles ("
                                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                   "rol TEXT"
                                   ")");
            this->sqlite3->command("CREATE TABLE usuario_roles ("
                                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                   "id_usuario INTEGER,"
                                   "id_rol INTEGER,"
                                   "FOREIGN KEY (id_usuario) REFERENCES usuarios (id),"
                                   "FOREIGN KEY (id_rol) REFERENCES roles (id)"
                                   ")");
            this->sqlite3->command("insert into roles values "
                                   "(NULL,'Venta'),"
                                   "(NULL,'Pago'),"
                                   "(NULL,'Carga'),"
                                   "(NULL,'Retirada'),"
                                   "(NULL,'Cambio M'),"
                                   "(NULL,'Cambio A'),"
                                   "(NULL,'Ingresos'),"
                                   "(NULL,'Enviar a casette'),"
                                   "(NULL,'Retirada a casette'),"
                                   "(NULL,'Consulta de efectivo'),"
                                   "(NULL,'MOv. Pendientes'),"
                                   "(NULL,'Consulta de Movimientos'),"
                                   "(NULL,'Cierre con Faltantes'),"
                                   "(NULL,'Estadisticas'),"
                                   "(NULL,'Fianza'),"
                                   "(NULL,'Mostrar Reportes'),"
                                   "(NULL,'Configuracion'),"
                                   "(NULL,'Salir a Escritorio'),"
                                   "(NULL,'Apagar equipo');");
            this->sqlite3->command("insert into usuarios values (null,'admin','admin');");

            std::string ROL_ADMIN{"INSERT INTO usuario_roles values"};
            for (size_t i = 0; i < 19; i++)
            {
                if (i != 18)
                    ROL_ADMIN += "(NULL,1," + std::to_string(i + 1) + "),\n";
                else
                    ROL_ADMIN += "(NULL,1," + std::to_string(i + 1) + ")";
            }
            this->sqlite3->command(ROL_ADMIN);
        }    
    }
    ~connectSqlite(){}
};

