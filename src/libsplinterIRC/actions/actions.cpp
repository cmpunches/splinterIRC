#include "../client/client.h"
//actions

void splinterClient::join_channel( std::string channel )
{
    std::string command = "JOIN " + channel;
    IRCActionEnvelope action = IRCActionEnvelope( get_id() , command );
    enqueue_action(action);
}

void splinterClient::send_private_message( std::string nick, std::string message )
{
    std::string command = "PRIVMSG " + nick + " :" + message;
    IRCActionEnvelope action = IRCActionEnvelope( get_id() , command );
    enqueue_action(action);
}

void splinterClient::set_nick( std::string nick )
{
    std::string command = "NICK " + nick;
    IRCActionEnvelope action = IRCActionEnvelope( get_id() , command );
    enqueue_action(action);

    // TODO: this is a hack.  move this to the handler for a successful nick change.
    nick_ = nick;
}

void splinterClient::register_user( std::string user )
{
    std::string command = "USER " + user + " 0 * :" + user;
    IRCActionEnvelope action = IRCActionEnvelope( get_id() , command );
    enqueue_action(action);
}

void splinterClient::quit( std::string message )
{
    std::string command = "QUIT :" + message;
    IRCActionEnvelope action = IRCActionEnvelope( get_id() , command );
    enqueue_action(action);
}

void splinterClient::list_server_capabilities()
{
    std::string command = "CAP LS";
    IRCActionEnvelope action = IRCActionEnvelope( get_id() , command );
    enqueue_action(action);
}

void splinterClient::send_ping_reply( std::string target )
{
    std::string command = "PONG " + target;
    IRCActionEnvelope action = IRCActionEnvelope( get_id() , command );
    enqueue_action(action);
}

void splinterClient::request_sasl_capability()
{
    std::string command = "CAP REQ :sasl";
    IRCActionEnvelope action = IRCActionEnvelope( get_id() , command );
    enqueue_action(action);
}

void splinterClient::end_capabilites_negotiation()
{
    std::string command = "CAP END";
    IRCActionEnvelope action = IRCActionEnvelope( get_id() , command );
    enqueue_action(action);
}

void splinterClient::whois_user( std::string nick )
{
    std::string command = "WHOIS " + nick;
    IRCActionEnvelope action = IRCActionEnvelope( get_id() , command );
    enqueue_action(action);
}

// internal action to tell all threads to start to die
void splinterClient::die()
{
    std::string command = "DIE";
    IRCActionEnvelope action = IRCActionEnvelope( get_id() , command );
    enqueue_action(action);
}