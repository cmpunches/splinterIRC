#include "IRCEventv3.h"

void IRCEventEnvelope::postprocess_S_RPL_CAP()
{
    // assume it's an undefined synthetic type until we know better
    Type subtype = Type::S_RPL_CAP_UNKNOWN;

    // get the CAP subcommand and compare to known subcommands
    // set the new type to the appropriate synthetic type when recognized
    if (autoextract_params[1] == "LS") {
        subtype = Type::S_RPL_CAP_LS;
    } else if (autoextract_params[1] == "ACK") {
        subtype = Type::S_RPL_CAP_ACK;
    } else if (autoextract_params[1] == "NAK") {
        subtype = Type::S_RPL_CAP_NAK;
    }

    // set the new type
    type_ = subtype;

    // make the new synthetic type visible and human readable, assuming there is only one layer of synthetic types for
    // CAP
    set_attribute( "type", type_to_verb( subtype ) );

    // call the appropriate postprocessor based on type
    switch ( type_ )
    {
        case Type::S_RPL_CAP_LS:
            postprocess_S_RPL_CAP_LS();
            break;
        case Type::S_RPL_CAP_ACK:
            postprocess_S_RPL_CAP_ACK();
            break;
        default:
            postprocess_S_RPL_CAP_UNKNOWN();
            break;
    }
}

void IRCEventEnvelope::postprocess_S_RPL_CAP_LS()
{
    // set the server capabilities
    // the capabilities list from the server is space delimited and can be represented as a vector of strings
    // (flat array for json)
    set_attribute( "server_capabilities", split(autoextract_params[2], ' ') );
    set_attribute( "_postprocessed", "true" );
}

void IRCEventEnvelope::postprocess_S_RPL_CAP_ACK()
{
    set_attribute("sasl_available", "true");
    set_attribute("ack_capability", autoextract_params[2]);
    set_attribute("_postprocessed", "true");
}

void IRCEventEnvelope::postprocess_S_RPL_CAP_UNKNOWN()
{
    // let the event handler handle these for now but this will allow us to handle them in the future at parsing
    // in case we want to do something with them to prep them for better handling
    // std::cerr << "unknown S_RPL_CAP subcommand found" << std::endl;
}

void IRCEventEnvelope::postprocess_UNKNOWN()
{
    // let the event handler handle these for now but this will allow us to handle them in the future at parsing
    // in case we want to do something with them to prep them for better handling
    // std::cerr << "unknown found" << std::endl;
}

void IRCEventEnvelope::postprocess_NOTICE()
{
    std::string message = autoextract_params[1];
    set_attribute("message", message);
    set_attribute("sender", get_scalar_attribute( "_prefix" ) );
    set_attribute("type", type_to_verb( Type::NOTICE ) );
    type_ = Type::NOTICE;
    set_attribute("_postprocessed", "true");
}

void IRCEventEnvelope::postprocess_ERROR()
{
    set_attribute( "message", autoextract_params[0] );
    set_attribute( "sender", get_scalar_attribute( "server" ) );
    set_attribute( "_postprocessed", "true" );
}

void IRCEventEnvelope::postprocess_PRIVMSG()
{
    std::string target = autoextract_params[0];
    set_attribute( "target", target );

    std::string prefix = get_scalar_attribute( "_prefix" );
    set_attribute( "sender", prefix );

    std::string nick = split(prefix, '!')[0];
    set_attribute( "nick", nick );

    std::string host = split(prefix, '@')[1];
    set_attribute( "host", host );

    std::string ident = split(split(prefix, '!')[1], '@')[0];
    set_attribute( "ident", ident );

    set_attribute( "message", autoextract_params[1] );

    if (!target.empty() && target[0] == '#') {
        set_attribute( "type", type_to_verb( Type::S_CHANNEL_MESSAGE ) );
        type_ = Type::S_CHANNEL_MESSAGE;
        postprocess_S_CHANNEL_MESSAGE();
    } else {
        set_attribute( "type", type_to_verb( Type::S_PRIVATE_MESSAGE ) );
        type_ = Type::S_PRIVATE_MESSAGE;
        postprocess_S_PRIVATE_MESSAGE();
    }
}

