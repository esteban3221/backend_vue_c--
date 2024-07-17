#include "helper.hpp"

namespace Helper
{
    namespace System
    {
        std::string exec(const char *cmd)
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

        std::string formatTime(int seconds)
        {
            int minutes = seconds / 60;
            int remainingSeconds = seconds % 60;

            std::ostringstream oss;
            oss << std::setw(2) << std::setfill('0') << minutes << ":"
                << std::setw(2) << std::setfill('0') << remainingSeconds;
            return oss.str();
        }

        /// @brief
        /// @param title
        /// @param subtitle
        /// @param type dialog-information
        void showNotify(const char *title, const char *subtitle, const char *type)
        {
            if (BinaryDB::select_<int>(9) == true)
            {
                notify_init("MaxiCajero");
                NotifyNotification *Notify = notify_notification_new(title, subtitle, type);
                notify_notification_show(Notify, NULL);
                g_object_unref(G_OBJECT(Notify));
                notify_uninit();
            }
        }
    } // namespace System

    namespace User
    {
        crow::status validUser(const crow::request &req, Session::context &session, std::string &userName)
        {
            auto auth_header = req.get_header_value("Authorization");
            if (auth_header.empty() || auth_header.substr(0, 7) != "Bearer ")
                return crow::status::NOT_FOUND;

            auto keys = session.keys();
            for (const auto &key : keys)
            {
                userName = key;
                if (auth_header.substr(7) == session.string(key))
                    return crow::status::OK;
            }
            return crow::status::UNAUTHORIZED;
        }

        std::pair<crow::status, std::string> validPermissions(const crow::request &req, Session::context &session, const std::vector<Rol> &vecRol)
        {
            std::string userName;

            if (auto status = validUser(req, session, userName); status != crow::status::OK)
                return {status, userName};
            else
            {
                Model::usuarios_roles rol;
                auto rolesPorUsuario = rol.obten_roles_by_usuarios(userName);
                std::unordered_set<Rol> rolesNecesarios(vecRol.begin(), vecRol.end());
                for (const auto &rol_ : rolesPorUsuario["id_rol"])
                {
                    Helper::User::Rol rolUsuarioEnum = static_cast<Helper::User::Rol>(std::stoi(rol_));

                    if (rolesNecesarios.find(rolUsuarioEnum) != rolesNecesarios.end())
                    {
                        rolesNecesarios.erase(rolUsuarioEnum); // Remueve el rol encontrado de rolesNecesarios
                    }
                }
                if (rolesNecesarios.empty())
                {
                    return {crow::status::OK, userName}; // Todos los roles necesarios están presentes, devuelve OK
                }
            }
            return {crow::status::UNAUTHORIZED, userName};
        }

    } // namespace User

    size_t Validator::write_data(void *buffer, size_t size, size_t nmemb, void *userp)
    {
        ((std::string *)userp)->append((char *)buffer, size * nmemb);
        return size * nmemb;
    }

