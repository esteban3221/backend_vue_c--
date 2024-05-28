#include "venta.controller.hpp"

venta_controller::venta_controller(crow::App<crow::CookieParser, Session> &app_, Gtk::Stack &main_stack_, Gtk::Box *&box_main_) : app(app_),
                                                                                                                                  main_stack(main_stack_),
                                                                                                                                  box_main(box_main_)
{
    this->init_ui();

    // test no requieren token EXCLUSIVO DE TEST
    CROW_ROUTE(app, "/test/venta").methods("POST"_method)(sigc::mem_fun(*this, &venta_controller::Test_venta));
}

crow::response venta_controller::Test_venta(const crow::request &req)
{
    this->m_Dispatcher.emit();
    auto x = crow::json::load(req.body);
    if (!x || !x["value"])
    {
        this->main_stack.set_visible_child(*box_main);
        return crow::response(crow::status::BAD_REQUEST);
    }

    this->main_stack.set_visible_child(*this);
    auto total = x["value"].i() / 100;

    std::this_thread::sleep_for(std::chrono::milliseconds(3));
    this->ety_recibido.set_text("");
    this->ety_faltante.set_text("");
    this->ety_cambio.set_text("");

    this->lbl_mensaje_fin.set_visible(false);
    this->ety_monto_total.set_text(std::to_string(total));

    if (!this->init_perifericos())
    {
        this->main_stack.set_visible_child(*box_main);
        return crow::response(crow::status::INTERNAL_SERVER_ERROR, "Periferico no se encuentra o esta ocupado.");
    }

    long sum = 0;
    unsigned channel;

    while (sum < total)
    {
        if (ssp_poll(this->ssp_setup_bill, &this->poll) != SSP_RESPONSE_OK)
            printf("SSP_POLL_ERROR\n");
        if (ssp_poll(this->ssp_setup_coin, &this->poll) != SSP_RESPONSE_OK)
            printf("SSP_POLL_ERROR\n");
        for (int i = 0; i < this->poll.event_count; ++i)
        {
            switch (this->poll.events[i].event)
            {
            case SSP_POLL_RESET:
                printf("Unit Reset\n");
                break;
            case SSP_POLL_READ:
                if (this->poll.events[i].data > 0)
                    channel = this->poll.events[i].data;
                break;
            case SSP_POLL_CREDIT:
                channel = this->poll.events[i].data;
                break;
            case SSP_POLL_STACKED:
            {
                printf("Stacked %d MXN$\n", this->DENOMINATION[channel]);
                sum += this->DENOMINATION[channel];
                channel = 0;
                this->ety_recibido.set_text(std::to_string(sum));
                (sum < total) ? this->ety_faltante.set_text(std::to_string(total - sum)) : this->ety_faltante.set_text("0");
                break;
            }
            case SSP_POLL_FRAUD_ATTEMPT:
                printf("Fraud Attempt %ld\n", this->poll.events[i].data);
                break;
            case SSP_POLL_STACKER_FULL:
            {
                CloseSSPPort(this->port_bill);
                CloseSSPPort(this->port_coin);
                printf("Stacker Full\n");
                //return crow::response(crow::status::CONFLICT, "Stacker Full");
                break;
            }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    ssp_disable(this->ssp_setup_bill);
    ssp_disable(this->ssp_setup_coin);
    CloseSSPPort(this->port_bill);
    CloseSSPPort(this->port_coin);

    //ternaria no funciona en un thread hacia modificacion de gui
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
    if (sum > total)  
        this->ety_cambio.set_text(std::to_string(sum-total));
    this->lbl_mensaje_fin.set_visible();
    
    std::this_thread::sleep_for(std::chrono::seconds(3));
    this->main_stack.set_visible_child(*box_main);
    this->m_Dispatcher.emit();
    return crow::response(crow::status::OK, "OK");
}

venta_controller::~venta_controller()
{
}

bool venta_controller::init_perifericos()
{
    this->port_bill = OpenSSPPort("/dev/ttyUSB0");
    this->port_coin = OpenSSPPort("/dev/ttyUSB1");

    this->ssp_setup_bill.port = port_bill;
    this->ssp_setup_coin.port = port_coin;

    this->ssp_setup_bill.SSPAddress = 0;
    this->ssp_setup_coin.SSPAddress = 16;

    this->ssp_setup_bill.Timeout = this->ssp_setup_coin.Timeout = 1000;
    this->ssp_setup_bill.EncryptionStatus = this->ssp_setup_coin.EncryptionStatus = NO_ENCRYPTION;
    this->ssp_setup_bill.RetryLevel = this->ssp_setup_coin.RetryLevel = 3;

    return ((ssp_setup_encryption(&ssp_setup_bill, 0x0123456701234567L) == SSP_RESPONSE_OK) &&
            (ssp_enable(ssp_setup_bill) == SSP_RESPONSE_OK) &&
            (ssp_set_inhibits(ssp_setup_bill, 0xFF, 0xFF) == SSP_RESPONSE_OK));
}
void venta_controller::init_ui()
{
    this->lbl_titulo.set_text("Test Venta");
    this->lbl_titulo.set_css_classes({"title-1"});

    this->box_venta.append(lbl_titulo);
    this->box_venta.set_orientation(Gtk::Orientation::VERTICAL);
    this->box_venta.set_margin(15);

    this->lbl_mensaje_fin.set_text("Gracias por su compra.\n    No olvide su ticket");
    this->lbl_mensaje_fin.set_css_classes({"title-2", "dim-label"});

    this->lbl_monto_total.set_text("Total");
    this->lbl_monto_total.set_css_classes({"title-3"});
    this->lbl_recibido.set_text("Recibido");
    this->lbl_recibido.set_css_classes({"title-3"});
    this->lbl_faltante.set_text("Falta");
    this->lbl_faltante.set_css_classes({"title-3"});
    this->lbl_cambio.set_text("Cambio");
    this->lbl_cambio.set_css_classes({"title-3"});

    this->r1.append(lbl_monto_total);
    this->r1.append(ety_monto_total);
    ety_monto_total.set_halign(Gtk::Align::END);
    ety_monto_total.set_hexpand();
    ety_monto_total.set_editable(false);
    ety_monto_total.set_has_frame(false);
    ety_monto_total.set_css_classes({"title-3", "dim-label"});

    this->r2.append(lbl_recibido);
    this->r2.append(ety_recibido);
    ety_recibido.set_halign(Gtk::Align::END);
    ety_recibido.set_hexpand();
    ety_recibido.set_editable(false);
    ety_recibido.set_has_frame(false);
    ety_recibido.set_css_classes({"title-3", "dim-label"});

    this->r3.append(lbl_faltante);
    this->r3.append(ety_faltante);
    ety_faltante.set_halign(Gtk::Align::END);
    ety_faltante.set_hexpand();
    ety_faltante.set_editable(false);
    ety_faltante.set_has_frame(false);
    ety_faltante.set_css_classes({"title-3", "dim-label"});

    this->r4.append(lbl_cambio);
    this->r4.append(ety_cambio);
    ety_cambio.set_halign(Gtk::Align::END);
    ety_cambio.set_hexpand();
    ety_cambio.set_editable(false);
    ety_cambio.set_has_frame(false);
    ety_cambio.set_css_classes({"title-3", "dim-label"});

    this->BXRW1.set_child(this->r1);
    this->BXRW2.set_child(this->r2);
    this->BXRW3.set_child(this->r3);
    this->BXRW4.set_child(this->r4);

    this->list_venta.append(this->BXRW1);
    this->list_venta.append(this->BXRW2);
    this->list_venta.append(this->BXRW3);
    this->list_venta.append(this->BXRW4);

    list_venta.set_css_classes({"rich-list", "boxed-list"});
    this->box_venta.append(this->list_venta);
    this->box_venta.append(this->lbl_mensaje_fin);
    this->box_venta.set_spacing(20);

    this->lbl_mensaje_fin.set_visible(false);

    this->set_child(box_venta);
}