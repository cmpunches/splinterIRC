#include "IRCEventv3.h"

// split a string into a vector of strings based on a delimiter
std::vector<std::string> IRCEventEnvelope::split(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, delimiter)) {
        result.push_back(item);
    }
    return result;
}