void IRCEventEnvelope::postprocess_PING()
{
    set_attribute( "source", get_scalar_attribute("server") );
    set_attribute( "target", autoextract_params[0] );
    set_attribute( "_postprocessed", "true" );
}

void IRCEventEnvelope::postprocess_S_PRIVATE_MESSAGE()
{

}

void IRCEventEnvelope::postprocess_S_CHANNEL_MESSAGE()
{

}

void IRCEventEnvelope::postprocess_JOIN()
{}

void IRCEventEnvelope::postprocess_PART()
{}

void IRCEventEnvelope::postprocess_KICK()
{}

void IRCEventEnvelope::postprocess_QUIT()
{}

void IRCEventEnvelope::postprocess_NICK()
{}

void IRCEventEnvelope::postprocess_TOPIC()
{}

void IRCEventEnvelope::postprocess_INVITE()
{}

void IRCEventEnvelope::postprocess_AUTHENTICATE()
{}

void IRCEventEnvelope::postprocess_RPL_WELCOME()
{}

void IRCEventEnvelope::postprocess_RPL_YOURHOST()
{}

void IRCEventEnvelope::postprocess_RPL_CREATED()
{}

void IRCEventEnvelope::postprocess_RPL_MYINFO()
{}

void IRCEventEnvelope::postprocess_RPL_ISUPPORT()
{}

void IRCEventEnvelope::postprocess_RPL_BOUNCE()
{}

void IRCEventEnvelope::postprocess_RPL_STATSCOMMANDS()
{}

void IRCEventEnvelope::postprocess_RPL_ENDOFSTATS()
{}

void IRCEventEnvelope::postprocess_RPL_STATSUPTIME()
{}

void IRCEventEnvelope::postprocess_RPL_UMODEIS()
{}

void IRCEventEnvelope::postprocess_RPL_LUSERCLIENT()
{}

void IRCEventEnvelope::postprocess_RPL_LUSEROP()
{}

void IRCEventEnvelope::postprocess_RPL_LUSERUNKNOWN()
{}

void IRCEventEnvelope::postprocess_RPL_LUSERCHANNELS()
{}

void IRCEventEnvelope::postprocess_RPL_LUSERME()
{}

void IRCEventEnvelope::postprocess_RPL_ADMINME()
{}

void IRCEventEnvelope::postprocess_RPL_ADMINLOC1()
{}

void IRCEventEnvelope::postprocess_RPL_ADMINLOC2()
{}

void IRCEventEnvelope::postprocess_RPL_ADMINEMAIL()
{}

void IRCEventEnvelope::postprocess_RPL_TRYAGAIN()
{}

void IRCEventEnvelope::postprocess_RPL_LOCALUSERS()
{}

void IRCEventEnvelope::postprocess_RPL_GLOBALUSERS()
{}

void IRCEventEnvelope::postprocess_RPL_WHOISCERTFP()
{}

void IRCEventEnvelope::postprocess_RPL_NONE()
{}

void IRCEventEnvelope::postprocess_RPL_AWAY()
{}

void IRCEventEnvelope::postprocess_RPL_USERHOST()
{}

void IRCEventEnvelope::postprocess_RPL_UNAWAY()
{}

void IRCEventEnvelope::postprocess_RPL_NOWAWAY()
{}

void IRCEventEnvelope::postprocess_RPL_WHOREPLY()
{}

void IRCEventEnvelope::postprocess_RPL_ENDOFWHO()
{}

void IRCEventEnvelope::postprocess_RPL_WHOISREGNICK()
{}

void IRCEventEnvelope::postprocess_RPL_WHOISUSER()
{}

void IRCEventEnvelope::postprocess_RPL_WHOISSERVER()
{}

void IRCEventEnvelope::postprocess_RPL_WHOISOPERATOR()
{}

void IRCEventEnvelope::postprocess_RPL_WHOWASUSER()
{}

void IRCEventEnvelope::postprocess_RPL_WHOISIDLE()
{}

void IRCEventEnvelope::postprocess_RPL_ENDOFWHOIS()
{}

