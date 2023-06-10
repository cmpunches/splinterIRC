#include "src/libsplinterIRC/client/client.h"

int main()
{
    std::string master_server   = "irc.techrights.org";
    std::string master_port     = "6667";
    std::string master_nick     = "splinter";
    std::string master_password = "shredder";

    // connect master splinter to the control server
    splinterClient master( master_server, master_port, master_nick, master_password );

    return 0;
}
