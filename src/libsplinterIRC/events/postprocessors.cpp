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
    // std::cerr << "unknown RPL_CAP subcommand found" << std::endl;
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
{}

void IRCEventEnvelope::postprocess_S_CHANNEL_MESSAGE()
{}

void IRCEventEnvelope::postprocess_JOIN()
{
    std::string channel = autoextract_params[0];
    set_attribute( "channel", channel );
    set_attribute("_postprocessed", "true");
}

void IRCEventEnvelope::postprocess_RPL_LOGGEDIN()
{
    set_attribute( "account", autoextract_params[2] );
    set_attribute( "_postprocessed", "true" );
}


void IRCEventEnvelope::postprocess_AUTHENTICATE()
{
    std::string message = autoextract_params[0];
    set_attribute("message", message);
    set_attribute("sender", get_scalar_attribute( "_prefix" ) );
    set_attribute( "_postprocessed", "true" );
}

void IRCEventEnvelope::postprocess_RPL_TOPIC()
{
    std::string channel = autoextract_params[1];
    set_attribute( "channel", channel );

    std::string topic = autoextract_params[2];
    set_attribute( "topic", topic );

    std::string source = get_scalar_attribute( "server" );
    set_attribute( "source", source );

    std::string target = autoextract_params[0];
    set_attribute( "target", target );

    set_attribute("_postprocessed", "true");
}

void IRCEventEnvelope::postprocess_RPL_TOPICWHOTIME()
{
    std::string channel = autoextract_params[1];
    set_attribute( "channel", channel );

    std::string source = get_scalar_attribute( "server" );
    set_attribute( "source", source );

    std::string target = autoextract_params[0];
    set_attribute( "target", target );

    std::string set_by = autoextract_params[2];
    set_attribute( "set_by", set_by );

    std::string set_by_nick = split( set_by, '!' )[0];
    set_attribute( "set_by_nick", set_by_nick );

    std::string set_by_ident = split( split( set_by, '!' )[1], '@' )[0];
    set_attribute( "set_by_ident", set_by_ident );

    std::string set_by_host = split( split( set_by, '!' )[1], '@' )[1];
    set_attribute( "set_by_host", set_by_host );

    std::string time = autoextract_params[3];
    set_attribute( "time", time );

    set_attribute("_postprocessed", "true");
}

void IRCEventEnvelope::postprocess_RPL_NAMREPLY()
{
    std::string channel = autoextract_params[2];
    set_attribute( "channel", channel );

    set_attribute( "channel_members", split(autoextract_params[3], ' ') );
}

void IRCEventEnvelope::postprocess_ERR_CANNOTSENDTOCHAN()
{
    std::string channel = autoextract_params[1];
    std::string sender = get_scalar_attribute("server");
    std::string message = autoextract_params[2];
    std::string nick = autoextract_params[0];

    set_attribute("channel", channel);
    set_attribute("sender", sender);
    set_attribute("message", message);
    set_attribute("nick", nick);

    set_attribute("_postprocessed", "true");
}

void IRCEventEnvelope::postprocess_RPL_SASLSUCCESS()
{
    std::string message = autoextract_params[1];
    set_attribute("message", message);

    std::string sender = get_scalar_attribute("server");
    set_attribute("sender", sender);

    set_attribute("_postprocessed", "true");
}

void IRCEventEnvelope::postprocess_PART()
{
    std::string channel = autoextract_params[0];
    set_attribute( "channel", channel );

    std::string message = autoextract_params[1];
    set_attribute( "message", message );

    std::string sender = get_scalar_attribute( "_prefix" );
    set_attribute( "sender", sender );

    std::string nick = split( sender, '!' )[0];
    set_attribute( "nick", nick );

    std::string ident = split( split( sender, '!' )[1], '@' )[0];
    set_attribute( "ident", ident );

    std::string host = split( split( sender, '!' )[1], '@' )[1];
    set_attribute( "host", host );

    set_attribute("_postprocessed", "true");
}

