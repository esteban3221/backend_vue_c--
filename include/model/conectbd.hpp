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
        this->sqlite3->open();
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

            this->sqlite3->command("CREATE TABLE log (Id INT PRIMARY KEY, IdUser INT, Tipo text, Ingreso real, Cambio real, Total real, Estatus text, Fecha text , FOREIGN KEY (IdUser) REFERENCES usuarios (id))");
            this->sqlite3->command("CREATE TABLE pagoPendiente (Id INT PRIMARY KEY, IdLog int, Remanente real, Estatus text, FOREIGN KEY (IdLog) REFERENCES log (Id))");

            this->sqlite3->command("CREATE TABLE IF NOT EXISTS usuarios_historial ("
                                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                   "original_id INTEGER NOT NULL,"
                                   "old_username VARCHAR(255),"
                                   "old_password VARCHAR(255),"
                                   "new_username VARCHAR(255),"
                                   "new_password VARCHAR(255),"
                                   "fecha_modificacion TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
                                   "usuario_operacion VARCHAR(255),"
                                   "tipo_cambio VARCHAR(10) -- 'INSERT', 'UPDATE', 'DELETE'"
                                   ");");

            this->sqlite3->command("CREATE TRIGGER IF NOT EXISTS after_usuarios_insert"
                                   "AFTER INSERT ON usuarios"
                                   "FOR EACH ROW"
                                   "BEGIN"
                                   "INSERT INTO usuarios_historial ("
                                   "original_id,"
                                   "new_username,"
                                   "new_password,"
                                   "usuario_operacion,"
                                   "tipo_cambio"
                                   ") VALUES ("
                                   "NEW.id,"
                                   "NEW.username,"
                                   "NEW.password,"
                                   "'system_user', "
                                   " 'INSERT'"
                                   ");"
                                   "END;");
            this->sqlite3->command("CREATE TRIGGER IF NOT EXISTS after_usuarios_update"
                                   "AFTER UPDATE ON usuarios"
                                   "FOR EACH ROW"
                                   "BEGIN"
                                   "INSERT INTO usuarios_historial ("
                                   "original_id,"
                                   "old_username,"
                                   "old_password,"
                                   "new_username,"
                                   "new_password,"
                                   "usuario_operacion,"
                                   "tipo_cambio"
                                   ") VALUES ("
                                   "NEW.id,"
                                   "OLD.username,"
                                   "OLD.password,"
                                   "NEW.username,"
                                   "NEW.password,"
                                   "'system_user',"
                                   "'UPDATE'"
                                   ");"
                                   "END;");

            this->sqlite3->command("CREATE TRIGGER IF NOT EXISTS after_usuarios_delete"
                                   "AFTER DELETE ON usuarios"
                                   "FOR EACH ROW"
                                   "BEGIN"
                                   "INSERT INTO usuarios_historial ("
                                   "original_id,"
                                   "old_username,"
                                   "old_password,"
                                   "usuario_operacion,"
                                   "tipo_cambio"
                                   ") VALUES ("
                                   "OLD.id,"
                                   "OLD.username,"
                                   "OLD.password,"
                                   "'system_user',"
                                   "'DELETE'"
                                   ");"
                                   "END;");

            this->sqlite3->command("CREATE TABLE IF NOT EXISTS log_historial ("
                                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                   "original_id INTEGER NOT NULL,"
                                   "old_id_user INTEGER,"
                                   "old_tipo VARCHAR(255),"
                                   "old_ingreso DECIMAL(10, 2),"
                                   "old_cambio DECIMAL(10, 2),"
                                   "old_total DECIMAL(10, 2),"
                                   "old_estatus VARCHAR(255),"
                                   "old_fecha TIMESTAMP,"
                                   "new_id_user INTEGER,"
                                   "new_tipo VARCHAR(255),"
                                   "new_ingreso DECIMAL(10, 2),"
                                   "new_cambio DECIMAL(10, 2),"
                                   "new_total DECIMAL(10, 2),"
                                   "new_estatus VARCHAR(255),"
                                   "new_fecha TIMESTAMP,"
                                   "fecha_modificacion TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
                                   "usuario_operacion VARCHAR(255),"
                                   "tipo_cambio VARCHAR(10)"
                                   ");");

            this->sqlite3->command("CREATE TRIGGER IF NOT EXISTS after_log_insert"
                                   "AFTER INSERT ON log"
                                   "FOR EACH ROW"
                                   "BEGIN"
                                   "INSERT INTO log_historial ("
                                   "original_id,"
                                   "new_id_user,"
                                   "new_tipo,"
                                   "new_ingreso,"
                                   "new_cambio,"
                                   "new_total,"
                                   "new_estatus,"
                                   "new_fecha,"
                                   "usuario_operacion,"
                                   "tipo_cambio"
                                   ") VALUES ("
                                   "NEW.id,"
                                   "NEW.id_user,"
                                   "NEW.tipo,"
                                   "NEW.ingreso,"
                                   "NEW.cambio,"
                                   "NEW.total,"
                                   "NEW.estatus,"
                                   "NEW.fecha,"
                                   "'system_user',"
                                   "'INSERT'"
                                   " );"
                                   "END;");

            this->sqlite3->command("CREATE TRIGGER IF NOT EXISTS after_log_update"
                                   "AFTER UPDATE ON log"
                                   "FOR EACH ROW"
                                   "BEGIN"
                                   "INSERT INTO log_historial ("
                                   "original_id,"
                                   "old_id_user,"
                                   "old_tipo,"
                                   "old_ingreso,"
                                   "old_cambio,"
                                   "old_total,"
                                   "old_estatus,"
                                   "old_fecha,"
                                   "new_id_user,"
                                   "new_tipo,"
                                   "new_ingreso,"
                                   "new_cambio,"
                                   "new_total,"
                                   "new_estatus,"
                                   "new_fecha,"
                                   "usuario_operacion,"
                                   "tipo_cambio"
                                   ") VALUES ("
                                   "NEW.id,"
                                   "OLD.id_user,"
                                   "OLD.tipo,"
                                   "OLD.ingreso,"
                                   "OLD.cambio,"
                                   "OLD.total,"
                                   "OLD.estatus,"
                                   "OLD.fecha,"
                                   "NEW.id_user,"
                                   "NEW.tipo,"
                                   "NEW.ingreso,"
                                   "NEW.cambio,"
                                   "NEW.total,"
                                   "NEW.estatus,"
                                   "NEW.fecha,"
                                   "'system_user',"
                                   "'UPDATE'"
                                   ");"
                                   "END;");

            this->sqlite3->command("CREATE TRIGGER IF NOT EXISTS after_log_delete"
                                   "AFTER DELETE ON log"
                                   "FOR EACH ROW"
                                   "BEGIN"
                                   "INSERT INTO log_historial ("
                                   "original_id,"
                                   "old_id_user,"
                                   "old_tipo,"
                                   "old_ingreso,"
                                   "old_cambio,"
                                   "old_total,"
                                   "old_estatus,"
                                   "old_fecha,"
                                   "usuario_operacion,"
                                   "tipo_cambio"
                                   ") VALUES ("
                                   "OLD.id,"
                                   "OLD.id_user,"
                                   "OLD.tipo,"
                                   "OLD.ingreso,"
                                   "OLD.cambio,"
                                   "OLD.total,"
                                   "OLD.estatus,"
                                   "OLD.fecha,"
                                   "'system_user',"
                                   "'DELETE'"
                                   ");"
                                   "END;");

            std::string ROL_ADMIN{"INSERT INTO usuario_roles values"};
            for (size_t i = 0; i < 19; i++)
            {
                if (i != 18)
                    ROL_ADMIN += "(NULL,1," + std::to_string(i + 1) + "),\n";
                else
                    ROL_ADMIN += "(NULL,1," + std::to_string(i + 1) + ")";
            }
            this->sqlite3->command(ROL_ADMIN);

            this->sqlite3->command("create table Level_Bill (Denominacion INT, Cant_Alm INT, Cant_Recy INT , Nivel_Inmo int)");
            this->sqlite3->command("create table Level_Coin (Denominacion INT, Cant_Alm INT, Cant_Recy INT , Nivel_Inmo int)");
            this->sqlite3->command("insert INTO Level_Bill values (20,0,0,0)");
            this->sqlite3->command("insert INTO Level_Bill values (50,0,0,0)");
            this->sqlite3->command("insert INTO Level_Bill values (100,0,0,0)");
            this->sqlite3->command("insert INTO Level_Bill values (200,0,0,0)");
            this->sqlite3->command("insert INTO Level_Bill values (500,0,0,0)");
            this->sqlite3->command("insert INTO Level_Bill values (1000,0,0,0)");
            this->sqlite3->command("insert INTO Level_Coin values (1,0,0,0)");
            this->sqlite3->command("insert INTO Level_Coin values (2,0,0,0)");
            this->sqlite3->command("insert INTO Level_Coin values (5,0,0,0)");
            this->sqlite3->command("insert INTO Level_Coin values (10,0,0,0)");
        }
    }
    ~connectSqlite() {}
};
