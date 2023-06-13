#include "../client/client.h"

//handlers
// TODO: this should be reworked as not every CAP message is going to be SASL related
//      and not every CAP ACK message is going to be SASL related
void splinterClient::handle_S_RPL_CAP( const IRCEvent& event )
{
    handle_UNKNOWN(event);
}

// TODO: this should be reworked as not every CAP LS message is going to be SASL related
//      and not every CAP ACK message is going to be SASL related
void splinterClient::handle_S_RPL_CAP_LS( const IRCEvent& event )
{
    report_event(event);

    if ( use_sasl_ )
    {
        // Check if the server supports SASL
        if (event.message().find("sasl") != std::string::npos)
        {
            // Send CAP REQ command to request SASL capability
            send("CAP REQ :sasl\r\n");
        } else {
            std::cerr << "Received an unprocessed CAP LS message.  Report this message as a parsing bug." << std::endl;
            critical_thread_failed = true;
        }
    }
}
// TODO: this should be reworked as not every CAP ACK message is going to be SASL related
void splinterClient::handle_S_RPL_CAP_ACK( const IRCEvent& event )
{
    report_event(event);

    // is the capability response signifying SASL?
    if ( ( event.message().find("ACK sasl") != std::string::npos) and ( event.target() == "*" ) )
    {
        // Send AUTHENTICATE command to initiate SASL authentication
        send("AUTHENTICATE PLAIN\r\n");
    } else {
        std::cerr << "Received an unprocessed CAP ACK message.  Report this message as a parsing bug." << std::endl;
        critical_thread_failed = true;
    }
}

static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

std::string base64_encode(const std::string& s)
{
    std::string ret;
    int val = 0;
    int bits = -6;
    const unsigned int b63 = 0x3F;

    for (const auto& c : s)
    {
        val = (val << 8) + c;
        bits += 8;
        while (bits >= 0)
        {
            ret.push_back(base64_chars[(val >> bits) & b63]);
            bits -= 6;
        }
    }

    if (bits > -6)
    {
        ret.push_back(base64_chars[((val << 8) >> (bits + 8)) & b63]);
    }

    while (ret.size() % 4)
    {
        ret.push_back('=');
    }

    return ret;
}

void splinterClient::handle_AUTHENTICATE( const IRCEvent& event )
{
    report_event(event);

    // Send AUTHENTICATE command with base64-encoded credentials
    std::string auth = sasl_username_ + '\0' + sasl_username_ + '\0' + sasl_password_;
    send("AUTHENTICATE " + base64_encode(auth) + "\r\n");
}

