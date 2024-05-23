#include "config_text.hpp"

namespace Config
{
    namespace TextView
    {
        Glib::RefPtr<Gtk::TextBuffer> m_refTextBuffer1 = Gtk::TextBuffer::create();

        std::string preview_ticket()
        {
            std::stringstream ticket_config;

            ticket_config << "****** TICKET DE COMPRA ******\n"
                          << "--------------------------------\n\n"
                          << std::left << std::setw(20) << BinaryDB::select_<std::string>(2) << "\n\n";

            if (BinaryDB::select_<int>(12))
            {
                ticket_config << "Direccion: " << BinaryDB::select_<std::string>(3) << "\n"
                              << "--------------------------------\n";
            }

            if (BinaryDB::select_<int>(13))
            {
                ticket_config << "RFC: " << BinaryDB::select_<std::string>(4) << "\n"
                              << "--------------------------------\n";
            }
            if (BinaryDB::select_<int>(11))
                ticket_config << "Fecha: "
                              << Glib::DateTime::create_now_local().format("%Y-%m-%d %H:%M:%S")
                              << "\n";
            ticket_config << "No. Ticket: 0001"
                          << "\n\n";
            if (BinaryDB::select_<int>(14))
                ticket_config << std::left << std::setw(10) << "Le atendio: "
                              << "Juan Perez"
                              << "\n\n"
                              << "--------------------------------\n";

            ticket_config << "Articulo\n"
                          << std::left << std::setw(10) << "Cnt." << std::setw(10) << "P.U."
                          << "T.\n"
                          << "--------------------------------\n";
            ticket_config << "Articulo de prueba\n"
                          << std::setw(10) << "10" << std::setw(10) << "6"
                          << std::setw(10) << "60"
                          << "\n"
                          << "--------------------------------\n";
            ticket_config << std::left << std::setw(20) << "Total:"
                          << "60.00\n";
            ticket_config << std::left << std::setw(20) << "Tipo de Pago:"
                          << "Efectivo\n"
                          << "--------------------------------\n";
            ticket_config << std::left << std::setw(20) << "Ingreso:"
                          << "90.00\n";
            ticket_config << std::left << std::setw(20) << "Cambio:"
                          << "30.00\n"
                          << "--------------------------------\n";
            if (BinaryDB::select_<int>(15))
                ticket_config << "**" << BinaryDB::select_<std::string>(5) << "**"
                              << "\n"
                              << "--------------------------------\n";
            if (BinaryDB::select_<int>(15))
                ticket_config << "**" << BinaryDB::select_<std::string>(6) << "**"
                              << "\n"
                              << "--------------------------------\n";
            m_refTextBuffer1->set_text(ticket_config.str());
            return ticket_config.str();
        }
    } // namespace TextView
} // namespace Config
