#pragma once

#include <string>
#include <unordered_map>

class ProtocolHandler {

public:
    std::string CreateProtocolString(const std::unordered_map<std::string, std::string>& map) const;
    std::unordered_map<std::string, std::string> ParseProtocolString(const std::string& message) const;
};