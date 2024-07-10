#include "view_one.hpp"

view_one::view_one(Gtk::Stack &main_stack_,
                   Gtk::Window &main_window_,
                   Gtk::Label *&lbl_main_,
                   Gtk::Image *&img_main_logo_,
                   Gtk::Frame *&nip_set_view_) : main_stack(main_stack_),
                                                 main_window(main_window_),
                                                 lbl_main(lbl_main_),
                                                 img_main_logo(img_main_logo_),
                                                 nip_set_view(nip_set_view_)
{
    BinaryDB::BDB.init();

    if (BinaryDB::BDB.length() == 0)
        BinaryDB::init_();

    builder->add_from_string(this->PAGE0);
    this->page_0 = builder->get_widget<Gtk::Box>("page_0");
    this->set_homogeneous();
    this->append(*page_0);

    this->init_signals_config();
}

view_one::~view_one()
{
    BinaryDB::update_(17, ety_mensaje_inicio->get_text().operator std::string());
}

void view_one::init_signals_config()
{
    this->check_config_notifi = builder->get_widget<Gtk::CheckButton>("check_config_notifi");
    this->btn_select_icon = builder->get_widget<Gtk::Button>("btn_select_icon");
    this->ety_mensaje_inicio = builder->get_widget<Gtk::Entry>("ety_mensaje_inicio");
    this->btn_select_carrousel = builder->get_widget<Gtk::Button>("btn_select_carrousel");
    this->Drop_temporizador = builder->get_widget<Gtk::DropDown>("Drop_temporizador");
    this->lbl_path_carrousel = builder->get_widget<Gtk::Label>("lbl_path_carrousel");
    this->lbl_path_icon = builder->get_widget<Gtk::Label>("lbl_path_icon");
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

    this->btn_select_icon->signal_clicked().connect(sigc::mem_fun(*this, &view_one::on_button_file_clicked));

    this->lbl_path_carrousel->set_text(BinaryDB::select_<std::string>(18).empty() ? "Seleccionar Carpeta" : BinaryDB::select_<std::string>(18));
    this->lbl_path_icon->set_text(BinaryDB::select_<std::string>(16).empty() ? "Seleccionar Imágen" : BinaryDB::select_<std::string>(16));
    this->lbl_path_icon->set_ellipsize(Pango::EllipsizeMode::START);
    this->lbl_path_carrousel->set_ellipsize(Pango::EllipsizeMode::START);

    auto list_dropdown = Gtk::StringList::create({"Ninguno",
                                                  "5 Seg.",
                                                  "10 Seg.",
                                                  "15 Seg.",
                                                  "30 Seg.",
                                                  "1 Min."});

    this->Drop_temporizador->property_model() = list_dropdown;
    this->ety_mensaje_inicio->set_text(BinaryDB::select_<std::string>(17));
    this->ety_mensaje_inicio->signal_changed().connect([this]()
                                                       {
        lbl_main->set_text(ety_mensaje_inicio->get_text());
        BinaryDB::update_(17, ety_mensaje_inicio->get_text().operator std::string()); });

    this->check_config_notifi->set_active(BinaryDB::select_<int>(9));

    this->check_config_notifi->signal_toggled().connect([this]()
                                                        { BinaryDB::update_<int>(9, check_config_notifi->get_active()); });
    this->list_configurable = builder->get_widget<Gtk::ListBox>("list_configurable");
    this->list_configurable->signal_row_activated().connect([this](Gtk::ListBoxRow *row)
                                                            {
                                                                if (row->get_index() == 0)
                                                                {
                                                                    if (check_config_notifi->get_active())
                                                                    {
                                                                        check_config_notifi->set_active(false);
                                                                        BinaryDB::update_<int>(9, false);
                                                                    }
                                                                    else
                                                                    {
                                                                        check_config_notifi->set_active();
                                                                        BinaryDB::update_<int>(9, true);
                                                                    }
                                                                } });
}
void view_one::on_file_dialog_response(int response_id, Gtk::FileChooserDialog *dialog)
{
    switch (response_id)
    {
    case Gtk::ResponseType::OK:
    {
        std::cout << "Open clicked." << std::endl;
        auto filename = dialog->get_file()->get_path();
        img_main_logo->property_file() = filename;
        lbl_path_icon->set_text(filename);
        BinaryDB::update_(16, filename);
        std::cout << "File selected: " << filename << std::endl;
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
void view_one::on_button_file_clicked()
{
    auto dialog = new Gtk::FileChooserDialog(main_window,
                                             /*type ? "Escoge un Directorio" : */ "Escoge una Imágen",
                                             /*type ? Gtk::FileChooser::Action::SELECT_FOLDER :*/ Gtk::FileChooser::Action::OPEN,
                                             true);

    dialog->set_modal(true);
    dialog->set_default_size(10, 10);

    dialog->signal_response().connect(sigc::bind(sigc::mem_fun(*this, &view_one::on_file_dialog_response), dialog));

    dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
    dialog->add_button("_Open", Gtk::ResponseType::OK);

    auto filter_images = Gtk::FileFilter::create();
    filter_images->set_name("Image files");
    filter_images->add_mime_type("image/*");
    dialog->add_filter(filter_images);
    dialog->set_filter(filter_images);

    dialog->show();
}
void view_one::on_click_reboot()
{
    Dialog.reset(new Gtk::MessageDialog(main_window, "Reinicio", false, Gtk::MessageType::QUESTION, Gtk::ButtonsType::CANCEL, true));
    Dialog->add_button("Reiniciar", Gtk::ResponseType::OK)->add_css_class({"warning"});
    Dialog->set_secondary_text("¿Desea reinicia el dispositivo?");
    Dialog->signal_response().connect([this](int response)
                                      {
        if(Gtk::ResponseType::OK == response)
            Helper::System::exec("shutdown -r");

        Dialog->close(); });

    Dialog->show();
}
void view_one::on_click_shutdown()
{
    Dialog.reset(new Gtk::MessageDialog(main_window, "Apagar", false, Gtk::MessageType::QUESTION, Gtk::ButtonsType::CANCEL, true));
    Dialog->add_button("Apagar", Gtk::ResponseType::OK)->add_css_class({"warning"});
    Dialog->set_secondary_text("¿Desea Apagar el dispositivo?");
    Dialog->signal_response().connect([this](int response)
                                      {
        if(Gtk::ResponseType::OK == response)
            Helper::System::exec("shutdown +1");

        Dialog->close(); });

    Dialog->show();
}
void view_one::on_rest_app()
{
    Dialog.reset(new Gtk::MessageDialog(main_window, "Reinicio de Fabrica", false, Gtk::MessageType::QUESTION, Gtk::ButtonsType::CANCEL, true));
    auto btn = Dialog->add_button("Ok", Gtk::ResponseType::OK);
    btn->add_css_class({"destructive-action"});
    btn->set_sensitive(false);
    Dialog->set_secondary_text("Esto eliminara toda configuración y datos guardados.\n"
                               "¿Desea continuar?");

    auto chkb = Gtk::manage(new Gtk::CheckButton("Estoy consiente de la perdida completa de los datos del dispositivo."));
    Dialog->get_content_area()->append(*chkb);

    chkb->signal_toggled().connect([btn, chkb]()
                                   { btn->set_sensitive(chkb->get_active()); });

    Dialog->signal_response().connect([this](int response)
                                      {
        if(Gtk::ResponseType::OK == response)
            std::cout << "Reinicio de Fabrica\n";

        Dialog->close(); });

    Dialog->show();
}