    std::pair<long, std::string> Validator::PostAPIRequest(const std::string &uri, const std::string &deviceID, const std::string &jsonData = "")
    {
        CURL *curl = curl_easy_init();
        std::string response_string;
        long response_code;
        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            struct curl_slist *headers = nullptr;                                   // Initialise to nullptr
            headers = curl_slist_append(headers, "Content-Type: application/json"); // Set Content-Type to application/json
            std::string url = "http://localhost:5000/api/CashDevice/" + uri + "?deviceID=" + deviceID;
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); // Set headers for the request
            if (!jsonData.empty())
            {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
            }
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK)
            {
                std::cerr << "CURL failed: " << curl_easy_strerror(res) << std::endl;
            }
            else
            {
                curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
                // response_string = "Response Code: " + to_string(response_code) + " - " + response_string;
            }

            curl_slist_free_all(headers); // Clean up the headers list
            curl_easy_cleanup(curl);
        }
        return {response_code, response_string};
    }

    std::string Validator::GetAPIRequest(const std::string &uri, const std::string &deviceID)
    {
        CURL *curl = curl_easy_init();
        std::string response_string;
        if (curl)
        {
            // Construct the URL with the device ID query parameter
            std::string url = "http://localhost:5000/api/CashDevice/" + uri + "?deviceID=" + deviceID;

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK)
            {
                std::cerr << "CURL GET request failed: " << curl_easy_strerror(res) << std::endl;
                response_string = ""; // Return an empty std::string on failure
            }
            curl_easy_cleanup(curl);
        }
        return response_string;
    }

    std::future<std::pair<long, std::string>> Validator::PostAPIRequestAsync(const std::string &uri, const std::string &deviceID, const std::string &jsonData = "")
    {
        return async(std::launch::async, &Validator::PostAPIRequest, this, uri, deviceID, jsonData);
    }
    // Asynchronous function to make a GET request
    std::future<std::string> Validator::GetAPIRequestAsync(const std::string &uri, const std::string &deviceID)
    {
        return async(std::launch::async, &Validator::GetAPIRequest, this, uri, deviceID);
    }

    // Function to handle device status
    void Validator::handleDeviceStatus(const std::string &state, const std::string &deviceID)
    {
        if (state == "ESCROW")
        {
            auto acceptEscrowResponse = PostAPIRequestAsync("AcceptFromEscrow", deviceID);
            std::cout << acceptEscrowResponse.get().second << std::endl;
        }
    }

    // Function to continuously poll the device status every 200 milliseconds
    void Validator::pollDeviceStatus(const std::string &deviceID)
    {
        while (this->pollInit.load())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            std::string deviceStatusJson = GetAPIRequest("GetDeviceStatus", deviceID);
            if (!deviceStatusJson.empty())
            {
                try
                {
                    std::cout << " TEST " << deviceStatusJson << std::endl;
                    auto responseObjects = crow::json::load(deviceStatusJson);

                    for (const auto &responseObject : responseObjects)
                    {
                        std::string state;

                        if (responseObject.has("stateAsString"))
                        {
                            state = responseObject["stateAsString"].s();
                        }
                        else if (responseObject.has("eventTypeAsString"))
                        {
                            state = responseObject["eventTypeAsString"].s();
                            if (responseObject["eventTypeAsString"] == "STACKED")
                                std::cout << "Se recibio :" << responseObject["value"].i() / 100 << '\n';
                            if (responseObject["eventTypeAsString"] == "COIN_CREDIT")
                                std::cout << "Se recibio :" << responseObject["value"].i() / 100 << '\n';

                                sumInput.store(sumInput.load() + (responseObject["value"].i() / 100));
                        }

                        handleDeviceStatus(state, deviceID);
                    }
                }
                catch (const std::exception &e)
                {
                    std::cerr << "JSON parsing error: " << e.what() << '\n';
                }
            }
        }
    }

    // Function to process commands from the terminal
    void Validator::processCommand(const std::string &command, const std::string &deviceID)
    {
        std::istringstream iss(command);
        std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};

        if (tokens.empty())
            return;

        const std::string &commandType = tokens[0];

        try
        {
            if (commandType == "PayoutByDenomination")
            {
                if (tokens.size() != 5)
                {
                    std::cout << "Usage: PayoutByDenomination <deviceID> <value> <countryCode> <numNotes>" << std::endl;
                    return;
                }
                std::string value = tokens[2];
                std::string countryCode = tokens[3];
                std::string numNotes = tokens[4];
                std::string payoutData = R"({"Value":)" + value + R"(,"CountryCode":")" + countryCode + R"(","NumNotes":)" + numNotes + R"(})";
                auto response = PostAPIRequestAsync("PayoutByDenomination", deviceID, payoutData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "DisablePayout")
            {
                auto response = PostAPIRequestAsync("DisablePayout", deviceID);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "GetCompleteCashDevice")
            {
                auto response = GetAPIRequestAsync("GetCompleteCashDevice", deviceID);
                std::cout << response.get() << std::endl;
            }
            else if (commandType == "GetDeviceStatus")
            {
                auto response = GetAPIRequestAsync("GetDeviceStatus", deviceID);
                std::cout << response.get() << std::endl;
            }
            else if (commandType == "GetAllLevels")
            {
                auto response = GetAPIRequestAsync("GetAllLevels", deviceID);
                std::cout << response.get() << std::endl;
            }
            else if (commandType == "SetDenominationInhibitSingleCurrency")
            {
                if (tokens.size() != 3)
                {
                    std::cout << "Usage: SetDenominationInhibitSingleCurrency <deviceID> <value> <inhibit>" << std::endl;
                    return;
                }
                std::string value = tokens[2];
                std::string inhibit = tokens[3];
                std::string requestData = R"({"Value":)" + value + R"(,"Inhibit":)" + inhibit + R"(})";
                auto response = PostAPIRequestAsync("SetDenominationInhibitSingleCurrency", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "SetDenominationInhibitByIndex")
            {
                if (tokens.size() != 3)
                {
                    std::cout << "Usage: SetDenominationInhibitByIndex <deviceID> <index> <inhibit>" << std::endl;
                    return;
                }
                std::string index = tokens[2];
                std::string inhibit = tokens[3];
                std::string requestData = R"({"Index":)" + index + R"(,"Inhibit":)" + inhibit + R"(})";
                auto response = PostAPIRequestAsync("SetDenominationInhibitByIndex", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "SetDenominationInhibit")
            {
                if (tokens.size() != 4)
                {
                    std::cout << "Usage: SetDenominationInhibit <deviceID> <value> <countryCode> <inhibit>" << std::endl;
                    return;
                }
                std::string value = tokens[2];
                std::string countryCode = tokens[3];
                std::string inhibit = tokens[4];
                std::string requestData = R"({"ValueCountryCode":{"Value":)" + value + R"(,"CountryCode":")" + countryCode + R"("},"Inhibit":)" + inhibit + R"(})";
                auto response = PostAPIRequestAsync("SetDenominationInhibit", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "DispenseValueSingleCurrency")
            {
                if (tokens.size() != 3)
                {
                    std::cout << "Usage: DispenseValueSingleCurrency <deviceID> <value>" << std::endl;
                    return;
                }
                std::string value = tokens[2];
                std::string requestData = value;
                auto response = PostAPIRequestAsync("DispenseValueSingleCurrency", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "DispenseValue")
            {
                if (tokens.size() != 4)
                {
                    std::cout << "Usage: DispenseValue <deviceID> <value> <countryCode>" << std::endl;
                    return;
                }
                std::string value = tokens[2];
                std::string countryCode = tokens[3];
                std::string requestData = R"({"Value":)" + value + R"(,"CountryCode":")" + countryCode + R"("})";
                auto response = PostAPIRequestAsync("DispenseValue", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "InitialiseDevice")
            {
                auto response = PostAPIRequestAsync("InitialiseDevice", deviceID);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "OpenDevice")
            {
                if (tokens.size() != 4)
                {
                    std::cout << "Usage: OpenDevice <deviceID> <comPort> <sspAddress> <logFilePath>" << std::endl;
                    return;
                }
                std::string comPort = tokens[2];
                std::string sspAddress = tokens[3];
                std::string logFilePath = tokens[4];
                std::string requestData = R"({"ComPort":")" + comPort + R"(","SspAddress":)" + sspAddress + R"(,"LogFilePath":")" + logFilePath + R"("})";
                auto response = PostAPIRequestAsync("OpenDevice", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "ConnectDevice")
            {
                auto response = PostAPIRequestAsync("ConnectDevice", deviceID);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "StartDevice")
            {
                auto response = PostAPIRequestAsync("StartDevice", deviceID);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "StopDevice")
            {
                auto response = PostAPIRequestAsync("StopDevice", deviceID);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "DisconnectDevice")
            {
                auto response = PostAPIRequestAsync("DisconnectDevice", deviceID);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "CloseDevice")
            {
                auto response = PostAPIRequestAsync("CloseDevice", deviceID);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "AcceptFromEscrow")
            {
                auto response = PostAPIRequestAsync("AcceptFromEscrow", deviceID);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "ReturnFromEscrow")
            {
                auto response = PostAPIRequestAsync("ReturnFromEscrow", deviceID);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "Float")
            {
                if (tokens.size() < 3)
                {
                    std::cout << "Usage: Float <deviceID> <noteCounts...>" << std::endl;
                    return;
                }
                std::vector<std::string> noteCounts(tokens.begin() + 2, tokens.end());
                std::string requestData = "[";
                for (size_t i = 0; i < noteCounts.size(); ++i)
                {
                    requestData += noteCounts[i];
                    if (i < noteCounts.size() - 1)
                        requestData += ",";
                }
                requestData += "]";
                auto response = PostAPIRequestAsync("Float", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "SetCashboxPayoutLimit")
            {
                if (tokens.size() < 3)
                {
                    std::cout << "Usage: SetCashboxPayoutLimit <deviceID> <noteCounts...>" << std::endl;
                    return;
                }
                std::vector<std::string> noteCounts(tokens.begin() + 2, tokens.end());
                std::string requestData = "[";
                for (size_t i = 0; i < noteCounts.size(); ++i)
                {
                    requestData += noteCounts[i];
                    if (i < noteCounts.size() - 1)
                        requestData += ",";
                }
                requestData += "]";
                auto response = PostAPIRequestAsync("SetCashboxPayoutLimit", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "SmartEmpty")
            {
                if (tokens.size() != 3)
                {
                    std::cout << "Usage: SmartEmpty <deviceID> <moduleNumber> <isNV4000>" << std::endl;
                    return;
                }
                std::string moduleNumber = tokens[2];
                std::string isNV4000 = tokens[3];
                std::string requestData = R"({"ModuleNumber":)" + moduleNumber + R"(,"IsNV4000":)" + isNV4000 + R"(})";
                auto response = PostAPIRequestAsync("SmartEmpty", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "EnableAcceptor")
            {
                auto response = PostAPIRequestAsync("EnableAcceptor", deviceID);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "DisableAcceptor")
            {
                auto response = PostAPIRequestAsync("DisableAcceptor", deviceID);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "SendCustomCommand")
            {
                if (tokens.size() != 4)
                {
                    std::cout << "Usage: SendCustomCommand <deviceID> <customCommandData> <customCommandDataLength>" << std::endl;
                    return;
                }
                std::string customCommandData = tokens[2];
                std::string customCommandDataLength = tokens[3];
                std::string requestData = R"({"CustomCommandData":")" + customCommandData + R"(","CustomCommandDataLength":)" + customCommandDataLength + R"(})";
                auto response = PostAPIRequestAsync("SendCustomCommand", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "EnablePayout")
            {
                auto response = PostAPIRequestAsync("EnablePayout", deviceID);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "EnableCoinMechOrFeeder")
            {
                auto response = PostAPIRequestAsync("EnableCoinMechOrFeeder", deviceID);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "ResetDevice")
            {
                auto response = PostAPIRequestAsync("ResetDevice", deviceID);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "HaltPayout")
            {
                auto response = PostAPIRequestAsync("HaltPayout", deviceID);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "Replenish")
            {
                if (tokens.size() != 3)
                {
                    std::cout << "Usage: Replenish <deviceID> <numberToReplenish>" << std::endl;
                    return;
                }
                std::string numberToReplenish = tokens[2];
                std::string requestData = numberToReplenish;
                auto response = PostAPIRequestAsync("Replenish", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "RefillMode")
            {
                if (tokens.size() != 3)
                {
                    std::cout << "Usage: RefillMode <deviceID> <enable>" << std::endl;
                    return;
                }
                std::string enable = tokens[2];
                std::string requestData = enable;
                auto response = PostAPIRequestAsync("RefillMode", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "KeyExchangeLimit32bit")
            {
                auto response = PostAPIRequestAsync("KeyExchangeLimit32bit", deviceID);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "GetUnitInfo")
            {
                auto response = GetAPIRequestAsync("GetUnitInfo", deviceID);
                std::cout << response.get() << std::endl;
            }
            else if (commandType == "GetHopperOptions")
            {
                auto response = GetAPIRequestAsync("GetHopperOptions", deviceID);
                std::cout << response.get() << std::endl;
            }
            else if (commandType == "SetHopperOptions")
            {
                if (tokens.size() != 4)
                {
                    std::cout << "Usage: SetHopperOptions <deviceID> <reg0> <reg1>" << std::endl;
                    return;
                }
                std::string reg0 = tokens[2];
                std::string reg1 = tokens[3];
                std::string requestData = R"({"Reg0":)" + reg0 + R"(,"Reg1":)" + reg1 + R"(})";
                auto response = PostAPIRequestAsync("SetHopperOptions", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "LogRawPackets")
            {
                if (tokens.size() != 3)
                {
                    std::cout << "Usage: LogRawPackets <deviceID> <rawPacketsOptionSelected>" << std::endl;
                    return;
                }
                std::string rawPacketsOptionSelected = tokens[2];
                std::string requestData = rawPacketsOptionSelected;
                auto response = PostAPIRequestAsync("LogRawPackets", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "GetGlobalErrorCode")
            {
                auto response = GetAPIRequestAsync("GetGlobalErrorCode", deviceID);
                std::cout << response.get() << std::endl;
            }
            else if (commandType == "GetServiceInformation")
            {
                if (tokens.size() != 3)
                {
                    std::cout << "Usage: GetServiceInformation <deviceID> <subCommand>" << std::endl;
                    return;
                }
                std::string subCommand = tokens[2];
                std::string requestData = subCommand;
            }
            else if (commandType == "SetServiceInformationMaintenanceReset")
            {
                if (tokens.size() != 6)
                {
                    std::cout << "Usage: SetServiceInformationMaintenanceReset <deviceID> <week1> <week2> <year1> <year2>" << std::endl;
                    return;
                }
                std::string week1 = tokens[2];
                std::string week2 = tokens[3];
                std::string year1 = tokens[4];
                std::string year2 = tokens[5];
                std::string requestData = R"({"WeekNumber1AsciiByte":)" + week1 + R"(,"WeekNumber2AsciiByte":)" + week2 + R"(,"YearNumber1AsciiByte":)" + year1 + R"(,"YearNumber2AsciiByte":)" + year2 + R"(})";
                auto response = PostAPIRequestAsync("SetServiceInformationMaintenanceReset", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "SetNoPayinCount")
            {
                if (tokens.size() != 3)
                {
                    std::cout << "Usage: SetNoPayinCount <deviceID> <count>" << std::endl;
                    return;
                }
                std::string count = tokens[2];
                std::string requestData = count;
                auto response = PostAPIRequestAsync("SetNoPayinCount", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "Purge")
            {
                auto response = PostAPIRequestAsync("Purge", deviceID);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "PurgeDevice")
            {
                if (tokens.size() != 3)
                {
                    std::cout << "Usage: PurgeDevice <deviceID> <device>" << std::endl;
                    return;
                }
                std::string device = tokens[2];
                std::string requestData = device;
                auto response = PostAPIRequestAsync("PurgeDevice", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "PurgeDeviceHopper")
            {
                if (tokens.size() != 4)
                {
                    std::cout << "Usage: PurgeDeviceHopper <deviceID> <device> <hopper>" << std::endl;
                    return;
                }
                std::string device = tokens[2];
                std::string hopper = tokens[3];
                std::string requestData = R"({"Device":)" + device + R"(,"Hopper":)" + hopper + R"(})";
                auto response = PostAPIRequestAsync("PurgeDeviceHopper", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "CoinStirWithMode")
            {
                if (tokens.size() != 4)
                {
                    std::cout << "Usage: CoinStirWithMode <deviceID> <seconds> <modeByte>" << std::endl;
                    return;
                }
                std::string seconds = tokens[2];
                std::string modeByte = tokens[3];
                std::string requestData = R"({"Seconds":)" + seconds + R"(,"ModeByte":)" + modeByte + R"(})";
                auto response = PostAPIRequestAsync("CoinStirWithMode", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "CoinStir")
            {
                if (tokens.size() != 3)
                {
                    std::cout << "Usage: CoinStir <deviceID> <seconds>" << std::endl;
                    return;
                }
                std::string seconds = tokens[2];
                std::string requestData = seconds;
                auto response = PostAPIRequestAsync("CoinStir", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "GetCoinAcceptance")
            {
                if (tokens.size() != 3)
                {
                    std::cout << "Usage: GetCoinAcceptance <deviceID> <device>" << std::endl;
                    return;
                }
                std::string device = tokens[2];
                std::string requestData = device;
            }
            else if (commandType == "GetCounters")
            {
                auto response = GetAPIRequestAsync("GetCounters", deviceID);
                std::cout << response.get() << std::endl;
            }
            else if (commandType == "GetCoinsExit")
            {
                auto response = GetAPIRequestAsync("GetCoinsExit", deviceID);
                std::cout << response.get() << std::endl;
            }
            else if (commandType == "SetRealTimeClock")
            {
                if (tokens.size() != 8)
                {
                    std::cout << "Usage: SetRealTimeClock <deviceID> <byte1> <byte2> <byte3> <byte4> <byte5> <byte6>" << std::endl;
                    return;
                }
                std::string byte1 = tokens[2];
                std::string byte2 = tokens[3];
                std::string byte3 = tokens[4];
                std::string byte4 = tokens[5];
                std::string byte5 = tokens[6];
                std::string byte6 = tokens[7];
                std::string requestData = R"({"Byte1":)" + byte1 + R"(,"Byte2":)" + byte2 + R"(,"Byte3":)" + byte3 + R"(,"Byte4":)" + byte4 + R"(,"Byte5":)" + byte5 + R"(,"Byte6":)" + byte6 + R"(})";
                auto response = PostAPIRequestAsync("SetRealTimeClock", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "GetRealTimeClock")
            {
                auto response = GetAPIRequestAsync("GetRealTimeClock", deviceID);
                std::cout << response.get() << std::endl;
            }
            else if (commandType == "SetCashboxLevels")
            {
                if (tokens.size() != 5)
                {
                    std::cout << "Usage: SetCashboxLevels <deviceID> <numCoinsToAdd> <denomination> <countryCode>" << std::endl;
                    return;
                }
                std::string numCoinsToAdd = tokens[2];
                std::string denomination = tokens[3];
                std::string countryCode = tokens[4];
                std::string requestData = R"({"NumCoinsToAdd":)" + numCoinsToAdd + R"(,"Denomination":)" + denomination + R"(,"CountryCode":")" + countryCode + R"("})";
                auto response = PostAPIRequestAsync("SetCashboxLevels", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "SetDenominationRouteSingleCurrency")
            {
                if (tokens.size() != 4)
                {
                    std::cout << "Usage: SetDenominationRouteSingleCurrency <deviceID> <value> <route>" << std::endl;
                    return;
                }
                std::string value = tokens[2];
                std::string route = tokens[3];
                std::string requestData = R"({"Value":)" + value + R"(,"Route":)" + route + R"(})";
                auto response = PostAPIRequestAsync("SetDenominationRouteSingleCurrency", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "ClearCashboxLevels")
            {
                auto response = PostAPIRequestAsync("ClearCashboxLevels", deviceID);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "GetCashboxLevels")
            {
                auto response = GetAPIRequestAsync("GetCashboxLevels", deviceID);
                std::cout << response.get() << std::endl;
            }
            else if (commandType == "SetSorterRoute")
            {
                if (tokens.size() != 5)
                {
                    std::cout << "Usage: SetSorterRoute <deviceID> <value> <countryCode> <sorterRoute>" << std::endl;
                    return;
                }
                std::string value = tokens[2];
                std::string countryCode = tokens[3];
                std::string sorterRoute = tokens[4];
                std::string requestData = R"({"ValueCountryCode":{"Value":)" + value + R"(,"CountryCode":")" + countryCode + R"("},"SorterRoute":)" + sorterRoute + R"(})";
                auto response = PostAPIRequestAsync("SetSorterRoute", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "SetPayoutLimit")
            {
                if (tokens.size() != 3)
                {
                    std::cout << "Usage: SetPayoutLimit <deviceID> <value>" << std::endl;
                    return;
                }
                std::string value = tokens[2];
                std::string requestData = value;
                auto response = PostAPIRequestAsync("SetPayoutLimit", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "GetPayoutCount")
            {
                auto response = GetAPIRequestAsync("GetPayoutCount", deviceID);
                std::cout << response.get() << std::endl;
            }
            else if (commandType == "SetTwInMode")
            {
                if (tokens.size() != 3)
                {
                    std::cout << "Usage: SetTwInMode <deviceID> <twinMode>" << std::endl;
                    return;
                }
                std::string twinMode = tokens[2];
                std::string requestData = twinMode;
                auto response = PostAPIRequestAsync("SetTwInMode", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "ExtendedGetDatasetVersion")
            {
                auto response = GetAPIRequestAsync("ExtendedGetDatasetVersion", deviceID);
                std::cout << response.get() << std::endl;
            }
            else if (commandType == "ExtendeDGetFirmwareVersion")
            {
                auto response = GetAPIRequestAsync("ExtendeDGetFirmwareVersion", deviceID);
                std::cout << response.get() << std::endl;
            }
            else if (commandType == "comPortReadError")
            {
                auto response = GetAPIRequestAsync("comPortReadError", deviceID);
                std::cout << response.get() << std::endl;
            }
            else if (commandType == "DeviceState_StartupReady")
            {
                if (tokens.size() != 3)
                {
                    std::cout << "Usage: DeviceState_StartupReady <deviceID> <deviceState>" << std::endl;
                    return;
                }
                std::string deviceState = tokens[2];
                std::string requestData = deviceState;
                auto response = PostAPIRequestAsync("DeviceState_StartupReady", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "GetLifterStatus")
            {
                auto response = GetAPIRequestAsync("GetLifterStatus", deviceID);
                std::cout << response.get() << std::endl;
            }
            else if (commandType == "GetLastRejectCode")
            {
                auto response = GetAPIRequestAsync("GetLastRejectCode", deviceID);
                std::cout << response.get() << std::endl;
            }
            else if (commandType == "SetDenominationLevel")
            {
                if (tokens.size() != 5)
                {
                    std::cout << "Usage: SetDenominationLevel <deviceID> <value> <countryCode> <numCoinsToAdd>" << std::endl;
                    return;
                }
                std::string value = tokens[2];
                std::string countryCode = tokens[3];
                std::string numCoinsToAdd = tokens[4];
                std::string requestData = R"({"SetLevelsDenomination":{"Value":)" + value + R"(,"CountryCode":")" + countryCode + R"("},"NumCoinsToAdd":)" + numCoinsToAdd + R"(})";
                auto response = PostAPIRequestAsync("SetDenominationLevel", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "DeviceErrorLimpMode")
            {
                if (tokens.size() != 3)
                {
                    std::cout << "Usage: DeviceErrorLimpMode <deviceID> <deviceError>" << std::endl;
                    return;
                }
                std::string deviceError = tokens[2];
                std::string requestData = deviceError;
                auto response = PostAPIRequestAsync("DeviceErrorLimpMode", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "DeviceStateLimpMode")
            {
                if (tokens.size() != 3)
                {
                    std::cout << "Usage: DeviceStateLimpMode <deviceID> <deviceState>" << std::endl;
                    return;
                }
                std::string deviceState = tokens[2];
                std::string requestData = deviceState;
                auto response = PostAPIRequestAsync("DeviceStateLimpMode", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "CurrencyAssignment")
            {
                auto response = GetAPIRequestAsync("CurrencyAssignment", deviceID);
                std::cout << response.get() << std::endl;
            }
            else if (commandType == "SorterRouteAssignment")
            {
                auto response = GetAPIRequestAsync("SorterRouteAssignment", deviceID);
                std::cout << response.get() << std::endl;
            }
            else if (commandType == "SetDenominationInhibitsSingleCurrency")
            {
                if (tokens.size() != 3)
                {
                    std::cout << "Usage: SetDenominationInhibitsSingleCurrency <deviceID> <values> <inhibit>" << std::endl;
                    return;
                }
                std::string values = tokens[2];
                std::string inhibit = tokens[3];
                std::string requestData = R"({"Values":[)" + values + R"(],"Inhibit":)" + inhibit + R"(})";
                auto response = PostAPIRequestAsync("SetDenominationInhibitsSingleCurrency", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "SetDenominationInhibits")
            {
                if (tokens.size() != 3)
                {
                    std::cout << "Usage: SetDenominationInhibits <deviceID> <values> <inhibit>" << std::endl;
                    return;
                }
                std::string values = tokens[2];
                std::string inhibit = tokens[3];
                std::string requestData = R"({"Values":[)" + values + R"(],"Inhibit":)" + inhibit + R"(})";
                auto response = PostAPIRequestAsync("SetDenominationInhibits", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else if (commandType == "SetDenominationRoute")
            {
                if (tokens.size() != 5)
                {
                    std::cout << "Usage: SetDenominationRoute <deviceID> <value> <countryCode> <route>" << std::endl;
                    return;
                }
                std::string value = tokens[2];
                std::string countryCode = tokens[3];
                std::string route = tokens[4];
                std::string requestData = R"({"Value":)" + value + R"(,"CountryCode":")" + countryCode + R"(","Route":)" + route + R"(})";
                auto response = PostAPIRequestAsync("SetDenominationRoute", deviceID, requestData);
                std::cout << response.get().second << std::endl;
            }
            else
            {
                std::cout << "Unknown command: " << commandType << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error processing command: " << e.what() << std::endl;
        }
    }

    bool Validator::isConected()
    {

        // auto pollingThread1 = async(std::launch::async, pollDeviceStatus, BILL_VALIDATOR);
        // auto pollingThread2 = async(std::launch::async, pollDeviceStatus, COIN_VALIDATOR);
        // std::thread pollingThread1(pollDeviceStatus, BILL_VALIDATOR);
        // std::thread pollingThread2(pollDeviceStatus, COIN_VALIDATOR);

        // // Para asegurar que los hilos se junten antes de que el programa termine
        // pollingThread1.detach();
        // pollingThread2.detach();
        return true;
    }

    bool Validator::startPay()
    {
        this->pollInit.store(true);
        this->sumInput.store(0);
        // auto pollingThread1 = async(std::launch::async, &Validator::pollDeviceStatus, this, BILL_VALIDATOR);
        // auto pollingThread2 = async(std::launch::async, &Validator::pollDeviceStatus, this, COIN_VALIDATOR);
        std::thread pollingThread1(&Validator::pollDeviceStatus, this, BILL_VALIDATOR);
        std::thread pollingThread2(&Validator::pollDeviceStatus, this, COIN_VALIDATOR);

        // Para asegurar que los hilos se junten antes de que el programa termine
        pollingThread1.detach();
        pollingThread2.detach();

        auto connectResponse1 = PostAPIRequestAsync("ConnectDevice", BILL_VALIDATOR);
        auto connectResponse2 = PostAPIRequestAsync("ConnectDevice", COIN_VALIDATOR);

        auto r1 = connectResponse1.get();
        auto r2 = connectResponse2.get();

        if (r1.first != 200 || r2.first != 200)
        {
            std::cerr << r1.second << std::endl;
            std::cerr << r2.second << std::endl;
            this->pollInit.store(false);
            return false;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        connectResponse1 = PostAPIRequestAsync("StartDevice", BILL_VALIDATOR);
        connectResponse2 = PostAPIRequestAsync("StartDevice", COIN_VALIDATOR);

        r1 = connectResponse1.get();
        r2 = connectResponse2.get();

        if (r1.first != 200 || r2.first != 200)
        {
            std::cerr << r1.second << std::endl;
            std::cerr << r2.second << std::endl;
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        connectResponse1 = PostAPIRequestAsync("EnablePayout", BILL_VALIDATOR);
        connectResponse2 = PostAPIRequestAsync("EnablePayout", COIN_VALIDATOR);

        r1 = connectResponse1.get();
        r2 = connectResponse2.get();

        if (r1.first != 200 || r2.first != 200)
        {
            std::cerr << r1.second << std::endl;
            std::cerr << r2.second << std::endl;
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        connectResponse1 = PostAPIRequestAsync("EnableAcceptor", BILL_VALIDATOR);
        connectResponse2 = PostAPIRequestAsync("EnableAcceptor", COIN_VALIDATOR);

        r1 = connectResponse1.get();
        r2 = connectResponse2.get();

        if (r1.first != 200 || r2.first != 200)
        {
            std::cerr << r1.second << std::endl;
            std::cerr << r2.second << std::endl;
            return false;
        }

        return true;
    }

    bool Validator::stopPay()
    {
        this->pollInit.store(false);
        auto connectResponse1 = PostAPIRequestAsync("DisableAcceptor", BILL_VALIDATOR);
        auto connectResponse2 = PostAPIRequestAsync("DisableAcceptor", COIN_VALIDATOR);

        connectResponse1 = PostAPIRequestAsync("DisablePayout", BILL_VALIDATOR);
        connectResponse2 = PostAPIRequestAsync("DisablePayout", COIN_VALIDATOR);

        connectResponse1 = PostAPIRequestAsync("StopDevice", BILL_VALIDATOR);
        connectResponse2 = PostAPIRequestAsync("StopDevice", COIN_VALIDATOR);

        auto r1 = connectResponse1.get();
        auto r2 = connectResponse2.get();

        if (r1.first != 200 || r2.first != 200)
        {
            std::cerr << r1.second << std::endl;
            std::cerr << r2.second << std::endl;
            return false;
        }
        return true;
    }

    Validator::Validator(/* args */) : pollInit(false), BILL_VALIDATOR("NV200"), COIN_VALIDATOR("NV4000")
    {
        std::string device = "0";
        std::string ssp = "0";

        // Change "ComPort" to be the port name on your machine and change "LogFilePath"
        std::string openDeviceRequestData = "{\"ComPort\":\"/dev/ttyUSB" + device + "\",\"SspAddress\":" + ssp + ",\"LogFilePath\":\"./log\"}";

        auto initResponse1 = PostAPIRequestAsync("InitialiseDevice", BILL_VALIDATOR);
        auto initResponse2 = PostAPIRequestAsync("InitialiseDevice", COIN_VALIDATOR);

        auto re1 = initResponse1.get();
        auto re2 = initResponse2.get();

        std::cout << re1.second << std::endl;
        std::cout << re2.second << std::endl;

        auto openResponse1 = PostAPIRequestAsync("OpenDevice", BILL_VALIDATOR, openDeviceRequestData);
        auto r1 = openResponse1.get();
        std::cout << r1.second << std::endl;

        if (r1.first == 400)
        {
            device = "1";
            openDeviceRequestData = "{\"ComPort\":\"/dev/ttyUSB" + device + "\",\"SspAddress\":" + ssp + ",\"LogFilePath\":\"./log\"}";
            auto openResponse = PostAPIRequestAsync("OpenDevice", BILL_VALIDATOR, openDeviceRequestData);
            std::cout << openResponse.get().second << std::endl;

            device = "0";
            ssp = "16";
            openDeviceRequestData = "{\"ComPort\":\"/dev/ttyUSB" + device + "\",\"SspAddress\":" + ssp + ",\"LogFilePath\":\"./log\"}";
            auto openResponse2 = PostAPIRequestAsync("OpenDevice", COIN_VALIDATOR, openDeviceRequestData);
            std::cout << openResponse2.get().second << std::endl;
        }
        else
        {
            device = "1";
            ssp = "16";
            openDeviceRequestData = "{\"ComPort\":\"/dev/ttyUSB" + device + "\",\"SspAddress\":" + ssp + ",\"LogFilePath\":\"./log\"}";
            auto openResponse2 = PostAPIRequestAsync("OpenDevice", COIN_VALIDATOR, openDeviceRequestData);
            std::cout << openResponse2.get().second << std::endl;
        }
    }

    Validator::~Validator()
    {
        stopPay();
    }
} // namespace Helper