void IRCEventEnvelope::postprocess_RPL_WHOISCHANNELS()
{}

void IRCEventEnvelope::postprocess_RPL_WHOISSPECIAL()
{}

void IRCEventEnvelope::postprocess_RPL_LISTSTART()
{}

void IRCEventEnvelope::postprocess_RPL_LIST()
{}

void IRCEventEnvelope::postprocess_RPL_LISTEND()
{}

void IRCEventEnvelope::postprocess_RPL_CHANNELMODEIS()
{}

void IRCEventEnvelope::postprocess_RPL_CREATIONTIME()
{}

void IRCEventEnvelope::postprocess_RPL_WHOISACCOUNT()
{}

void IRCEventEnvelope::postprocess_RPL_NOTOPIC()
{}

void IRCEventEnvelope::postprocess_RPL_TOPIC()
{}

void IRCEventEnvelope::postprocess_RPL_TOPICWHOTIME()
{}

void IRCEventEnvelope::postprocess_RPL_INVITELIST()
{}

void IRCEventEnvelope::postprocess_RPL_ENDOFINVITELIST()
{}

void IRCEventEnvelope::postprocess_RPL_WHOISACTUALLY()
{}

void IRCEventEnvelope::postprocess_RPL_INVITING()
{}

void IRCEventEnvelope::postprocess_RPL_INVEXLIST()
{}

void IRCEventEnvelope::postprocess_RPL_ENDOFINVEXLIST()
{}

void IRCEventEnvelope::postprocess_RPL_EXCEPTLIST()
{}

void IRCEventEnvelope::postprocess_RPL_ENDOFEXCEPTLIST()
{}

void IRCEventEnvelope::postprocess_RPL_VERSION()
{}

void IRCEventEnvelope::postprocess_RPL_NAMREPLY()
{}

void IRCEventEnvelope::postprocess_RPL_ENDOFNAMES()
{}

void IRCEventEnvelope::postprocess_RPL_LINKS()
{}

void IRCEventEnvelope::postprocess_RPL_ENDOFLINKS()
{}

void IRCEventEnvelope::postprocess_RPL_BANLIST()
{}

void IRCEventEnvelope::postprocess_RPL_ENDOFBANLIST()
{}

void IRCEventEnvelope::postprocess_RPL_ENDOFWHOWAS()
{}

void IRCEventEnvelope::postprocess_RPL_INFO()
{}

void IRCEventEnvelope::postprocess_RPL_ENDOFINFO()
{}

void IRCEventEnvelope::postprocess_RPL_MOTDSTART()
{}

void IRCEventEnvelope::postprocess_RPL_MOTD()
{}

void IRCEventEnvelope::postprocess_RPL_ENDOFMOTD()
{}

void IRCEventEnvelope::postprocess_RPL_WHOISHOST()
{}

void IRCEventEnvelope::postprocess_RPL_WHOISMODES()
{}

void IRCEventEnvelope::postprocess_RPL_YOUREOPER()
{}

void IRCEventEnvelope::postprocess_RPL_REHASHING()
{}

void IRCEventEnvelope::postprocess_RPL_TIME()
{}

void IRCEventEnvelope::postprocess_ERR_UNKNOWNERROR()
{}

void IRCEventEnvelope::postprocess_ERR_NOSUCHNICK()
{}

void IRCEventEnvelope::postprocess_ERR_NOSUCHSERVER()
{}

void IRCEventEnvelope::postprocess_ERR_NOSUCHCHANNEL()
{}

void IRCEventEnvelope::postprocess_ERR_CANNOTSENDTOCHAN()
{}

void IRCEventEnvelope::postprocess_ERR_TOOMANYCHANNELS()
{}

void IRCEventEnvelope::postprocess_ERR_WASNOSUCHNICK()
{}

void IRCEventEnvelope::postprocess_ERR_NOORIGIN()
{}

void IRCEventEnvelope::postprocess_ERR_NORECIPIENT()
{}

void IRCEventEnvelope::postprocess_ERR_NOTEXTTOSEND()
{}

