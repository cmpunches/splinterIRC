#include "../client/client.h"
//actions

void splinterClient::join_channel( std::string channel )
{
    send( "JOIN " + channel );
}

void splinterClient::send_private_message( std::string nick, std::string message )
{
    send("PRIVMSG " + nick + " :" + message );
}

void splinterClient::set_nick( std::string nick )
{
    send("NICK " + nick );
    nick_ = nick;
}

void splinterClient::register_user(std::string user )
{
    send("USER " + user + " 0 * :" + user );
}

void splinterClient::quit( std::string message )
{
    send("QUIT :" + message );
}

void splinterClient::list_server_capabilities()
{
    send("CAP LS");
}

void splinterClient::send_ping_reply( std::string target )
{
    send("PONG " + target );
}

void splinterClient::request_sasl_capability()
{
    send("CAP REQ :sasl");
}

void splinterClient::end_capabilites_negotiation()
{
    send("CAP END");
}