void IRCEventEnvelope::postprocess_KICK()
{
    std::string channel = autoextract_params[0];
    set_attribute( "channel", channel );

    std::string target = autoextract_params[1];
    set_attribute( "target", target );

    std::string message = autoextract_params[2];
    set_attribute( "message", message );

    std::string sender = get_scalar_attribute( "_prefix" );
    set_attribute( "sender", sender );

    std::string nick = split( sender, '!' )[0];
    set_attribute( "nick", nick );

    std::string ident = split( split( sender, '!' )[1], '@' )[0];
    set_attribute( "ident", ident );

    std::string host = split( split( sender, '!' )[1], '@' )[1];
    set_attribute( "host", host );

    set_attribute("_postprocessed", "true");
}

void IRCEventEnvelope::postprocess_QUIT()
{
    std::string message = autoextract_params[0];
    set_attribute( "message", message );

    std::string sender = get_scalar_attribute( "_prefix" );
    set_attribute( "sender", sender );

    std::string nick = split( sender, '!' )[0];
    set_attribute( "nick", nick );

    std::string ident = split( split( sender, '!' )[1], '@' )[0];
    set_attribute( "ident", ident );

    std::string host = split( split( sender, '!' )[1], '@' )[1];
    set_attribute( "host", host );

    set_attribute("_postprocessed", "true");
}

void IRCEventEnvelope::postprocess_NICK()
{
    std::string new_nick = autoextract_params[0];
    set_attribute( "new_nick", new_nick );

    std::string sender = get_scalar_attribute( "_prefix" );
    set_attribute( "sender", sender );

    std::string old_nick = split(sender, '!' )[0];
    set_attribute("old_nick", old_nick );

    std::string ident = split( split( sender, '!' )[1], '@' )[0];
    set_attribute( "ident", ident );

    std::string host = split( split( sender, '!' )[1], '@' )[1];
    set_attribute( "host", host );

    set_attribute("_postprocessed", "true");
}

void IRCEventEnvelope::postprocess_TOPIC()
{
    std::string channel = autoextract_params[0];
    set_attribute( "channel", channel );

    std::string topic = autoextract_params[1];
    set_attribute( "topic", topic );

    std::string sender = get_scalar_attribute( "_prefix" );
    set_attribute( "sender", sender );

    std::string nick = split( sender, '!' )[0];
    set_attribute( "nick", nick );

    std::string ident = split( split( sender, '!' )[1], '@' )[0];
    set_attribute( "ident", ident );

    std::string host = split( split( sender, '!' )[1], '@' )[1];
    set_attribute( "host", host );

    set_attribute("_postprocessed", "true");
}

void IRCEventEnvelope::postprocess_INVITE()
{
    std::string channel = autoextract_params[1];
    set_attribute( "channel", channel );

    std::string sender = get_scalar_attribute( "_prefix" );
    set_attribute( "sender", sender );

    std::string nick = split( sender, '!' )[0];
    set_attribute( "nick", nick );

    std::string ident = split( split( sender, '!' )[1], '@' )[0];
    set_attribute( "ident", ident );

    std::string host = split( split( sender, '!' )[1], '@' )[1];
    set_attribute( "host", host );

    set_attribute("_postprocessed", "true");
}

void IRCEventEnvelope::postprocess_RPL_WELCOME()
{
    std::string message = autoextract_params[1];
    set_attribute( "message", message );

    std::string sender = get_scalar_attribute( "server" );
    set_attribute( "sender", sender );

    std::string target = get_scalar_attribute( "nick" );
    set_attribute( "target", target );

    set_attribute("_postprocessed", "true");
}

void IRCEventEnvelope::postprocess_RPL_YOURHOST()
{
    std::string message = autoextract_params[1];
    set_attribute( "message", message );

    std::string sender = get_scalar_attribute( "server" );
    set_attribute( "sender", sender );

    std::string target = autoextract_params[0];
    set_attribute( "target", target );

    set_attribute("_postprocessed", "true");
}

void IRCEventEnvelope::postprocess_RPL_CREATED()
{
    std::string message = autoextract_params[1];
    set_attribute( "message", message );

    std::string sender = get_scalar_attribute( "server" );
    set_attribute( "sender", sender );

    std::string target = autoextract_params[0];
    set_attribute( "target", target );

    set_attribute("_postprocessed", "true");
}

