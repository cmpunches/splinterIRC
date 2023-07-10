#define VERSION "0.1a"

#include <iostream>
#include <string>
#include <getopt.h>
#include <fstream>
#include "src/libsplinterIRC/events/IRCEventEnvelope.h"
#include <cctype>

int main() {
    std::ifstream file("parser_tests.txt");
    if (!file) {
        std::cerr << "Error: test data file not found\n";
        return 1;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::string client_id = "0";
        IRCEventEnvelope event(line, "testserver", client_id );
        std::cout << event.to_json(1, 4, 1) << std::endl;
    }
    return 0;
}