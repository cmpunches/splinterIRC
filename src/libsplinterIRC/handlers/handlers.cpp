#include "../client/client.h"

// decision/action

void splinterClient::handle_ping(const IRCEvent &event)
{
    std::string target = event.target();
    send("PONG " + target + "\r\n");
}

void splinterClient::handle_channel_message(const IRCEvent& event)
{
    handle_command( event );
}

void splinterClient::handle_private_message(const IRCEvent& event) {
    //std::cout << "PRIVATE_MSG: <" << event.sender() << "> " << message << std::endl;
    handle_command( event );
}

void splinterClient::handle_join( const IRCEvent& event )
{
    //std::cout << "JOIN: " << event.channel() << " <" << event.sender() << "> " << event.message() << std::endl;
}

void splinterClient::handle_part( const IRCEvent& event )
{
    //std::cout << "PART: " << event.channel() << " <" << event.sender() << "> " << event.message() << std::endl;
}

void splinterClient::handle_quit( const IRCEvent& event )
{
    critical_thread_failed = true;
    //std::cout << "QUIT: " << event.channel() << " <" << event.sender() << "> " << event.message() << std::endl;
}

void splinterClient::handle_nick( const IRCEvent& event )
{
    //std::cout << "NICK: " << event.nick() << " -> " << event.target() << std::endl;
}

void splinterClient::handle_mode( const IRCEvent& event )
{
    //std::cout << "MODE: " << event.channel() << " <" << event.sender() << "> " << event.message() << std::endl;
}

void splinterClient::handle_topic( const IRCEvent& event )
{
    std::string message = event.message();
    std::string sender = event.sender();
    std::string channel = event.channel();
    //std::cout << "TOPIC: " << channel << " <" << sender << "> " << message << std::endl;
}

void splinterClient::handle_kick( const IRCEvent& event )
{
    std::string message = event.message();
    std::string sender = event.sender();
    std::string channel = event.channel();
    //std::cout << "KICK: " << channel << " <" << sender << "> " << message << std::endl;
}

void splinterClient::handle_invite( const IRCEvent& event )
{
    std::string message = event.message();
    std::string sender = event.sender();
    std::string channel = event.channel();
    //std::cout << "INVITE: " << channel << " <" << sender << "> " << message << std::endl;
}

void splinterClient::handle_notice( const IRCEvent& event )
{
    std::string message = event.message();
    std::string sender = event.sender();
    std::string target = event.target();
    //std::cout << "NOTICE: " << target << " <" << sender << "> " << message << std::endl;
}

void splinterClient::handle_privmsg( const IRCEvent& event )
{
    std::string message = event.message();
    std::string sender = event.sender();
    std::string target = event.target();
    //std::cout << "PRIVMSG: " << target << " <" << sender << "> " << message << std::endl;
}

void splinterClient::handle_welcome( const IRCEvent& event )
{
    std::string message = event.message();
    std::string sender = event.sender();
    //std::cout << "WELCOME: " << sender << " " << message << std::endl;
}

void splinterClient::handle_error( const IRCEvent& event )
{
    std::string message = event.message();
    std::string sender = event.sender();
    //std::cout << "ERROR: " << sender << " " << message << std::endl;
    //exit(1);
}

void splinterClient::handle_motd( const IRCEvent& event )
{
    std::string message = event.message();
    std::string sender = event.sender();
    //std::cout << "MOTD: " << sender << " " << message << std::endl;
}
// -EVENT_HANDLERS

void splinterClient::handle_rpl_welcome( const IRCEvent& event )
{
    handle_notice(event);
}

void splinterClient::handle_rpl_yourhost( const IRCEvent& event )
{
    handle_notice(event);
}

void splinterClient::handle_rpl_created( const IRCEvent& event )
{
    handle_notice(event);
}

void splinterClient::handle_rpl_myinfo( const IRCEvent& event )
{
    handle_notice(event);
}

void splinterClient::handle_rpl_isupport( const IRCEvent& event )
{
    handle_notice(event);
}
