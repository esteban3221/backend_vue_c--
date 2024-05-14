#include "nip_sett_view.hpp"

nip_sett_view::nip_sett_view(Gtk::Box *&box_principal_, Gtk::Stack &main_stack_,Gtk::Frame *&frame_config_) :
nip_view(box_principal_, main_stack_, frame_config_) , paso (0)
{
    this->ety_pin->property_placeholder_text() = "Digite el pin antiguo";
}

void nip_sett_view::on_btn_nip_back()
{
    this->ety_pin->property_placeholder_text() = "Digite el pin antiguo";
    this->ety_pin->set_css_classes({"entry","title-1"});
    this->new_pin.clear();
    this->main_stack.set_visible_child(*frame_config);
    this->paso = 0;
}

void nip_sett_view::on_btn_nip_enter()
{
    if(this->paso == 0)
    {
        this->ety_pin->set_css_classes({"entry","title-1"});
        if (BinaryDB::select_string_(1) == this->ety_pin->get_text().operator std::string())
        {
            this->paso++;
            this->ety_pin->property_placeholder_text() = "Digite el nuevo pin";
            this->ety_pin->set_text("");
            this->ety_pin->set_css_classes({"entry","title-1"});
            return;
        }
        else
        {
            this->ety_pin->property_placeholder_text() = "Error";
            this->ety_pin->set_css_classes({"error","title-1"});
            this->ety_pin->set_text("");
            return;
        }
    }
    else if(paso == 1)
    {
        if(ety_pin->get_text().operator std::string().size() == 6)
        {
            this->paso++;
            this->ety_pin->property_placeholder_text() = "Confirma pin";
            this->ety_pin->set_css_classes({"entry","title-1"});
            this->new_pin = (ety_pin->get_text());
            this->ety_pin->set_text("");
            return;
        }
        else
        {
            this->ety_pin->property_placeholder_text() = "Rango Invalido";
            this->ety_pin->set_css_classes({"error","title-1"});
            this->ety_pin->set_text("");
            return;
        }
    }
    else if(paso == 2)
    {
        if(ety_pin->get_text().operator std::string() == new_pin)
        {
            this->ety_pin->property_placeholder_text() = "Pin Actualizado exitosamente";
            this->ety_pin->set_css_classes({"entry","title-1"});
            this->ety_pin->set_text("");
            this->paso = 0;
            this->ety_pin->property_placeholder_text() = "Digite el pin antiguo";
            this->main_stack.set_visible_child(*frame_config);

            BinaryDB::update_(1,this->new_pin);
            this->new_pin.clear();
            return;
        }
        else
        {
            this->ety_pin->property_placeholder_text() = "Pin no coincide";
            this->ety_pin->set_css_classes({"error","title-1"});
            this->ety_pin->set_text("");
            return;
        }
    }
}

nip_sett_view::~nip_sett_view()
{
}
 