void IRCEventEnvelope::postprocess_RPL_MYINFO()
{
    // TODO - add attributes for the various fields to client

    std::string sender = get_scalar_attribute( "server" );
    set_attribute( "sender", sender );

    std::string target = autoextract_params[0];
    set_attribute( "target", target );

    std::string version = autoextract_params[2];
    set_attribute("version", version );

    std::string available_umodes = autoextract_params[3];
    set_attribute("available_umodes", available_umodes );

    std::string available_cmodes = autoextract_params[4];
    set_attribute("available_cmodes", available_cmodes );

    std::string available_cmodes_with_params = autoextract_params[5];
    set_attribute("available_cmodes_with_params", available_cmodes_with_params );

    set_attribute("_postprocessed", "true");
}

void IRCEventEnvelope::postprocess_RPL_ISUPPORT()
{
    // https://dd.ircdocs.horse/refs/numerics/005.html

    // TODO: add supported features vector to client

    std::string sender = get_scalar_attribute( "server" );
    set_attribute( "sender", sender );

    std::string target = autoextract_params[0];
    set_attribute( "target", target );

    std::string message = autoextract_params.back();
    set_attribute( "message", message );

    std::vector<std::string> tokens(autoextract_params.begin() + 1, autoextract_params.end() - 1);
    set_attribute( "supported_features", tokens );
}

/*
 *  RPL_BOUNCE (010)
 *  "<client> <hostname> <port> :<info>"
 *  Sent to the client to redirect it to another server.
 *  The <info> text varies between server software and
 *  reasons for the redirection.
 *
 *  Because this numeric does not specify whether to
 *  enable SSL and is not interpreted correctly by all
 *  clients, it is recommended that this not be used.
 */
void IRCEventEnvelope::postprocess_RPL_BOUNCE()
{
    std::string new_server = autoextract_params[1];
    set_attribute( "new_server", new_server );

    std::string sender = get_scalar_attribute( "server" );
    set_attribute( "sender", sender );

    std::string target = autoextract_params[0];
    set_attribute( "target", target );

    set_attribute("_postprocessed", "true");
}

/*
 * RPL_STATSCOMMANDS (212)
 * "<client> <command> <count> [<byte count> <remote count>]"
 *
 * Sent as a reply to the STATS command, when a client
 * requests statistics on command usage.
 *
 * <byte count> and <remote count> are optional and MAY be
 * included in responses.
 */
void IRCEventEnvelope::postprocess_RPL_STATSCOMMANDS()
{
    std::string sender = get_scalar_attribute( "server" );
    set_attribute( "sender", sender );

    std::string target = autoextract_params[0];
    set_attribute( "target", target );

    std::string command = autoextract_params[1];
    set_attribute( "command", command );

    std::string count = autoextract_params[2];
    set_attribute( "count", count );

    if ( autoextract_params.size() > 3 )
    {
        std::string byte_count = autoextract_params[3];
        set_attribute( "byte_count", byte_count );

        std::string remote_count = autoextract_params[4];
        set_attribute( "remote_count", remote_count );
    }

    set_attribute("_postprocessed", "true");
}

void IRCEventEnvelope::postprocess_RPL_ENDOFSTATS()
{
    std::string message = autoextract_params[1];
    set_attribute( "message", message );

    std::string sender = get_scalar_attribute( "server" );
    set_attribute( "sender", sender );

    std::string target = autoextract_params[0];
    set_attribute( "target", target );

    set_attribute("_postprocessed", "true");
}

void IRCEventEnvelope::postprocess_RPL_STATSUPTIME()
{
    std::string message = autoextract_params[1];
    set_attribute( "message", message );

    std::string sender = get_scalar_attribute( "server" );
    set_attribute( "sender", sender );

    std::string target = autoextract_params[0];
    set_attribute( "target", target );

    set_attribute("_postprocessed", "true");
}

void IRCEventEnvelope::postprocess_RPL_UMODEIS()
{
    std::string sender = get_scalar_attribute( "server" );
    set_attribute( "sender", sender );

    std::string target = autoextract_params[0];
    set_attribute( "target", target );

    // TODO - add attributes for the various fields to client
    std::string mode_change = autoextract_params[1];
    set_attribute( "mode_change", mode_change );

    // TODO update set_attribute to take a boolean
    // TODO update get_attribute to take/return appropriate generic types for json instead of this split brain thing we're doing
    set_attribute( "_postprocessed", "true" );
}

