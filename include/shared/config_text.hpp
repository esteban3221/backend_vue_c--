#pragma once
#include <gtkmm.h>
#include "wrapbinary.hpp"
#include <iomanip>

namespace Config
{
    namespace TextView
    {
        extern Glib::RefPtr<Gtk::TextBuffer> m_refTextBuffer1 ;
        std::string preview_ticket();
    } // namespace TextView
    
    enum Action
    {
        Venta,
        Pago_No_Disponible,
        Pago,
        Carga,
        Retirada,
        Cambio,
        Ingreso,
        Atasco_Entrada,
        Atasco_Salida
    };
} // namespace Config
