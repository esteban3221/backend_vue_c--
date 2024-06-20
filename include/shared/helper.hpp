#pragma once
#include <algorithm>
#include <array>
#include <cstdio>
#include <memory>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

namespace Helper
{
    namespace System
    {
        std::string exec(const char *cmd);
        std::string formatTime(int seconds);
    } // namespace System
} // namespace Helper
