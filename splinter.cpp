#include "src/libsplinterIRC/client/client.h"
#define VERSION "0.1a"

#include <iostream>
#include <string>
#include <getopt.h>

void show_usage(const std::string& program_name) {
    std::cout << "Usage: " << program_name << " -s|--server SERVER -p|--port PORT -n|--nick NICK -w||--password PASSWORD [ -S|--use-sasl -u|--sasl-username SASL_USERNAME -k|--sasl-password SASL_PASSWORD ]" << std::endl
              << std::endl
              << "Required options:" << std::endl
              << "  -s, --server=ADDR        Server address" << std::endl
              << "  -p, --port=PORT          Server port" << std::endl
              << "  -n, --nick=NICK          Nickname" << std::endl
              << "  -w, --password=PASS      Password" << std::endl
              << std::endl
              << "Optional options:" << std::endl
              << "  -u, --sasl-username=USER SASL username" << std::endl
              << "  -k, --sasl-password=PASS SASL password" << std::endl
              << "  -S, --use-sasl           Use SASL authentication" << std::endl
              << "  -v, --verbose            Verbose output" << std::endl
              << "  -V, --version            Show version information" << std::endl
              << "  -h, --help               Show this help screen" << std::endl << std::endl;
}

int main(int argc, char *argv[])
{
    std::string master_server;
    std::string master_port;
    std::string master_nick;
    std::string master_password;

    std::string sasl_username;
    std::string sasl_password;
    bool use_sasl = false;

    bool verbose = false;

    const char* const short_opts = "s:p:n:w:u:k:SvVh";
    const option long_opts[] = {
            {"server", required_argument, nullptr, 's'},
            {"port", required_argument, nullptr, 'p'},
            {"nick", required_argument, nullptr, 'n'},
            {"password", required_argument, nullptr, 'w'},
            {"sasl-username", required_argument, nullptr, 'u'},
            {"sasl-password", required_argument, nullptr, 'k'},
            {"use-sasl", no_argument, nullptr, 'S'},
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
            case 'u':
                sasl_username = optarg;
                break;
            case 'k':
                sasl_password = optarg;
                break;
            case 'S':
                use_sasl = true;
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
                show_usage(argv[0]);
                return 0;
        }
    }

    if (master_server.empty() || master_port.empty() || master_nick.empty() || master_password.empty()) {
        std::cerr << "Error: missing required arguments" << std::endl;
        show_usage(argv[0]);
        return 1;
    }

    if (use_sasl && (sasl_username.empty() || sasl_password.empty())) {
        std::cerr << "Error: missing required SASL arguments" << std::endl;
        show_usage(argv[0]);
        return 1;
    }

    // validate server address
    if (master_server.empty()) {
        std::cerr << "Error: server address cannot be empty" << std::endl;
        return 1;
    }

    if (verbose) {
        std::cout << "Server:\t\t" << master_server << std::endl;
        std::cout << "Port:\t\t" << master_port << std::endl;
        std::cout << "Nick:\t\t" << master_nick << std::endl;
        std::cout << "Password:\t" << master_password << std::endl;
        std::cout << "Verbose:\t" << (verbose ? "YES" : "NO") << std::endl;
        std::cout << std::endl;
        std::cout << "Use SASL:\t" << (use_sasl ? "YES" : "NO") << std::endl;
        std::cout << "SASL Username:\t" << sasl_username << std::endl;
        std::cout << "SASL Password:\t" << sasl_password << std::endl << std::endl;
    }


    // validate port number
    try {
        int port = std::stoi(master_port);
        if (port < 1 || port > 65535) {
            throw std::out_of_range("Invalid port number");
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: invalid port number (" << e.what() << ")" << std::endl;
        return 1;
    }

    // validate nickname
    if (master_nick.empty()) {
        std::cerr << "Error: nickname cannot be empty" << std::endl;
        return 1;
    }

    // connect master splinter to the control server
    try {
        auto master = std::make_shared<splinterClient>(master_server,
                                                       master_port,
                                                       master_nick,
                                                       master_password,
                                                       use_sasl,
                                                       sasl_username,
                                                       sasl_password,
                                                       verbose);
        master->connect();
        master->add_to_clients();
        master->run_event_loop();
    } catch (const std::exception& e) {
        std::cerr << "Error: failed to connect to server (" << e.what() << ")\n";
        return 1;
    }

    return 0;
}