#include "fty_common_quote_codec.h"

std::string quoteEncode(const std::string& param)
{
    std::string ret;

    for (const auto& c : param) {
        ret += (c == '"') ? "\\u0022" : std::string{c};
    }

    return ret;
}

std::string quoteDecode(const std::string& param)
{
    std::string ret{param};

    while(1) {
        size_t pos = ret.find("\\u0022");
        if (pos == std::string::npos)
            break;
        ret.replace(pos, 6, "\""); // 6 = len "\\u0022"
    }

    return ret;
}