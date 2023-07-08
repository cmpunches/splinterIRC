#include "helpers.h"

// gets the Nth word of a string
std::string get_word(int index, const std::string& str)
{
    std::istringstream iss(str);
    std::string word;
    int count = 1;
    while (iss >> word)
    {
        if (count == index)
        {
            return word;
        }
        count++;
    }
    return "";
}

std::string get_after_word(int index, const std::string& str)
{
    std::istringstream iss(str);
    std::string word;
    std::string result;
    int count = 1;
    while (iss >> word) {
        if (count >= index) {
            result = str.substr(iss.tellg());
            break;
        }
        count++;
    }
    // trim leading whitespace
    size_t start = result.find_first_not_of(" \t\n\r\f\v");
    if (start != std::string::npos) {
        result = result.substr(start);
    }
    return result;
}

// split a string into a vector of strings based on a delimiter
std::vector<std::string> split(const std::string& str, char delimiter)
{
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, delimiter)) {
        result.push_back(item);
    }
    return result;
}

bool is_num_or_any(std::string str) {
    if (str == "*") {
        return true;
    }
    for (char c : str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}