#include "view_six.hpp"

void view_six::init_datos_red()
{
    this->lbl_red[0] = this->builder->get_widget<Gtk::EditableLabel>("ety_info_ip_wifi");
    this->lbl_red[1] = this->builder->get_widget<Gtk::EditableLabel>("ety_info_mac_wifi");
    this->lbl_red[2] = this->builder->get_widget<Gtk::EditableLabel>("ety_info_ip_eth");
    this->lbl_red[3] = this->builder->get_widget<Gtk::EditableLabel>("ety_info_ip_eth_mac");
    this->lbl_red[4] = this->builder->get_widget<Gtk::EditableLabel>("ety_info_ssid_wifi");
}

view_six::view_six(/* args */)
{
    builder->add_from_string(this->PAGE5);
    this->page_5 = this->builder->get_widget<Gtk::Box>("page_5");
    this->append(*page_5);
    this->page_5->set_hexpand();

        this->init_datos_red();
    this->poll_info_red = Glib::signal_timeout().connect([this]() -> bool
                                                         {
        std::string SSID = Helper::System::exec("nmcli -t -f active,ssid dev wifi | grep -E '^(sí|yes):' | cut -d':' -f2");
        
        std::string IP_WLAN = Helper::System::exec(
        "for iface in wlan0 wlp2s0; do "
        "  ip=$(ip -o -4 addr show $iface 2>/dev/null | awk '{print $4}' | cut -d/ -f1); "
        "  if [ ! -z \"$ip\" ]; then echo $ip; break; fi; "
        "done");

        std::string MAC_WLAN = Helper::System::exec(
            "for iface in wlan0 wlp2s0; do "
            "  mac=$(cat /sys/class/net/$iface/address 2>/dev/null); "
            "  if [ ! -z \"$mac\" ]; then echo $mac; break; fi; "
            "done");



        std::string IP_ETHERNET = Helper::System::exec(
            "for iface in eth0 end0 enp0s31f6 enp0s20f0u1u4; do "
            "  ip=$(ip -o -4 addr show $iface 2>/dev/null | awk '{print $4}' | cut -d/ -f1); "
            "  if [ ! -z \"$ip\" ]; then echo $ip; break; fi; "
            "done");

        std::string MAC_ETHERNET = Helper::System::exec(
            "for iface in eth0 end0 enp0s31f6 enp0s20f0u1u4; do "
            "  mac=$(cat /sys/class/net/$iface/address 2>/dev/null); "
            "  if [ ! -z \"$mac\" ]; then echo $mac; break; fi; "
            "done");
            // Especifico de la RPI 4
            this->lbl_red[4]->set_text(SSID.empty() ? "No conectado." : SSID);
            this->lbl_red[0]->set_text(IP_WLAN.empty() ? "- - - -" : IP_WLAN);
            this->lbl_red[1]->set_text(MAC_WLAN.empty() ? "" : MAC_WLAN);

            this->lbl_red[2]->set_text(IP_ETHERNET.empty() ? "No conectado." : IP_ETHERNET);
            this->lbl_red[3]->set_text(MAC_ETHERNET.empty() ? "" : MAC_ETHERNET);

        return true; }, 5000);
}

view_six::~view_six()
{
}
