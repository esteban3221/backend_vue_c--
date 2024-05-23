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
    } // namespace System
} // namespace Helper
