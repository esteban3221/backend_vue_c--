#pragma once
#include <gtkmm.h>
#include <tuple>

namespace Levels
{

    namespace Bill
    {
        extern Gtk::Label *Cnt_Reciclador[7];
        extern Gtk::Label *Cnt_almacenado[7];
        extern Gtk::SpinButton *Niveles_inmobilidad[6];
        extern std::tuple<int[6],int[6]> vecu;
    } // namespace Bill

    namespace Coin
    {
        extern Gtk::Label *Cnt_Reciclador[5];
        extern Gtk::Label *Cnt_almacenado[5];
        extern Gtk::SpinButton *Niveles_inmobilidad[4];
        extern std::tuple<int[6],int[6]> vecu;
    } // namespace Coin

} // namespace Levels
