#include "src/libsplinterIRC/client/client.h"

#define VERSION "0.1a"

#include <iostream>
#include <string>
#include <getopt.h>

void show_usage() {
    std::cout << "Usage: [options]\n"
              << "Options:\n"
              << "  -s, --server    Server address\n"
              << "  -p, --port      Server port\n"
              << "  -n, --nick      Nickname\n"
              << "  -w, --password  Password\n"
              << "  -v, --verbose   Verbose output\n"
              << "  -V, --version   Show version information\n"
              << "  -h, --help      Show this help screen\n";
}

int main(int argc, char *argv[])
{
    std::string master_server;
    std::string master_port;
    std::string master_nick;
    std::string master_password;
    bool verbose = false;

    const char* const short_opts = "s:p:n:w:vVh";
    const option long_opts[] = {
            {"server", required_argument, nullptr, 's'},
            {"port", required_argument, nullptr, 'p'},
            {"nick", required_argument, nullptr, 'n'},
            {"password", required_argument, nullptr, 'w'},
            {"verbose", no_argument, nullptr, 'v'},
            {"version", no_argument, nullptr, 'V'},
            {"help", no_argument, nullptr, 'h'},
            {nullptr, no_argument, nullptr, 0}
    };

    while (true)
    {
        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

        if (-1 == opt)
            break;

        switch (opt)
        {
            case 's':
                master_server = optarg;
                break;
            case 'p':
                master_port = optarg;
                break;
            case 'n':
                master_nick = optarg;
                break;
            case 'w':
                master_password = optarg;
                break;
            case 'v':
                verbose = true;
                break;
            case 'V':
                std::cout << argv[0] << ": v" << VERSION << std::endl;
                return 0;
            case 'h': // -h or --help
            case '?': // Unrecognized option
            default:
                show_usage();
                return 0;
        }
    }

    if (master_server.empty() || master_port.empty() || master_nick.empty() || master_password.empty()) {
        std::cerr << "Error: missing required arguments\n";
        show_usage();
        return 1;
    }

    if (verbose) {
        std::cout << "Server: " << master_server << '\n';
        std::cout << "Port: " << master_port << '\n';
        std::cout << "Nick: " << master_nick << '\n';
        std::cout << "Password: " << master_password << '\n';
    }

    // validate server address
    if (master_server.empty()) {
        std::cerr << "Error: server address cannot be empty\n";
        return 1;
    }

    // validate port number
    try {
        int port = std::stoi(master_port);
        if (port < 1 || port > 65535) {
            throw std::out_of_range("Invalid port number");
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: invalid port number (" << e.what() << ")\n";
        return 1;
    }

    // validate nickname
    if (master_nick.empty()) {
        std::cerr << "Error: nickname cannot be empty\n";
        return 1;
    }

    // connect master splinter to the control server
    try {
        splinterClient master( master_server, master_port, master_nick, master_password );
        master.connect();
        master.run_event_loop();
    } catch (const std::exception& e) {
        std::cerr << "Error: failed to connect to server (" << e.what() << ")\n";
        return 1;
    }

    return 0;
}