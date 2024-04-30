#include "test.controller.hpp"

test_controller::test_controller(crow::SimpleApp &app_, Gtk::Stack &main_stack_, Gtk::Box &contenedor_main_) : app(app_), main_stack(main_stack_), contenedor_main(contenedor_main_)
{
    this->append(lbl_hijo);

    // registri de apis
    CROW_ROUTE(app, "/rutaTest")(sigc::mem_fun(*this, &test_controller::runTest));
    CROW_ROUTE(app, "/add_json").methods("POST"_method)(sigc::mem_fun(*this, &test_controller::testjson));
}

std::string test_controller::runTest()
{
    this->main_stack.set_visible_child(*this);
    this->lbl_hijo.set_text("ruta hija");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    this->main_stack.set_visible_child(contenedor_main);
    return "ruta hija";
}

crow::response test_controller::testjson(const crow::request &req)
{
    auto x = crow::json::load(req.body);
    if (!x)
        return crow::response(crow::status::EXPECTATION_FAILED); // same as crow::response(400)

    auto lambda = [](crow::json::rvalue x) -> int
    { return x["a"].i() + x["b"].i(); };
    std::ostringstream os;
    os << lambda(x);
    return crow::response{os.str()};
}

test_controller::~test_controller()
{
}