void IRCEventEnvelope::postprocess_ERR_INPUTTOOLONG()
{}

void IRCEventEnvelope::postprocess_ERR_UNKNOWNCOMMAND()
{}

void IRCEventEnvelope::postprocess_ERR_NOMOTD()
{}

void IRCEventEnvelope::postprocess_ERR_NONICKNAMEGIVEN()
{}

void IRCEventEnvelope::postprocess_ERR_ERRONEUSNICKNAME()
{}

void IRCEventEnvelope::postprocess_ERR_NICKNAMEINUSE()
{}

void IRCEventEnvelope::postprocess_ERR_NICKCOLLISION()
{}

void IRCEventEnvelope::postprocess_ERR_USERNOTINCHANNEL()
{}

void IRCEventEnvelope::postprocess_ERR_NOTONCHANNEL()
{}

void IRCEventEnvelope::postprocess_ERR_USERONCHANNEL()
{}

void IRCEventEnvelope::postprocess_ERR_NOTREGISTERED()
{}

void IRCEventEnvelope::postprocess_ERR_NEEDMOREPARAMS()
{}

void IRCEventEnvelope::postprocess_ERR_ALREADYREGISTERED()
{}

void IRCEventEnvelope::postprocess_ERR_PASSWDMISMATCH()
{}

void IRCEventEnvelope::postprocess_ERR_YOUREBANNEDCREEP()
{}

void IRCEventEnvelope::postprocess_ERR_CHANNELISFULL()
{}

void IRCEventEnvelope::postprocess_ERR_UNKNOWNMODE()
{}

void IRCEventEnvelope::postprocess_ERR_INVITEONLYCHAN()
{}

void IRCEventEnvelope::postprocess_ERR_BANNEDFROMCHAN()
{}

void IRCEventEnvelope::postprocess_ERR_BADCHANNELKEY()
{}

void IRCEventEnvelope::postprocess_ERR_BADCHANMASK()
{}

void IRCEventEnvelope::postprocess_ERR_NOPRIVILEGES()
{}

void IRCEventEnvelope::postprocess_ERR_CHANOPRIVSNEEDED()
{}

void IRCEventEnvelope::postprocess_ERR_CANTKILLSERVER()
{}

void IRCEventEnvelope::postprocess_ERR_NOOPERHOST()
{}

void IRCEventEnvelope::postprocess_ERR_UMODEUNKNOWNFLAG()
{}

void IRCEventEnvelope::postprocess_ERR_USERSDONTMATCH()
{}

void IRCEventEnvelope::postprocess_ERR_HELPNOTFOUND()
{}

void IRCEventEnvelope::postprocess_ERR_INVALIDKEY()
{}

void IRCEventEnvelope::postprocess_RPL_STARTTLS()
{}

void IRCEventEnvelope::postprocess_RPL_WHOISSECURE()
{}

void IRCEventEnvelope::postprocess_ERR_STARTTLS()
{}

void IRCEventEnvelope::postprocess_ERR_INVALIDMODEPARAM()
{}

void IRCEventEnvelope::postprocess_RPL_HELPSTART()
{}

void IRCEventEnvelope::postprocess_RPL_HELPTXT()
{}

void IRCEventEnvelope::postprocess_RPL_ENDOFHELP()
{}

void IRCEventEnvelope::postprocess_ERR_NOPRIVS()
{}

void IRCEventEnvelope::postprocess_RPL_LOGGEDIN()
{}

void IRCEventEnvelope::postprocess_RPL_LOGGEDOUT()
{}

void IRCEventEnvelope::postprocess_ERR_NICKLOCKED()
{}

void IRCEventEnvelope::postprocess_RPL_SASLSUCCESS()
{}

void IRCEventEnvelope::postprocess_ERR_SASLFAIL()
{}

void IRCEventEnvelope::postprocess_ERR_SASLTOOLONG()
{}

void IRCEventEnvelope::postprocess_ERR_SASLABORTED()
{}

void IRCEventEnvelope::postprocess_ERR_SASLALREADY()
{}

void IRCEventEnvelope::postprocess_RPL_SASLMECHS()
{}
