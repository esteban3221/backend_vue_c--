#include "venta.controller.hpp"

venta_controller::venta_controller(crow::App<crow::CookieParser, Session> &app_, Gtk::Stack &main_stack_, Gtk::Box *&box_main_) : app(app_),
                                                                                                                                  main_stack(main_stack_),
                                                                                                                                  box_main(box_main_)
{
    this->init_ui();
    dispatcher.connect(sigc::mem_fun(*this, &venta_controller::on_dispatcher_emit));

    // test no requieren token EXCLUSIVO DE TEST
    // CROW_ROUTE(app, "/test/venta").methods("POST"_method)(sigc::mem_fun(*this, &venta_controller::Test_venta));

    CROW_ROUTE(app, "/accion/venta").methods("POST"_method)(sigc::mem_fun(*this, &venta_controller::venta));
    CROW_ROUTE(app, "/accion/pago").methods("POST"_method)(sigc::mem_fun(*this, &venta_controller::refound));
    CROW_ROUTE(app, "/accion/cambioM").methods("POST"_method)(sigc::mem_fun(*this, &venta_controller::refound));
}

// crow::response venta_controller::Test_venta(const crow::request &req)
// {
//     auto x = crow::json::load(req.body);
//     if (!x || !x["value"])
//     {
//         this->dispatch_to_gui([this]
//                               { this->main_stack.set_visible_child(*box_main); });
//         return crow::response(crow::status::BAD_REQUEST);
//     }
//     Helper::Validator validator;
//     if (!start_payment(validator))
//     {
//         handle_payment_error();
//         return crow::response(crow::status::CONFLICT, "{\"status\":\"Error al inicializar los validadores\"}");
//     }
//     this->dispatch_to_gui([this] { this->main_stack.set_visible_child(*this); });
//     auto total = x["value"].i() / 100;
//     std::this_thread::sleep_for(std::chrono::milliseconds(1));
//     reset_gui();
//     update_total(total);
//     initialize_timeout(total);
//     if (!process_payment(validator, total))
//     {
//         handle_timeout();
//         return crow::response(crow::status::NO_CONTENT, "{\"status\":\"Timeout\"}");
//     }
//     finalize_payment(validator, total);
//     return crow::response(crow::status::OK, "{\"status\":\"Ok\"}");
// }

void venta_controller::reset_gui()
{
    this->dispatch_to_gui([this]
                          {
        this->lbl_mensaje_fin.set_text("Gracias por su compra.\n    No olvide su ticket");
        this->lbl_mensaje_fin.set_visible(false);
        this->ety_recibido.set_text("");
        this->ety_faltante.set_text("");
        this->ety_cambio.set_text("");
        this->btn_timeout_cancel.set_visible(false);
        this->btn_timeout_retry.set_visible(false); });
}

void venta_controller::update_total(int total)
{
    this->dispatch_to_gui([this, total]
                          { this->ety_monto_total.set_text(std::to_string(total)); });
}

void venta_controller::initialize_timeout(int total)
{
    auto start = std::chrono::steady_clock::now();
    auto end = start + std::chrono::seconds(5 * 60 + 2);
    this->timeout.store(false);
    this->payment_completed.store(false);

    this->M_timeout_venta = Glib::signal_timeout().connect([this, start, end]() -> bool
                                                           {
        auto now = std::chrono::steady_clock::now();
        auto remaining = std::chrono::duration_cast<std::chrono::seconds>(end - now).count();

        this->dispatch_to_gui([this, remaining] {
            this->lbl_timeout.set_text("Tiempo Restante: " + Helper::System::formatTime(remaining));
        });

        bool still_running = (remaining > 0 && !this->payment_completed.load());
        if (!still_running) {
            this->timeout.store(true);
        }

        return still_running; }, 1000);
}

bool venta_controller::start_payment(Helper::Validator &validator)
{
    if (!validator.startPay())
    {
        this->lbl_mensaje_fin.set_text("!Error al inicializar los validadores¡");
        this->lbl_mensaje_fin.set_visible();
        this->M_timeout_venta.disconnect();
        this->timeout.store(true);
        validator.pollInit.store(false);
        validator.stopPay();
        std::this_thread::sleep_for(std::chrono::seconds(3));
        this->dispatch_to_gui([this]
                              { this->main_stack.set_visible_child(*box_main); });
        return false;
    }
    validator.pollInit.store(true);
    validator.sumInput.store(0);
    return true;
}

