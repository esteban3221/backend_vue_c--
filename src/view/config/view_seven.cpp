#include "view_seven.hpp"

void view_seven::init_widgets()
{
    
    auto contenedor_bill = this->nd.get_niveles_bill();

    int sum_bill_cant = 0;
    int sum_bill_recy = 0;

    Helper::Validator validator;

    auto m1 = validator.PostAPIRequestAsync("ConnectDevice",validator.BILL_VALIDATOR, "").get();
    auto m2 = validator.PostAPIRequestAsync("StartDevice",validator.BILL_VALIDATOR, "").get();

    auto responseBill = validator.GetAPIRequestAsync("GetAllLevels",validator.BILL_VALIDATOR).get();
    auto x = crow::json::load(responseBill);


    crow::json::rvalue levelsBill;
    if (x)
        levelsBill = x["levels"];

    // Bill
    for (size_t i = 0; i < 7; i++)
    {
        Levels::Bill::Cnt_almacenado[i] = this->builder->get_widget<Gtk::Label>("lbl_bill_cant_" + std::to_string(i));
        Levels::Bill::Cnt_Reciclador[i] = this->builder->get_widget<Gtk::Label>("lbl_bill_recy_" + std::to_string(i));

        if (i < 6)
        {
            if (levelsBill)
            {
                std::get<0>(Levels::Bill::vecu)[i] = levelsBill[i]["stored"].i();
                Levels::Bill::Cnt_almacenado[i]->set_text(std::to_string(std::get<0>(Levels::Bill::vecu)[i]));
            }
            else
            {
                std::get<0>(Levels::Bill::vecu)[i] = std::stoi(contenedor_bill["Cant_Alm"][i]);
                Levels::Bill::Cnt_almacenado[i]->set_text(contenedor_bill["Cant_Alm"][i]);
            }
            
            std::get<1>(Levels::Bill::vecu)[i] = std::stoi(contenedor_bill["Cant_Recy"][i]);

            sum_bill_cant += std::get<0>(Levels::Bill::vecu)[i];
            sum_bill_recy += std::get<1>(Levels::Bill::vecu)[i];

            
            Levels::Bill::Cnt_Reciclador[i]->set_text(contenedor_bill["Cant_Recy"][i]);

            Levels::Bill::Niveles_inmobilidad[i] = this->builder->get_widget<Gtk::SpinButton>("lbl_bill_spin_" + std::to_string(i));
            Levels::Bill::Niveles_inmobilidad[i]->set_adjustment(
                Gtk::Adjustment::create(std::stoi(contenedor_bill["Nivel_Inmo"][i]), 
                0.0, 200.0, 1.0, 10.0, 0.0));
            
            Levels::Bill::Niveles_inmobilidad[i]->signal_changed().connect([this,i](){
                try
                {
                    const int LEVELS[3] = 
                    {
                        std::get<0>(Levels::Bill::vecu)[i], 
                        std::get<1>(Levels::Bill::vecu)[i], 
                        Levels::Bill::Niveles_inmobilidad[i]->get_value_as_int()
                    }; 
                    this->nd.actualiza_nivel_bill(DENOMINACION_BILL[i],LEVELS);
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }
            });
        }
    }
    Levels::Bill::Cnt_almacenado[6]->set_text(std::to_string(sum_bill_cant));
    Levels::Bill::Cnt_Reciclador[6]->set_text(std::to_string(sum_bill_recy));

    validator.PostAPIRequestAsync("ConnectDevice",validator.COIN_VALIDATOR, "").get();
    validator.PostAPIRequestAsync("StartDevice",validator.COIN_VALIDATOR, "").get();

    auto responseCoin = validator.GetAPIRequestAsync("GetAllLevels",validator.COIN_VALIDATOR).get();
    auto contenedor_coin = this->nd.get_niveles_coin();
    auto y = crow::json::load(responseCoin);

    crow::json::rvalue levelsCoin;
    if (y)
        levelsCoin = y["levels"];

    // Coin
    int sum_coin_cant = 0;
    int sum_coin_recy = 0;

    for (size_t i = 0; i < 5; i++)
    {
        Levels::Coin::Cnt_almacenado[i] = this->builder->get_widget<Gtk::Label>("lbl_coin_cnt_" + std::to_string(i));
        Levels::Coin::Cnt_Reciclador[i] = this->builder->get_widget<Gtk::Label>("lbl_coin_recy_" + std::to_string(i));

        if (i < 4)
        {

            if (y)
            {
                std::get<0>(Levels::Coin::vecu)[i] = levelsCoin[i]["stored"].i();
                Levels::Coin::Cnt_almacenado[i]->set_text(std::to_string(std::get<0>(Levels::Coin::vecu)[i]));
            }
            else
            {
                std::get<0>(Levels::Coin::vecu)[i] = std::stoi(contenedor_coin["Cant_Alm"][i]);
                Levels::Coin::Cnt_almacenado[i]->set_text(contenedor_coin["Cant_Alm"][i]);
            }

            std::get<1>(Levels::Coin::vecu)[i] = std::stoi(contenedor_coin["Cant_Recy"][i]);

            sum_coin_cant += std::get<0>(Levels::Coin::vecu)[i];
            sum_coin_recy += std::get<1>(Levels::Coin::vecu)[i];

            Levels::Coin::Cnt_Reciclador[i]->set_text(contenedor_coin["Cant_Recy"][i]);

            Levels::Coin::Niveles_inmobilidad[i] = this->builder->get_widget<Gtk::SpinButton>("lbl_coin_spin_" + std::to_string(i));
            Levels::Coin::Niveles_inmobilidad[i]->set_adjustment(Gtk::Adjustment::create(std::stoi(contenedor_coin["Nivel_Inmo"][i]), 0.0, 200.0, 1.0, 10.0, 0.0));

            Levels::Coin::Niveles_inmobilidad[i]->signal_changed().connect([this,i](){
                try
                {
                    const int LEVELS[3] = 
                    {
                        std::get<0>(Levels::Coin::vecu)[i], 
                        std::get<1>(Levels::Coin::vecu)[i], 
                        Levels::Coin::Niveles_inmobilidad[i]->get_value_as_int()
                    }; 
                    this->nd.actualiza_nivel_coin(this->DENOMINACION_COIN[i],LEVELS);
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }
            });
        }
    }

    Levels::Coin::Cnt_almacenado[4]->set_text(std::to_string(sum_coin_cant));
    Levels::Coin::Cnt_Reciclador[4]->set_text(std::to_string(sum_coin_recy));
}

view_seven::view_seven(/* args */)
{
    this->builder->add_from_string(this->PAGE7);
    this->init_widgets();

    this->box_page_7 = this->builder->get_widget<Gtk::Box>("box_page_7");
    this->Wrap_box.set_child(*box_page_7);
    this->Wrap_box.set_hexpand();

    this->append(this->Wrap_box);
}

view_seven::~view_seven()
{
}
