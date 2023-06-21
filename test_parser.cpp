#define VERSION "0.1a"

#include <iostream>
#include <string>
#include <getopt.h>
#include <fstream>
#include "src/libsplinterIRC/events/IRCEventv3.h"

int main() {
    std::ifstream file("parser_tests.txt");
    if (!file) {
        std::cerr << "Error: test data file not found\n";
        return 1;
    }
    std::string line;
    while (std::getline(file, line)) {
        IRCEventEnvelope event(line, "testserver" );
        std::cout << event.to_json(1, 4) << std::endl;
    }
    return 0;
}