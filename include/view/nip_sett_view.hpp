#pragma once
#include "nip_view.hpp"
#include "wrapbinary.hpp"

class nip_sett_view : public nip_view 
{
private:
    void on_btn_nip_enter() override;
    void on_btn_nip_back() override;
    uint16_t paso;
    std::string new_pin;
public:
    nip_sett_view(Gtk::Box *&box_principal_, Gtk::Stack &main_stack_, Gtk::Frame *&frame_config_);
    ~nip_sett_view();
};