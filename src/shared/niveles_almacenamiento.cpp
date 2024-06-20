#include "niveles_almacenamiento.hpp"

namespace Levels
{

    namespace Bill
    {
        Gtk::Label *Cnt_Reciclador[7] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
        Gtk::Label *Cnt_almacenado[7] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
        Gtk::SpinButton *Niveles_inmobilidad[6] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
        std::tuple<int[6],int[6]> vecu;
    } // namespace Bill

    namespace Coin
    {
        Gtk::Label *Cnt_Reciclador[5] = {nullptr, nullptr, nullptr, nullptr, nullptr};
        Gtk::Label *Cnt_almacenado[5] = {nullptr, nullptr, nullptr, nullptr, nullptr};
        Gtk::SpinButton *Niveles_inmobilidad[4] = {nullptr, nullptr, nullptr, nullptr};
        std::tuple<int[6],int[6]> vecu;
    } // namespace Coin

} // namespace Levels