bool venta_controller::process_payment(Helper::Validator &validator, int total)
{
    while (validator.sumInput.load() < total)
    {
        if (this->timeout.load())
        {
            this->lbl_mensaje_fin.set_text("Cancelación por exceso de tiempo.");
            this->lbl_mensaje_fin.set_visible();
            std::this_thread::sleep_for(std::chrono::seconds(3));
            this->dispatch_to_gui([this]
                                  { this->main_stack.set_visible_child(*box_main); });
            return false;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        this->dispatch_to_gui([this, total, &validator]
                              {
            auto sum = validator.sumInput.load();
            this->ety_recibido.set_text(std::to_string(sum));
            this->ety_faltante.set_text(sum < total ? std::to_string(total - sum) : "0"); });
    }
    return true;
}

void venta_controller::finalize_payment(Helper::Validator &validator, int total)
{
    this->payment_completed.store(false);
    this->M_timeout_venta.disconnect();

    this->dispatch_to_gui([this, total, &validator]
                          {
        if (validator.sumInput.load() > total) {
            auto cambio = validator.sumInput.load() - total;
            this->ety_cambio.set_text(std::to_string(cambio));
            validator.calculateChange(cambio);
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
        this->lbl_mensaje_fin.set_visible(true); });

    std::this_thread::sleep_for(std::chrono::seconds(3));
    this->dispatch_to_gui([this]
                          { this->main_stack.set_visible_child(*box_main); });
    validator.stopPay();
}

void venta_controller::handle_payment_error(Acciones &accion, const std::pair<int, std::string> &status, const std::string &total, const std::string &sum)
{
    this->lbl_mensaje_fin.set_text("¡Error al inicializar los validadores!");
    this->lbl_mensaje_fin.set_visible();
    this->M_timeout_venta.disconnect();
    this->timeout.store(true);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    this->dispatch_to_gui([this]
                          { this->main_stack.set_visible_child(*box_main); });

    accion.insertLog(status.second,
                     Action::Type::VENTA,
                     total,
                     sum,
                     std::to_string(stoi(sum) - stoi(total)),
                     "ERROR");
}

void venta_controller::handle_timeout(Acciones &accion, const std::pair<int, std::string> &status, const std::string &total, const std::string &sum)
{
    this->lbl_mensaje_fin.set_text("Cancelación por exceso de tiempo.");
    this->lbl_mensaje_fin.set_visible();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    this->dispatch_to_gui([this]
                          { this->main_stack.set_visible_child(*box_main); });

    accion.insertLog(status.second,
                     Action::Type::VENTA,
                     total,
                     sum,
                     std::to_string(stoi(sum) - stoi(total)),
                     "TIMEOUT");
}

// crow::response venta_controller::venta(const crow::request &req)
// {
//     auto &session = app.get_context<Session>(req);
//     if (auto status = Helper::User::validPermissions(req, session,
//                                                      {Helper::User::Rol::Venta,
//                                                       Helper::User::Rol::Pago});
//         status.first != crow::status::OK)
//     {
//         return crow::response(status.first);
//     }
//     else
//     {
//         auto x = crow::json::load(req.body);
//         if (!x || !x["value"])
//         {
//             this->dispatch_to_gui([this]
//                                   { this->main_stack.set_visible_child(*box_main); });
//             return crow::response(crow::status::BAD_REQUEST);
//         }
//         Acciones accion;
//         this->dispatch_to_gui([this]
//                               { this->main_stack.set_visible_child(*this); });
//         auto total = x["value"].i() / 100;
//         std::this_thread::sleep_for(std::chrono::milliseconds(3));
//         this->dispatch_to_gui([this]
//                               {
//         this->lbl_mensaje_fin.set_text("Gracias por su compra.\n    No olvide su ticket");
//         this->lbl_mensaje_fin.set_visible(false);
//         this->ety_recibido.set_text("");
//         this->ety_faltante.set_text("");
//         this->ety_cambio.set_text("");
//         this->btn_timeout_cancel.set_visible(false);
//         this->btn_timeout_retry.set_visible(false); });
//         this->dispatch_to_gui([this, total]
//                               { this->ety_monto_total.set_text(std::to_string(total)); });
//         auto start = std::chrono::steady_clock::now();
//         auto end = start + std::chrono::seconds(5 * 60 + 2);
//         this->timeout.store(false);
//         this->payment_completed.store(false);
//         this->M_timeout_venta = Glib::signal_timeout().connect([this, start, end]() -> bool
//                                                                {
//         auto now = std::chrono::steady_clock::now();
//         auto remaining = std::chrono::duration_cast<std::chrono::seconds>(end - now).count();
//         this->dispatch_to_gui([this, remaining] {
//             this->lbl_timeout.set_text("Tiempo Restante: " + Helper::System::formatTime(remaining));
//         });
//         bool still_running = (remaining > 0 && !this->payment_completed.load());
//         if (!still_running)
//             this->timeout.store(true);
//         return still_running; }, 1000);
//         long sum = 0;
//         unsigned channel;
//         while (sum < total)
//         {
//             if (this->timeout.load())
//             {
//                 this->lbl_mensaje_fin.set_text("Se alcanzo el tiempo maximo de espera.");
//                 this->lbl_mensaje_fin.set_visible();
//                 this->btn_timeout_cancel.set_visible();
//                 this->btn_timeout_retry.set_visible();
//                 accion.insertLog(
//                     status.second,
//                     Action::Type::VENTA,
//                     std::to_string(total),
//                     std::to_string(sum),
//                     std::to_string(sum - total),
//                     "TIMEOUT");
//                 aqui tambien se va a pagos pendientes.
//                 return crow::response(crow::status::GATEWAY_TIMEOUT, "Timeout");
//             }
//             std::this_thread::sleep_for(std::chrono::seconds(2));
//             sum += 5;
//             this->dispatch_to_gui([this, sum, total]
//                                   {
//             this->ety_recibido.set_text(std::to_string(sum));
//             this->ety_faltante.set_text(sum < total ? std::to_string(total - sum) : "0"); });
//         }
//         this->payment_completed.store(false);
//         this->M_timeout_venta.disconnect();
//         this->dispatch_to_gui([this, sum, total]
//                               {
//         if (sum > total)
//             this->ety_cambio.set_text(std::to_string(sum - total));
//         this->lbl_mensaje_fin.set_visible(true); });
//         accion.insertLog(
//             status.second,
//             Action::Type::VENTA,
//             std::to_string(total),
//             std::to_string(sum),
//             std::to_string(sum - total),
//             "Completado");
//         std::this_thread::sleep_for(std::chrono::seconds(3));
//         this->dispatch_to_gui([this]
//                               { this->main_stack.set_visible_child(*box_main); });
//     }
//     return crow::response();
// }

crow::response venta_controller::venta(const crow::request &req)
{
    auto &session = app.get_context<Session>(req);

    if (auto status = Helper::User::validPermissions(req, session, {Helper::User::Rol::Venta});
        status.first != crow::status::OK)
    {
        return crow::response(status.first);
    }
    else
    {
        auto x = crow::json::load(req.body);
        if (!x || !x["value"])
        {
            this->dispatch_to_gui([this]
                                  { this->main_stack.set_visible_child(*box_main); });
            return crow::response(crow::status::BAD_REQUEST);
        }

        Helper::Validator validator;
        Acciones accion;
        this->lbl_titulo.set_text("Venta");
        this->lbl_titulo.set_css_classes({"title-1"});
        if (!start_payment(validator))
        {
            handle_payment_error(accion, status, std::to_string(x["value"].i() / 100), "0");
            return crow::response(crow::status::CONFLICT, "{\"status\":\"Error al inicializar los validadores\"}");
        }

        this->dispatch_to_gui([this]
                              { this->main_stack.set_visible_child(*this); });

        int total = x["value"].i() / 100;
        reset_gui();
        update_total(total);
        initialize_timeout(total);

        if (!process_payment(validator, total))
        {
            handle_timeout(accion, status, std::to_string(total), std::to_string(validator.sumInput.load()));
            return crow::response(crow::status::NO_CONTENT, "{\"status\":\"Timeout\"}");
        }

        finalize_payment(validator, total);

        accion.insertLog(status.second,
                         Action::Type::VENTA,
                         std::to_string(total),
                         std::to_string(validator.sumInput.load()),
                         std::to_string(validator.sumInput.load() - total),
                         "Completado");

        crow::json::wvalue responseJson;
        responseJson["total"] = std::to_string(total);
        responseJson["recibido"] = std::to_string(validator.sumInput.load());
        responseJson["cambio"] = std::to_string(validator.sumInput.load() - total);

        return crow::response(crow::status::OK, responseJson);
    }
}

crow::response venta_controller::refill(const crow::request &req)
{
    auto &session = app.get_context<Session>(req);

    if (auto status = Helper::User::validPermissions(req, session, {Helper::User::Rol::Venta, Helper::User::Rol::Pago});
        status.first != crow::status::OK)
    {
        return crow::response(status.first);
    }

    return crow::response();
}

crow::response venta_controller::refound(const crow::request &req)
{
    auto &session = app.get_context<Session>(req);

    auto status = Helper::User::validPermissions(req, session, {Helper::User::Rol::Pago});
    if (status.first != crow::status::OK)
    {
        return crow::response(status.first);
    }
    auto x = crow::json::load(req.body);
    if (!x || !x["value"])
    {
        return crow::response(crow::status::BAD_REQUEST);
    }

    Helper::Validator validator;
    Acciones accion;

    start_payment(validator);
    this->lbl_titulo.set_text("Pago");
    this->lbl_titulo.set_css_classes({"title-1"});

    accion.insertLog(status.second,
                     Action::Type::PAGO,
                     "0",
                     std::to_string(validator.sumInput.load()),
                     std::to_string(x["value"].i() / 100),
                     "Completado");

    auto status_ = validator.calculateChange(x["value"].i() / 100);
    std::this_thread::sleep_for(std::chrono::seconds(10));
    return status_;
}
crow::response venta_controller::cambioM(const crow::request &req)
{
    auto &session = app.get_context<Session>(req);

    auto status = Helper::User::validPermissions(req, session, {Helper::User::Rol::Pago});
    if (status.first != crow::status::OK)
    {
        return crow::response(status.first);
    }
    auto x = crow::json::load(req.body);
    if (!x || !x["value"])
    {
        return crow::response(crow::status::BAD_REQUEST);
    }

    Helper::Validator validator;
    Acciones accion;

    start_payment(validator);
    this->lbl_titulo.set_text("Cambio");
    this->lbl_titulo.set_css_classes({"title-1"});

    accion.insertLog(status.second,
                     Action::Type::CAMBIO,
                     "0",
                     std::to_string(validator.sumInput.load()),
                     std::to_string(x["value"].i() / 100),
                     "Completado");

    auto status_ = validator.calculateChange(x["value"].i() / 100);
    std::this_thread::sleep_for(std::chrono::seconds(10));
    return status_;
}

void venta_controller::dispatch_to_gui(std::function<void()> func)
{
    {
        std::lock_guard<std::mutex> lock(dispatch_queue_mutex);
        dispatch_queue.push(func);
    }
    dispatcher.emit();
}

void venta_controller::on_dispatcher_emit()
{
    std::function<void()> func;
    {
        std::lock_guard<std::mutex> lock(dispatch_queue_mutex);
        if (!dispatch_queue.empty())
        {
            func = dispatch_queue.front();
            dispatch_queue.pop();
        }
    }
    if (func)
    {
        func();
    }
}

venta_controller::~venta_controller()
{
    M_timeout_venta.disconnect();
    timeout.store(true);
}

// crow::response venta_controller::Test_venta(const crow::request &req)
// {
//     this->m_Dispatcher.emit();
//     auto x = crow::json::load(req.body);
//     if (!x || !x["value"])
//     {
//         this->main_stack.set_visible_child(*box_main);
//         return crow::response(crow::status::BAD_REQUEST);
//     }
//     this->main_stack.set_visible_child(*this);
//     auto total = x["value"].i() / 100;
//     std::this_thread::sleep_for(std::chrono::milliseconds(3));
//     this->ety_recibido.set_text("");
//     this->ety_faltante.set_text("");
//     this->ety_cambio.set_text("");
//     this->lbl_mensaje_fin.set_visible(false);
//     this->ety_monto_total.set_text(std::to_string(total));
//     if (!this->init_perifericos())
//     {
//         this->main_stack.set_visible_child(*box_main);
//         return crow::response(crow::status::INTERNAL_SERVER_ERROR, "Periferico no se encuentra o esta ocupado.");
//     }
//     long sum = 0;
//     unsigned channel;
//     while (sum < total)
//     {
//         if (ssp_poll(this->ssp_setup_bill, &this->poll) != SSP_RESPONSE_OK)
//             printf("SSP_POLL_ERROR\n");
//         if (ssp_poll(this->ssp_setup_coin, &this->poll) != SSP_RESPONSE_OK)
//             printf("SSP_POLL_ERROR\n");
//         for (int i = 0; i < this->poll.event_count; ++i)
//         {
//             switch (this->poll.events[i].event)
//             {
//             case SSP_POLL_RESET:
//                 printf("Unit Reset\n");
//                 break;
//             case SSP_POLL_READ:
//                 if (this->poll.events[i].data > 0)
//                     channel = this->poll.events[i].data;
//                 break;
//             case SSP_POLL_CREDIT:
//                 channel = this->poll.events[i].data;
//                 break;
//             case SSP_POLL_STACKED:
//             {
//                 printf("Stacked %d MXN$\n", this->DENOMINATION[channel]);
//                 sum += this->DENOMINATION[channel];
//                 channel = 0;
//                 this->ety_recibido.set_text(std::to_string(sum));
//                 (sum < total) ? this->ety_faltante.set_text(std::to_string(total - sum)) : this->ety_faltante.set_text("0");
//                 break;
//             }
//             case SSP_POLL_FRAUD_ATTEMPT:
//                 printf("Fraud Attempt %ld\n", this->poll.events[i].data);
//                 break;
//             case SSP_POLL_STACKER_FULL:
//             {
//                 printf("Stacker Full\n");
//                 //return crow::response(crow::status::CONFLICT, "Stacker Full");
//                 break;
//             }
//             }
//         }
//         std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     }
//     ssp_disable(this->ssp_setup_bill);
//     ssp_disable(this->ssp_setup_coin);
//     CloseSSPPort(this->port_bill);
//     CloseSSPPort(this->port_coin);
//     //ternaria no funciona en un thread hacia modificacion de gui
//     std::this_thread::sleep_for(std::chrono::milliseconds(3));
//     if (sum > total)
//         this->ety_cambio.set_text(std::to_string(sum-total));
//     this->lbl_mensaje_fin.set_visible();
//     std::this_thread::sleep_for(std::chrono::seconds(3));
//     this->main_stack.set_visible_child(*box_main);
//     this->m_Dispatcher.emit();
//     return crow::response(crow::status::OK, "OK");
// }
// bool venta_controller::init_perifericos()
// {
//     this->port_bill = OpenSSPPort("/dev/ttyUSB0");
//     this->port_coin = OpenSSPPort("/dev/ttyUSB1");
//     this->ssp_setup_bill.port = port_bill;
//     this->ssp_setup_coin.port = port_coin;
//     this->ssp_setup_bill.SSPAddress = 0;
//     this->ssp_setup_coin.SSPAddress = 16;
//     this->ssp_setup_bill.Timeout = this->ssp_setup_coin.Timeout = 1000;
//     this->ssp_setup_bill.EncryptionStatus = this->ssp_setup_coin.EncryptionStatus = NO_ENCRYPTION;
//     this->ssp_setup_bill.RetryLevel = this->ssp_setup_coin.RetryLevel = 3;
//     return ((ssp_setup_encryption(&ssp_setup_bill, 0x0123456701234567L) == SSP_RESPONSE_OK) &&
//             (ssp_enable(ssp_setup_bill) == SSP_RESPONSE_OK) &&
//             (ssp_set_inhibits(ssp_setup_bill, 0xFF, 0xFF) == SSP_RESPONSE_OK));
// }
void venta_controller::init_ui()
{
    this->lbl_titulo.set_text("Test Venta");
    this->lbl_titulo.set_css_classes({"title-1"});

    this->box_venta.append(lbl_titulo);
    this->box_venta.set_orientation(Gtk::Orientation::VERTICAL);
    this->box_venta.set_margin(15);

    this->btn_timeout_cancel.set_label("Cancelar");
    this->btn_timeout_retry.set_label("Reintentar");

    this->box_action_timeout.append(this->lbl_timeout);
    this->box_action_timeout.append(this->btn_timeout_cancel);
    this->box_action_timeout.append(this->btn_timeout_retry);
    this->box_action_timeout.set_orientation(Gtk::Orientation::HORIZONTAL);
    this->box_action_timeout.set_homogeneous();

    this->lbl_mensaje_fin.set_css_classes({"title-2", "dim-label"});
    this->lbl_monto_total.set_text("Total");
    this->lbl_monto_total.set_css_classes({"title-3"});
    this->lbl_recibido.set_text("Recibido");
    this->lbl_recibido.set_css_classes({"title-3"});
    this->lbl_faltante.set_text("Falta");
    this->lbl_faltante.set_css_classes({"title-3"});
    this->lbl_cambio.set_text("Cambio");
    this->lbl_cambio.set_css_classes({"title-3"});
    this->lbl_timeout.set_text("Restante: 00:00");

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
    this->box_venta.append(this->box_action_timeout);
    this->box_venta.set_spacing(20);

    this->lbl_mensaje_fin.set_visible(false);

    this->set_child(box_venta);
}