/*
 *
 * RPL_LUSERCLIENT (251)
 *
 * "<client> :There are <u> users and <i> invisible on <s> servers"
 *
 * Sent as a reply to the LUSERS command. <u>, <i>, and <s> are
 * non-negative integers, and represent the number of total users,
 * invisible users, and other servers connected to this server.
 */
void IRCEventEnvelope::postprocess_RPL_LUSERCLIENT()
{
    std::string message = autoextract_params[1];
    set_attribute( "message", message );

    std::string sender = get_scalar_attribute( "server" );
    set_attribute( "sender", sender );

    std::string target = autoextract_params[0];
    set_attribute( "target", target );

    set_attribute("_postprocessed", "true");
}

void IRCEventEnvelope::postprocess_RPL_LUSEROP()
{
    std::string message = autoextract_params[1];
    set_attribute( "message", message );

    std::string sender = get_scalar_attribute( "server" );
    set_attribute( "sender", sender );

    std::string target = autoextract_params[0];
    set_attribute( "target", target );

    set_attribute("_postprocessed", "true");
}

void IRCEventEnvelope::postprocess_RPL_LUSERUNKNOWN()
{
    std::string message = autoextract_params[1];
    set_attribute( "message", message );

    std::string sender = get_scalar_attribute( "server" );
    set_attribute( "sender", sender );

    std::string target = autoextract_params[0];
    set_attribute( "target", target );

    set_attribute("_postprocessed", "true");
}

/*
 * RPL_LUSERCHANNELS (254)
 *
 * "<client> <channels> :channels formed"
 *
 * Sent as a reply to the LUSERS command. <channels> is
 * a positive integer and represents the number of channels
 * that currently exist on this server. The text used in
 * the last param of this message may vary.
 */
void IRCEventEnvelope::postprocess_RPL_LUSERCHANNELS()
{
    std::string message = autoextract_params[1];
    set_attribute( "message", message );

    std::string sender = get_scalar_attribute( "server" );
    set_attribute( "sender", sender );

    std::string target = autoextract_params[0];
    set_attribute( "target", target );

    set_attribute("_postprocessed", "true");

}

void IRCEventEnvelope::postprocess_RPL_STATSDLINE()
{
    std::string sender = get_scalar_attribute( "server" );
    set_attribute( "sender", sender );

    std::string target = autoextract_params[0];
    set_attribute( "target", target );

    // TODO - add attributes for the various fields to client
    std::string message = autoextract_params[1];
    set_attribute( "message", message );

    // TODO update set_attribute to take a boolean
    // TODO update get_attribute to take/return appropriate generic types for json instead of this split brain thing we're doing
    set_attribute( "_postprocessed", "true" );
}

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
{
    std::string motd = autoextract_params[1];
    set_attribute( "message", motd );

    std::string sender = get_scalar_attribute( "server");
    set_attribute( "sender", sender );

    std::string target = autoextract_params[0];
    set_attribute( "target", target );

    // TODO update set_attribute to take a boolean
    // TODO update get_attribute to take/return appropriate generic types for json instead of this split brain thing we're doing
    set_attribute( "_postprocessed", "true" );
}

void IRCEventEnvelope::postprocess_RPL_MOTD()
{
    std::string motd = autoextract_params[1];
    set_attribute( "message", motd );

    std::string sender = get_scalar_attribute( "server");
    set_attribute( "sender", sender );

    std::string target = autoextract_params[0];
    set_attribute( "target", target );

    // TODO update set_attribute to take a boolean
    // TODO update get_attribute to take/return appropriate generic types for json instead of this split brain thing we're doing
    set_attribute( "_postprocessed", "true" );
}

void IRCEventEnvelope::postprocess_RPL_ENDOFMOTD()
{
    std::string motd = autoextract_params[1];
    set_attribute( "message", motd );

    std::string sender = get_scalar_attribute( "server");
    set_attribute( "sender", sender );

    std::string target = autoextract_params[0];
    set_attribute( "target", target );

    // TODO update set_attribute to take a boolean
    // TODO update get_attribute to take/return appropriate generic types for json instead of this split brain thing we're doing
    set_attribute( "_postprocessed", "true" );
}

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

void IRCEventEnvelope::postprocess_RPL_LOGGEDOUT()
{}

void IRCEventEnvelope::postprocess_ERR_NICKLOCKED()
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
