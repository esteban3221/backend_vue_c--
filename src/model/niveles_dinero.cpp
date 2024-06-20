#include "niveles_dinero.hpp"

niveles_dinero::niveles_dinero(/* args */)
{
}

niveles_dinero::~niveles_dinero()
{
}

std::map<std::string, std::vector<std::string>> niveles_dinero::get_niveles_bill()
{
    this->sqlite3->command("select * from Level_Bill");
    return this->sqlite3->get_result();
}

std::map<std::string, std::vector<std::string>> niveles_dinero::get_niveles_coin()
{
    this->sqlite3->command("select * from Level_Coin");
    return this->sqlite3->get_result();
}

void niveles_dinero::actualiza_nivel_bill(const int &deno, const int vecu[3])
{
    this->sqlite3->command("update Level_Bill set Cant_Alm = ? , Cant_Recy = ?, Nivel_Inmo = ? where Denominacion = ?", vecu[0], vecu[1], vecu[2], deno);
}

void niveles_dinero::actualiza_nivel_coin(const int &deno, const int vecu[3])
{
    this->sqlite3->command("update Level_Coin set Cant_Alm = ? , Cant_Recy = ?, Nivel_Inmo = ? where Denominacion = ?", vecu[0], vecu[1], vecu[2], deno);
}

std::map<std::string, std::vector<std::string>> niveles_dinero::get_niveles_bill_by_deno(const int &deno)
{
    this->sqlite3->command("select * from Level_Bill where Denominacion = ?", deno);
    return this->sqlite3->get_result();
}

std::map<std::string, std::vector<std::string>> niveles_dinero::get_niveles_coin_by_demo(const int &deno)
{
    this->sqlite3->command("select * from Level_Coin where Denominacion = ?", deno);
    return this->sqlite3->get_result();
}
