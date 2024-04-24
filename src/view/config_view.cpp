#include "config_view.hpp"

config_view::config_view(Gtk::Box *&box_principal_, Gtk::Stack &main_stack_, Gtk::Window &main_window_, Gtk::Label *&lbl_main_, Gtk::Image *&img_main_logo_, Gtk::Frame *&nip_set_view_) : box_principal(box_principal_),
                                                                                                                                                               main_stack(main_stack_),
                                                                                                                                                               main_window(main_window_),
                                                                                                                                                               lbl_main(lbl_main_),
                                                                                                                                                               img_main_logo(img_main_logo_),
                                                                                                                                                               nip_set_view(nip_set_view_)
{
    db.init();

    if (db.length() == 0)
    {
        db.clear();
        db.adding_string("123456"); // nip de fabrica
        db.adding_string("");       // razon social
        db.adding_string("");       // direccion
        db.adding_string("");       // rfc
        db.adding_string("");       // Contacto
        db.adding_string("");       // Agradecimiento

        db.adding_number<int>(true); // status impresion
        db.adding_number<int>(true); // status inalambrica
        db.adding_number<int>(true); // Mostrar Notificaciones

        db.adding_number<int>(true); // Mostrar Mensaje de agradeciemiento
        db.adding_number<int>(true); // Mostrar fecha
        db.adding_number<int>(true); // Mostrar direccion
        db.adding_number<int>(true); // Mostrar rfc
        db.adding_number<int>(true); // Mostrar vendedor
        db.adding_number<int>(true); // Mostrar contacto

        db.adding_string("");           // Ruta Logo Inicio
        db.adding_string("Bienvenido"); // Mensaje de inicio
        db.adding_string("");           // Ruta de Carrousel de Img.
        db.adding_number<int>(5);       // Temporizador de carrousel

        db.save();
        std::cout << "Se escribio archivo binario" << std::endl;
    }
    try
    {
        builder->add_from_string(this->XML);
    }
    catch (const Glib::Error &e)
    {
        std::cout << " In config view :" << e.what() << "\n";
        // this->get_parent()->get_parent();
    }

    this->box_configuracion = builder->get_widget<Gtk::Box>("box_configuracion");
    this->btn_back_config = builder->get_widget<Gtk::Button>("btn_back_config");

    this->set_child(*box_configuracion);

    this->btn_back_config->signal_clicked().connect([this](){ main_stack.set_visible_child(*box_principal); });

    this->init_info_systema();
    this->init_signals_config();
    this->inti_ety_datos();
    this->init_signals_listas();
    this->init_signals_impresion();
}

void config_view::on_file_dialog_response(int response_id, Gtk::FileChooserDialog *dialog)
{
    // Handle the response:
    switch (response_id)
    {
    case Gtk::ResponseType::OK:
    {
        std::cout << "Open clicked." << std::endl;

        // Notice that this is a std::string, not a Glib::ustring.
        auto filename = dialog->get_file()->get_path();
        img_main_logo->property_file() = filename;
        lbl_path_icon->set_text(filename);
        db.update_string(16, filename);
        std::cout << "File selected: " << filename << std::endl;
        db.save();
        break;
    }
    case Gtk::ResponseType::CANCEL:
    {
        std::cout << "Cancel clicked." << std::endl;
        break;
    }
    default:
    {
        std::cout << "Unexpected button clicked." << std::endl;
        break;
    }
    }
    delete dialog;
}

void config_view::on_button_file_clicked()
{
    auto dialog = new Gtk::FileChooserDialog(main_window,
                                             /*type ? "Escoge un Directorio" : */ "Escoge una Imágen",
                                             /*type ? Gtk::FileChooser::Action::SELECT_FOLDER :*/ Gtk::FileChooser::Action::OPEN,
                                             true);

    dialog->set_modal(true);
    dialog->set_default_size(10, 10);

    dialog->signal_response().connect(sigc::bind(sigc::mem_fun(*this, &config_view::on_file_dialog_response), dialog));

    dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
    dialog->add_button("_Open", Gtk::ResponseType::OK);

    auto filter_images = Gtk::FileFilter::create();
    filter_images->set_name("Image files");
    filter_images->add_mime_type("image/*");
    dialog->add_filter(filter_images);
    dialog->set_filter(filter_images);

    dialog->show();
}

