#include "../client/client.h"

void splinterClient::report_event( IRCEventEnvelope& event )
{
    bool pretty_print = true;
    int indent = 4;

    std::cout << event.to_json( pretty_print, indent, verbose_ ) << std::endl;
}

//handlers

// TODO: this should be reworked as not every CAP message is going to be SASL related
//      and not every CAP ACK message is going to be SASL related
void splinterClient::handle_S_RPL_CAP( IRCEventEnvelope& event )
{
    handle_UNKNOWN(event);
}

// TODO: this should be reworked as not every CAP LS message is going to be SASL related
//      and not every CAP ACK message is going to be SASL related
void splinterClient::handle_S_RPL_CAP_LS( IRCEventEnvelope& event )
{
    // TODO: add client state tracking for SASL authentication instead of just mindlessly replying to CAP LS

    report_event(event);

    if ( use_sasl_ )
    {
        // Check if the server supports SASL
        std::vector<std::string> capabilities = event.get_array_attribute("server_capabilities");
        for (const std::string& capability: capabilities)
        {
            if (capability  == "sasl" ) {
                request_sasl_capability();
            }
        }
    }
}
// TODO: this should be reworked as not every CAP ACK message is going to be SASL related -- related to state tracking
void splinterClient::handle_S_RPL_CAP_ACK( IRCEventEnvelope& event ) {
    report_event(event);

    std::string capability = event.get_scalar_attribute("ack_capability");

    if ( capability == "sasl" )
    {
        // Send AUTHENTICATE command to initiate SASL authentication
        std::string command = "AUTHENTICATE PLAIN";
        IRCActionEnvelope action = IRCActionEnvelope( get_id() , command );
        enqueue_action(action);
    } else {
        std::cerr << "Received an unprocessed CAP ACK message.  Report this message as a parsing bug." << std::endl;
        set_to_fail();
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

void splinterClient::handle_AUTHENTICATE( IRCEventEnvelope& event )
{
    report_event(event);

    // Send AUTHENTICATE command with base64-encoded credentials
    std::string auth = sasl_username_ + '\0' + sasl_username_ + '\0' + sasl_password_;

    std::string command = "AUTHENTICATE " + base64_encode(auth);
    IRCActionEnvelope action = IRCActionEnvelope( get_id() , command );
    enqueue_action(action);
}

void splinterClient::handle_RPL_SASLSUCCESS( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_LOGGEDIN( IRCEventEnvelope& event )
{
    report_event(event);

    // SASL authentication successful

    // Send CAP END command to end capability negotiation
    end_capabilites_negotiation();

    set_nick( nick_ );
    register_user(nick_);
}

void splinterClient::handle_ERR_SASLFAIL( IRCEventEnvelope& event )
{
    report_event(event);
    // SASL authentication failed
    set_to_fail();
}

void splinterClient::handle_ERR_SASLTOOLONG( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_SASLABORTED( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_SASLALREADY( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_SASLMECHS( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_STATSDLINE( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_unassociated_event( IRCEventEnvelope& event )
{
    std::cerr << "Received a command without a handler associated.  Report this message as a bug." << std::endl;
    std::cerr << event.to_json(1, 4, 1) << std::endl;
}

void splinterClient::handle_UNKNOWN( IRCEventEnvelope& event )
{
    std::cerr << "Unknown message command handled: " << std::endl;
    std::cerr << event.to_json(1, 4, 1) << std::endl;
}

void splinterClient::handle_pipes(IRCEventEnvelope &PRIVMSG_event)
{
    // do the pipe stuff
    std::string source_splinter_id;
    std::string source_sender;
    std::string target_splinter_id;
    std::string target_receiver;
    int pipe_id;
    bool preserve;
    bool routable = false;

    // for each pipe in pipes_
    for (const auto& entry : pipes_)
    {
        pipe_id = std::stoi( entry.first );

        // get the values representing the pipe
        source_splinter_id = std::to_string(std::get<0>(entry.second));
        source_sender = std::get<1>(entry.second);
        target_splinter_id = std::to_string(std::get<2>(entry.second));
        target_receiver = std::get<3>(entry.second);
        preserve = std::get<4>(entry.second);

        // load up our clients to orchestrate
        auto sourceClient = clients_.find(source_splinter_id);
        auto targetClient = clients_.find(target_splinter_id);

        if ( sourceClient == clients_.end())
        {
            send_private_message( owner_, "Source client not found for pipe: " + entry.first );
            pipe_destroy( owner_, pipe_id );
            continue;
        }

        if ( targetClient == clients_.end())
        {
            send_private_message( owner_, "Target client not found for pipe: " + entry.first );
            pipe_destroy( owner_, pipe_id );
            continue;
        }

        // is this splinter client processing the event the source of the pipe?
        if ( splinter_id_ != std::stoi(source_splinter_id) )
        {
            // not the source, so skip this pipe
            continue;
        }

        std::string origin;
        if ( PRIVMSG_event.get_type() == IRCEventEnvelope::Type::S_CHANNEL_MESSAGE )
        {
            if ( source_sender == PRIVMSG_event.get_scalar_attribute("target") )
            {
                origin = PRIVMSG_event.get_scalar_attribute("target");
                routable = true;
            } else {
                continue;
            }
        }

        if ( PRIVMSG_event.get_type() == IRCEventEnvelope::Type::S_PRIVATE_MESSAGE )
        {
            if ( source_sender == PRIVMSG_event.get_scalar_attribute("nick") )
            {
                origin = PRIVMSG_event.get_scalar_attribute("nick");
                routable = true;
            } else {
                continue;
            }
        }

        if ( PRIVMSG_event.get_scalar_attribute("message")[0] == '!' )
        {
            // it's a command so don't relay it
            routable = false;
            continue;
        }

        if ( routable )
        {
            if ( preserve )
            {
                // preserve the context
                // send the message to the target
                targetClient->second->send_private_message( target_receiver, source_splinter_id + "/" + origin + "/" + PRIVMSG_event.get_scalar_attribute("nick") + ": " + PRIVMSG_event.get_scalar_attribute("message") );
            } else {
                // don't preserve the context
                // send the message to the target
                targetClient->second->send_private_message( target_receiver, PRIVMSG_event.get_scalar_attribute("message") );
            }
        }
    }
}

void splinterClient::handle_S_CHANNEL_MESSAGE(IRCEventEnvelope& event)
{
    report_event(event);
    handle_ctcp( event );
    handle_command( event );
    handle_pipes( event );
}

void splinterClient::handle_S_PRIVATE_MESSAGE(IRCEventEnvelope &event)
{
    report_event(event);
    handle_ctcp( event );
    handle_command( event );
    handle_pipes( event );
}

void splinterClient::handle_ctcp_version(IRCEventEnvelope& event)
{
    std::string target = event.get_scalar_attribute("sender");
    std::string message = "NOTICE " + target + " :\x01VERSION splinterIRC\x01";
    IRCActionEnvelope action = IRCActionEnvelope( get_id() , message );
    enqueue_action(action);
}

void splinterClient::handle_ctcp_time( IRCEventEnvelope& event )
{
    std::string target = event.get_scalar_attribute("sender");
    std::string message = "NOTICE " + target + " :\x01TIME " + std::to_string(std::time(nullptr)) + "\x01";
    IRCActionEnvelope action = IRCActionEnvelope( get_id() , message );
    enqueue_action(action);
}

void splinterClient::handle_ctcp_ping(IRCEventEnvelope& event)
{
    std::string target = event.get_scalar_attribute("sender");
    std::string message = event.get_scalar_attribute("message");
    std::string ctcp_args = message.substr(6, message.size() - 7);
    message = "NOTICE " + target + " :\x01PING " + ctcp_args + "\x01";
    IRCActionEnvelope action = IRCActionEnvelope( get_id() , message );
    enqueue_action(action);
}

void splinterClient::handle_ctcp( IRCEventEnvelope& event )
{
    std::string message = event.get_scalar_attribute("message");

    // Check if the message is a CTCP message
    if (message.front() == '\x01' && message.back() == '\x01')
    {
        // Extract the CTCP command and its arguments
        std::string ctcp_message = message.substr(1, message.size() - 2);
        std::string ctcp_command;
        std::string ctcp_args;
        std::istringstream ctcp_ss(ctcp_message);
        if (std::getline(ctcp_ss, ctcp_command, ' '))
        {
            std::getline(ctcp_ss, ctcp_args);
            // Handle the CTCP command
            if (ctcp_command == "VERSION") {
                handle_ctcp_version( event );
            } else if (ctcp_command == "TIME") {
                handle_ctcp_time( event );
            } else if (ctcp_command == "PING") {
                handle_ctcp_ping( event );
            }
        }
    }
}

void splinterClient::handle_PING( IRCEventEnvelope& event )
{
    report_event(event);

    std::string target = event.get_scalar_attribute( "target" );
    send_ping_reply( target );
}

void splinterClient::handle_QUIT( IRCEventEnvelope& event )
{
    report_event(event);
    if ( event.get_scalar_attribute("nick") == nick_ )
    {
        die();
    }
}

void splinterClient::handle_JOIN( IRCEventEnvelope& event )
{
    // if someone joins a channel and you see it, you're in that channel
    report_event(event);
    std::string channel = event.get_scalar_attribute("channel");
    channels_add(channel);
}

void splinterClient::handle_PART( IRCEventEnvelope& event )
{
    report_event(event);
    std::string channel = event.get_scalar_attribute("channel");
    channels_del(channel);
}

void splinterClient::handle_KICK( IRCEventEnvelope& event )
{
    report_event(event);
    std::string channel = event.get_scalar_attribute("channel");
    channels_del(channel);
}

void splinterClient::handle_NOTICE( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_NICK( IRCEventEnvelope& event )
{
    report_event(event);
    std::string old_nick = event.get_scalar_attribute("old_nick");
    std::string new_nick = event.get_scalar_attribute("new_nick");
    if ( old_nick == nick_ )
    {
        nick_ = new_nick;
    }
}

void splinterClient::handle_TOPIC( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_INVITE( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERROR( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WELCOME( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_YOURHOST( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_CREATED( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_MYINFO( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ISUPPORT( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_BOUNCE( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_STATSCOMMANDS( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ENDOFSTATS( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_STATSUPTIME( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_UMODEIS( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_LUSERCLIENT( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_LUSEROP( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_LUSERUNKNOWN( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_LUSERCHANNELS( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_LUSERME( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ADMINME( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ADMINLOC1( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ADMINLOC2( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ADMINEMAIL( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_TRYAGAIN( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_LOCALUSERS( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_GLOBALUSERS( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOISCERTFP( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_NONE( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_AWAY( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_USERHOST( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_UNAWAY( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_NOWAWAY( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOREPLY( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ENDOFWHO( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOISREGNICK( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOISUSER( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOISSERVER( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOISOPERATOR( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOWASUSER( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOISIDLE( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ENDOFWHOIS( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOISCHANNELS( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOISSPECIAL( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_LISTSTART( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_LIST( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_LISTEND( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_CHANNELMODEIS( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_CREATIONTIME( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOISACCOUNT( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_NOTOPIC( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_TOPIC( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_TOPICWHOTIME( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_INVITELIST( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ENDOFINVITELIST( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOISACTUALLY( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_INVITING( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_INVEXLIST( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ENDOFINVEXLIST( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_EXCEPTLIST( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ENDOFEXCEPTLIST( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_VERSION( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_NAMREPLY( IRCEventEnvelope& event )
{
    report_event(event);
    for (auto &nick: event.get_array_attribute("channel_members"))
    {
        whois_user(nick);
    }
}

void splinterClient::handle_RPL_ENDOFNAMES( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_LINKS( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ENDOFLINKS( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_BANLIST( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ENDOFBANLIST( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ENDOFWHOWAS( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_INFO( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ENDOFINFO( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_MOTDSTART( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_MOTD( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ENDOFMOTD( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOISHOST( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOISMODES( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_YOUREOPER( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_REHASHING( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_TIME( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_UNKNOWNERROR( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NOSUCHNICK( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NOSUCHSERVER( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NOSUCHCHANNEL( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_CANNOTSENDTOCHAN( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_TOOMANYCHANNELS( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_WASNOSUCHNICK( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NOORIGIN( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NORECIPIENT( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NOTEXTTOSEND( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_INPUTTOOLONG( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_UNKNOWNCOMMAND( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NOMOTD( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NONICKNAMEGIVEN( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_ERRONEUSNICKNAME( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NICKNAMEINUSE( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NICKCOLLISION( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_USERNOTINCHANNEL( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NOTONCHANNEL( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_USERONCHANNEL( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NOTREGISTERED( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NEEDMOREPARAMS( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_ALREADYREGISTERED( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_PASSWDMISMATCH( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_YOUREBANNEDCREEP( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_CHANNELISFULL( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_UNKNOWNMODE( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_INVITEONLYCHAN( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_BANNEDFROMCHAN( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_BADCHANNELKEY( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_BADCHANMASK( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NOPRIVILEGES( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_CHANOPRIVSNEEDED( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_CANTKILLSERVER( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NOOPERHOST( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_UMODEUNKNOWNFLAG( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_USERSDONTMATCH( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_HELPNOTFOUND( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_INVALIDKEY( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_STARTTLS( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_WHOISSECURE( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_STARTTLS( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_INVALIDMODEPARAM( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_HELPSTART( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_HELPTXT( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_ENDOFHELP( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NOPRIVS( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_RPL_LOGGEDOUT( IRCEventEnvelope& event )
{
    report_event(event);
}

void splinterClient::handle_ERR_NICKLOCKED( IRCEventEnvelope& event )
{
    report_event(event);
}