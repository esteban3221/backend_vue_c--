#pragma once
#include "conectbd.hpp"

class niveles_dinero : public connectSqlite
{
private:
    /* data */
public:
    niveles_dinero(/* args */);
    ~niveles_dinero();

    std::map<std::string, std::vector<std::string>> get_niveles_bill();
    std::map<std::string, std::vector<std::string>> get_niveles_coin();
    
    void actualiza_nivel_bill(const int &deno, const int vecu[3]);
    void actualiza_nivel_coin(const int &deno, const int vecu[3]);

    std::map<std::string, std::vector<std::string>> get_niveles_bill_by_deno(const int &deno);
    std::map<std::string, std::vector<std::string>> get_niveles_coin_by_demo(const int &deno);
};