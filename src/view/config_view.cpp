#include "config_view.hpp"

config_view::config_view(Gtk::Box *&box_principal_,
                         Gtk::Stack &main_stack_,
                         Gtk::Box *&view_one_) : box_principal(box_principal_),
                                                 main_stack(main_stack_),
                                                 view_one(view_one_)
{
    builder->add_from_string(this->XML);

    this->set_orientation(Gtk::Orientation::HORIZONTAL);
    this->btn_back_config = builder->get_widget<Gtk::Button>("btn_back_config");
    this->left_box = builder->get_widget<Gtk::Box>("left_box");
    this->stackside = builder->get_widget<Gtk::StackSidebar>("stackside");
    stackside->set_stack(stacconfig);
    stacconfig.set_margin(10);
    stacconfig.set_hexpand();
    stacconfig.set_transition_type(Gtk::StackTransitionType::SLIDE_UP_DOWN);

    stacconfig.add(*view_one, "page 0", "General");
    stacconfig.add(*v3, "page 1", "Inf. del Sistema");
    stacconfig.add(*v2, "page 2", "Datos de Empresa");
    stacconfig.add(*v4, "page 3", "Impresora");
    stacconfig.add(*v5, "page 4", "Usuarios");
    stacconfig.add(*v6, "page 5", "Red");
    stacconfig.add(*v7, "page 6", "Fondo de Caja");

    this->append(*left_box);
    this->append(stacconfig);

    this->btn_back_config->signal_clicked().connect([this]() { main_stack.set_visible_child(*box_principal); });
}

// void config_view::test_creat_pdf(Gtk::ListBoxRow *row)
// {
//     #ifdef CAIRO_HAS_PDF_SURFACE

//     auto surface = Cairo::PdfSurface::create("Prueba pdf", 226.77, 600.0); // Tamaño A4
//     auto cr = Cairo::Context::create(surface);

//         cr->select_font_face("Sans", (Cairo::ToyFontFace::Slant) 0 , (Cairo::ToyFontFace::Weight) 0);
//         cr->set_font_size(24);
//         cr->move_to(50, 100);
//         cr->show_text("Hola, Mundo!");

//         cr->show_page();

// #else
//     std::cout << "You must compile cairo with PDF support for this example to work." << std::endl;
// #endif
// }

config_view::~config_view()
{
}