void splinterClient::handle_RPL_SASLSUCCESS( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_LOGGEDIN( const IRCEvent& event )
{
    report_event(event);

    // SASL authentication successful

    // Send CAP END command to end capability negotiation
    send("CAP END\r\n");

    set_nick( nick_ );
    set_user( nick_ );
}

void splinterClient::handle_ERR_SASLFAIL( const IRCEvent& event )
{
    report_event(event);
    // SASL authentication failed
    std::cerr << "SASL AUTH FAILED." << std::endl;
    critical_thread_failed = true;
}

void splinterClient::handle_ERR_SASLTOOLONG( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_SASLABORTED( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_SASLALREADY( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_SASLMECHS( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_unassociated_event( const IRCEvent& event )
{
    std::cerr << "Received a type without a handler associated.  Report this message as a bug." << std::endl;
    std::cerr << event.to_json() << std::endl;
}

void splinterClient::handle_UNKNOWN( const IRCEvent& event )
{
    std::cerr << "Unknown message type: ";
    std::cerr << event.to_json() << std::endl;
}

void splinterClient::report_event( const IRCEvent& event )
{
    std::cout << event.to_json();
}

void splinterClient::handle_S_CHANNEL_MESSAGE(const IRCEvent& event)
{
    report_event(event);
    handle_ctcp( event );
    handle_command( event );
}

void splinterClient::handle_S_PRIVATE_MESSAGE(const IRCEvent &event)
{
    report_event(event);
    handle_ctcp( event );
    handle_command( event );
}

void splinterClient::handle_ctcp_version(const IRCEvent& event)
{
    std::string target = event.nick();
    std::string message = "NOTICE " + target + " :\x01VERSION splinterIRC\x01\r\n";
    send(message);
}

void splinterClient::handle_ctcp_time( const IRCEvent& event )
{
    std::string target = event.nick();
    std::string message = "NOTICE " + target + " :\x01TIME " + std::to_string(std::time(nullptr)) + "\x01\r\n";
    send(message);
}

void splinterClient::handle_ctcp_ping(const IRCEvent& event)
{
    std::string target = event.nick();
    std::string ctcp_args = event.message().substr(6, event.message().size() - 7);
    std::string message = "NOTICE " + target + " :\x01PING " + ctcp_args + "\x01\r\n";
    send(message);
}

void splinterClient::handle_ctcp( const IRCEvent& event )
{
    std::string message_ = event.message();

    // Check if the message is a CTCP message
    if (message_.front() == '\x01' && message_.back() == '\x01')
    {
        // Extract the CTCP command and its arguments
        std::string ctcp_message = message_.substr(1, message_.size() - 2);
        std::string ctcp_command;
        std::string ctcp_args;
        std::istringstream ctcp_ss(ctcp_message);
        if (std::getline(ctcp_ss, ctcp_command, ' '))
        {
            std::getline(ctcp_ss, ctcp_args);
            // Handle the CTCP command
            if (ctcp_command == "VERSION") {
                std::cerr << "VERSION CTCP REQUESTED" << std::endl;
                handle_ctcp_version( event );
            } else if (ctcp_command == "TIME") {
                handle_ctcp_time( event );
            } else if (ctcp_command == "PING") {
                handle_ctcp_ping( event );
            }
        }
    }
}

void splinterClient::handle_JOIN( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_PART( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_KICK( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_NOTICE( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_PING( const IRCEvent& event )
{
    report_event(event);
    std::string target = event.target();
    send("PONG " + target + "\r\n");
}

void splinterClient::handle_QUIT( const IRCEvent& event )
{
    report_event(event);
    critical_thread_failed = true;
}

void splinterClient::handle_NICK( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_TOPIC( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_INVITE( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERROR( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WELCOME( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_YOURHOST( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_CREATED( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_MYINFO( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ISUPPORT( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_BOUNCE( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_STATSCOMMANDS( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ENDOFSTATS( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_STATSUPTIME( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_UMODEIS( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_LUSERCLIENT( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_LUSEROP( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_LUSERUNKNOWN( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_LUSERCHANNELS( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_LUSERME( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ADMINME( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ADMINLOC1( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ADMINLOC2( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ADMINEMAIL( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_TRYAGAIN( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_LOCALUSERS( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_GLOBALUSERS( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOISCERTFP( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_NONE( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_AWAY( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_USERHOST( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_UNAWAY( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_NOWAWAY( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOREPLY( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ENDOFWHO( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOISREGNICK( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOISUSER( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOISSERVER( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOISOPERATOR( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOWASUSER( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOISIDLE( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ENDOFWHOIS( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOISCHANNELS( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOISSPECIAL( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_LISTSTART( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_LIST( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_LISTEND( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_CHANNELMODEIS( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_CREATIONTIME( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOISACCOUNT( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_NOTOPIC( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_TOPIC( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_TOPICWHOTIME( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_INVITELIST( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ENDOFINVITELIST( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOISACTUALLY( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_INVITING( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_INVEXLIST( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ENDOFINVEXLIST( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_EXCEPTLIST( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ENDOFEXCEPTLIST( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_VERSION( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_NAMREPLY( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ENDOFNAMES( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_LINKS( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ENDOFLINKS( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_BANLIST( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ENDOFBANLIST( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ENDOFWHOWAS( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_INFO( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ENDOFINFO( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_MOTDSTART( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_MOTD( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ENDOFMOTD( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOISHOST( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOISMODES( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_YOUREOPER( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_REHASHING( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_TIME( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_UNKNOWNERROR( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NOSUCHNICK( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NOSUCHSERVER( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NOSUCHCHANNEL( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_CANNOTSENDTOCHAN( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_TOOMANYCHANNELS( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_WASNOSUCHNICK( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NOORIGIN( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NORECIPIENT( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NOTEXTTOSEND( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_INPUTTOOLONG( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_UNKNOWNCOMMAND( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NOMOTD( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NONICKNAMEGIVEN( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_ERRONEUSNICKNAME( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NICKNAMEINUSE( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NICKCOLLISION( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_USERNOTINCHANNEL( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NOTONCHANNEL( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_USERONCHANNEL( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NOTREGISTERED( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NEEDMOREPARAMS( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_ALREADYREGISTERED( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_PASSWDMISMATCH( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_YOUREBANNEDCREEP( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_CHANNELISFULL( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_UNKNOWNMODE( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_INVITEONLYCHAN( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_BANNEDFROMCHAN( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_BADCHANNELKEY( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_BADCHANMASK( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NOPRIVILEGES( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_CHANOPRIVSNEEDED( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_CANTKILLSERVER( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NOOPERHOST( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_UMODEUNKNOWNFLAG( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_USERSDONTMATCH( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_HELPNOTFOUND( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_INVALIDKEY( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_STARTTLS( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOISSECURE( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_STARTTLS( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_INVALIDMODEPARAM( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_HELPSTART( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_HELPTXT( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ENDOFHELP( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NOPRIVS( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_LOGGEDOUT( const IRCEvent& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NICKLOCKED( const IRCEvent& event )
{
    report_event(event);
}
