#include "../client/client.h"
//actions

void splinterClient::join_channel( std::string channel )
{
    send( "JOIN " + channel + "\r\n" );
}

void splinterClient::send_private_message( std::string nick, std::string message )
{
    send("PRIVMSG " + nick + " :" + message + "\r\n");
}

void splinterClient::set_nick( std::string nick )
{
    send("NICK " + nick + "\r\n");
    nick_ = nick;
}

void splinterClient::set_user( std::string user )
{
    send("USER " + user + " 0 * :" + user + "\r\n");
}