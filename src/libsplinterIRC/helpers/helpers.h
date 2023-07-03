#ifndef SPLINTERIRC_HELPERS_H
#define SPLINTERIRC_HELPERS_H


#include <string>
#include <sstream>
#include <vector>

std::string get_word(int index, const std::string& str);
std::string get_after_word(int index, const std::string& str);
std::vector<std::string> split(const std::string& str, char delimiter);

#endif //SPLINTERIRC_HELPERS_H
