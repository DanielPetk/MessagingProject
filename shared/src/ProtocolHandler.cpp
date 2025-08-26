#include <shared/protocol/ProtocolHandler.h>
#include <shared/shared.h>
#include <iostream>
#include <sstream>

std::string ProtocolHandler::CreateProtocolString(const std::unordered_map<std::string, std::string>& map) {
    std::ostringstream oss;
    for (const auto& [key, value] : map) {
        oss << key << VALUE_SEPARATOR << value << DELIM;
    }
    oss << ENDM;

    return oss.str();
}

std::unordered_map<std::string, std::string> ProtocolHandler::ParseProtocolString(const std::string& message) {
    size_t pos = message.find(ENDM);
    if (pos == std::string::npos) {
        return {};
    }

    std::istringstream iss{message.substr(0, pos)};
    std::string pair;
    std::unordered_map<std::string, std::string> map;
    while (std::getline(iss, pair, DELIM.at(0))) {
        pos = pair.find(VALUE_SEPARATOR);
        if (pos == std::string::npos || static_cast<int>(pos) == pair.size() - 1) {
            continue;
        }

        map.insert_or_assign(pair.substr(0,pos), pair.substr(pos+1));
    }

    return map;
}