void config_view::init_signals_config()
{
    this->check_config_notifi = builder->get_widget<Gtk::CheckButton>("check_config_notifi");
    this->btn_select_icon = builder->get_widget<Gtk::Button>("btn_select_icon");
    this->ety_mensaje_inicio = builder->get_widget<Gtk::Entry>("ety_mensaje_inicio");
    this->btn_select_carrousel = builder->get_widget<Gtk::Button>("btn_select_carrousel");
    this->Drop_temporizador = builder->get_widget<Gtk::DropDown>("Drop_temporizador");
    this->lbl_path_carrousel = builder->get_widget<Gtk::Label>("lbl_path_carrousel");
    this->lbl_path_icon = builder->get_widget<Gtk::Label>("lbl_path_icon");

    auto list_dropdown = Gtk::StringList::create ({"Ninguno",
    "5 Seg.",
    "10 Seg.",
    "15 Seg.",
    "30 Seg.",
    "1 Min."});

    this->Drop_temporizador->property_model() = list_dropdown;
    this->ety_mensaje_inicio->set_text(db.get_string(17));
    this->ety_mensaje_inicio->signal_changed().connect([this](){
        lbl_main->set_text(ety_mensaje_inicio->get_text());
        db.update_string(17, ety_mensaje_inicio->get_text());
        db.save();
    });

    this->btn_select_icon->signal_clicked().connect(sigc::mem_fun(*this, &config_view::on_button_file_clicked));

    this->lbl_path_carrousel->set_text(db.get_string(18).empty() ? "Seleccionar Carpeta" : db.get_string(18));
    this->lbl_path_icon->set_text(db.get_string(16).empty() ? "Seleccionar Imágen" : db.get_string(16));
    this->lbl_path_icon->set_ellipsize(Pango::EllipsizeMode::START);
    this->lbl_path_carrousel->set_ellipsize(Pango::EllipsizeMode::START);

    this->check_config_notifi->set_active(db.get_number<int>(9));
    this->check_config_notifi->signal_toggled().connect([this]()
                                                        { db.update_number<int>(9, check_config_notifi->get_active()); });
    this->list_configurable = builder->get_widget<Gtk::ListBox>("list_configurable");
    this->list_configurable->signal_row_activated().connect([this](Gtk::ListBoxRow *row)
                                                            {
                                                                if (row->get_index() == 0)
                                                                {
                                                                    if (check_config_notifi->get_active())
                                                                    {
                                                                        check_config_notifi->set_active(false);
                                                                        db.update_number<int>(9, false);
                                                                    }
                                                                    else
                                                                    {
                                                                        check_config_notifi->set_active();
                                                                        db.update_number<int>(9, true);
                                                                    }
                                                                    db.save();
                                                                } });
}

void config_view::init_signals_listas()
{
    this->list_config_general = builder->get_widget<Gtk::ListBox>("list_config_general");
    this->list_config_general->signal_row_activated().connect([this](Gtk::ListBoxRow *row)
                                                              {
                                                            switch (row->get_index())
                                                            {
                                                            case 0:{
                                                                //llamar vista para establecer un nuevo nip
                                                                main_stack.set_visible_child(*nip_set_view);
                                                                break;
                                                            }
                                                            case 1:{
                                                                //reiniciar
                                                                on_click_reboot();
                                                                break;
                                                            }
                                                            case 2:{
                                                                //apagar
                                                                on_click_shutdown();
                                                                break;
                                                            }
                                                            case 3:{
                                                                // restablecer de fabrica
                                                                on_rest_app();
                                                                break;
                                                            }
                                                            default:
                                                                break;
                                                            } });
}

void config_view::init_info_systema()
{
    this->list_info_system[0] = builder->get_widget<Gtk::EditableLabel>("ety_info_host");
    this->list_info_system[1] = builder->get_widget<Gtk::EditableLabel>("ety_info_modelo");
    this->list_info_system[2] = builder->get_widget<Gtk::EditableLabel>("ety_info_procesador");
    this->list_info_system[3] = builder->get_widget<Gtk::EditableLabel>("ety_info_memoria");
    this->list_info_system[4] = builder->get_widget<Gtk::EditableLabel>("ety_info_disco");

    const std::string parent{"cat /sys/devices/virtual/dmi/id/"};
    const std::string a{parent + "board_vendor"};
    const std::string b{parent + "product_name"};

    const auto model{(exec(a.c_str())).empty() ? exec("cat /proc/device-tree/model") : exec(a.c_str()) + " " + exec(b.c_str())};

    this->list_info_system[0]->set_text(exec("cat /etc/hostname"));
    this->list_info_system[1]->set_text(model);
    this->list_info_system[2]->set_text(exec("lscpu | grep -E 'Nombre del modelo|Model name' | awk -F': ' '{print $2}'"));
    this->list_info_system[3]->set_text(exec("grep MemTotal /proc/meminfo | awk '{print $2/1024/1024 \" GB\"}' "));
    this->list_info_system[4]->set_text(exec("lsblk -o SIZE -b | head -2 | tail -1 | awk '{print $1/1024/1024/1024 \" GB\"}'"));
}

void config_view::init_signals_impresion()
{
    this->list_config = builder->get_widget<Gtk::ListBox>("list_config");
    this->list_config_visualizacion = builder->get_widget<Gtk::ListBox>("list_config_visualizacion");
    this->switch_impresion = builder->get_widget<Gtk::Switch>("switch_impresion");
    this->switch_impresion->set_active((bool)db.get_number<int>(7));

    this->list_config->signal_row_activated().connect([this](Gtk::ListBoxRow *row)
                                                      {
        switch_impresion->get_active() ? switch_impresion->set_active(false) : switch_impresion->set_active();
        db.update_number<int>(7, switch_impresion->get_active());
        db.save(); });

    this->switch_impresion->property_active().signal_changed().connect([this]()
                                                                       {
        db.update_number<int>(7, switch_impresion->get_active());
        db.save(); });

    for (size_t i = 0; i < list_view_ticket.size(); i++)
    {
        list_view_ticket[i] = builder->get_widget<Gtk::CheckButton>("check_config_" + std::to_string(i + 1));
        list_view_ticket[i]->set_active((bool)db.get_number<int>(i + 10));
        list_view_ticket[i]->signal_toggled().connect([this, i]()
                                                      {
            db.update_number<int>(i + 10, list_view_ticket[i]->get_active());
            db.save(); });
    }
    this->list_config_visualizacion->signal_row_activated().connect([this](Gtk::ListBoxRow *row)
                                                                    {
        list_view_ticket[row->get_index()]->get_active() ? list_view_ticket[row->get_index()]->set_active(false) : list_view_ticket[row->get_index()]->set_active();
        db.update_number<int>(row->get_index() + 10, list_view_ticket[row->get_index()]->get_active());
        db.save(); });
}

void config_view::inti_ety_datos()
{
    this->list_ety_datos[0] = builder->get_widget<Gtk::Entry>("ety_conf_razon");
    this->list_ety_datos[1] = builder->get_widget<Gtk::Entry>("ety_conf_direccion");
    this->list_ety_datos[2] = builder->get_widget<Gtk::Entry>("ety_conf_rfc");
    this->list_ety_datos[3] = builder->get_widget<Gtk::Entry>("ety_conf_contacto");
    this->list_ety_datos[4] = builder->get_widget<Gtk::Entry>("ety_conf_thanks");

    for (int i = 0; i < list_ety_datos.size(); i++)
        list_ety_datos[i]->set_text(db.get_string(i + 2));

    for (int i = 0; i < list_ety_datos.size(); i++)
        list_ety_datos[i]->signal_changed().connect([this, i]()
                                                    {db.update_string(i+2,list_ety_datos[i]->get_text()); db.save(); });
}

std::string config_view::exec(const char *cmd)
{
    std::array<char, 128> buffer;
    std::string result;

    auto file_deleter = [](FILE *file)
    {
        if (file)
            pclose(file);
    };

    std::unique_ptr<FILE, decltype(file_deleter)> pipe(popen(cmd, "r"), file_deleter);

    if (!pipe)
        throw std::runtime_error("popen() failed!");

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
        result += buffer.data();

    result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
    return result;
}

void config_view::on_click_reboot()
{
    Dialog.reset(new Gtk::MessageDialog(main_window, "Reinicio", false, Gtk::MessageType::QUESTION, Gtk::ButtonsType::CANCEL, true));
    Dialog->add_button("Reiniciar", Gtk::ResponseType::OK)->add_css_class({"warning"});
    Dialog->set_secondary_text("¿Desea reinicia el dispositivo?");
    Dialog->signal_response().connect([this](int response){
        if(Gtk::ResponseType::OK == response)
            std::cout << "reboot\n";

        Dialog->close();
    });

    Dialog->show();
}
void config_view::on_click_shutdown()
{
    Dialog.reset(new Gtk::MessageDialog(main_window, "Apagar", false, Gtk::MessageType::QUESTION, Gtk::ButtonsType::CANCEL, true));
    Dialog->add_button("Apagar", Gtk::ResponseType::OK)->add_css_class({"warning"});
    Dialog->set_secondary_text("¿Desea Apagar el dispositivo?");
    Dialog->signal_response().connect([this](int response){
        if(Gtk::ResponseType::OK == response)
            std::cout << "apagar\n";

        Dialog->close();
    });

    Dialog->show();
}
void config_view::on_rest_app()
{
    Dialog.reset(new Gtk::MessageDialog(main_window, "Reinicio de Fabrica", false, Gtk::MessageType::QUESTION, Gtk::ButtonsType::CANCEL, true));
    auto btn = Dialog->add_button("Ok", Gtk::ResponseType::OK);
    btn->add_css_class({"destructive-action"});
    btn->set_sensitive(false);
    Dialog->set_secondary_text("Esto eliminara toda configuración y datos guardados.\n""¿Desea continuar?");

    auto chkb = Gtk::manage(new Gtk::CheckButton("Estoy consiente de la perdida completa de los datos del dispositivo."));
    Dialog->get_content_area()->append(*chkb);

    chkb->signal_toggled().connect([btn,chkb](){ 
        btn->set_sensitive(chkb->get_active()); 
    });

    Dialog->signal_response().connect([this](int response){
        if(Gtk::ResponseType::OK == response)
            std::cout << "Reinicio de Fabrica\n";

        Dialog->close();
    });

    Dialog->show();
}



config_view::~config_view()
{
}
