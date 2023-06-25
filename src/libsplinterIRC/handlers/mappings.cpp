#include "../client/client.h"

// decision/action
void splinterClient::decision_loop(IRCEventEnvelope& event)
{
    // command mappings for actions and events
    switch (event.get_type() )
    {
        case IRCEventEnvelope::Type::UNKNOWN:
            handle_UNKNOWN(event);
            break;

        case IRCEventEnvelope::Type::JOIN:
            handle_JOIN(event);
            break;

        case IRCEventEnvelope::Type::PART:
            handle_PART(event);
            break;

        case IRCEventEnvelope::Type::KICK:
            handle_KICK(event);
            break;

        case IRCEventEnvelope::Type::NOTICE:
            handle_NOTICE(event);
            break;

        case IRCEventEnvelope::Type::PING:
            handle_PING(event);
            break;

        case IRCEventEnvelope::Type::QUIT:
            handle_QUIT(event);
            break;

        case IRCEventEnvelope::Type::NICK:
            handle_NICK(event);
            break;

        case IRCEventEnvelope::Type::TOPIC:
            handle_TOPIC(event);
            break;

        case IRCEventEnvelope::Type::INVITE:
            handle_INVITE(event);
            break;

        case IRCEventEnvelope::Type::ERROR:
            handle_ERROR(event);
            break;

        case IRCEventEnvelope::Type::S_PRIVATE_MESSAGE:
            handle_S_PRIVATE_MESSAGE(event);
            break;

        case IRCEventEnvelope::Type::S_CHANNEL_MESSAGE:
            handle_S_CHANNEL_MESSAGE(event);
            break;

        case IRCEventEnvelope::Type::RPL_WELCOME:
            handle_RPL_WELCOME(event);
            break;

        case IRCEventEnvelope::Type::RPL_YOURHOST:
            handle_RPL_YOURHOST(event);
            break;

        case IRCEventEnvelope::Type::RPL_CREATED:
            handle_RPL_CREATED(event);
            break;

        case IRCEventEnvelope::Type::RPL_MYINFO:
            handle_RPL_MYINFO(event);
            break;

        case IRCEventEnvelope::Type::RPL_ISUPPORT:
            handle_RPL_ISUPPORT(event);
            break;

        case IRCEventEnvelope::Type::RPL_BOUNCE:
            handle_RPL_BOUNCE(event);
            break;

        case IRCEventEnvelope::Type::RPL_STATSCOMMANDS:
            handle_RPL_STATSCOMMANDS(event);
            break;

        case IRCEventEnvelope::Type::RPL_ENDOFSTATS:
            handle_RPL_ENDOFSTATS(event);
            break;

        case IRCEventEnvelope::Type::RPL_STATSUPTIME:
            handle_RPL_STATSUPTIME(event);
            break;

        case IRCEventEnvelope::Type::RPL_UMODEIS:
            handle_RPL_UMODEIS(event);
            break;

        case IRCEventEnvelope::Type::RPL_LUSERCLIENT:
            handle_RPL_LUSERCLIENT(event);
            break;

        case IRCEventEnvelope::Type::RPL_LUSEROP:
            handle_RPL_LUSEROP(event);
            break;

        case IRCEventEnvelope::Type::RPL_LUSERUNKNOWN:
            handle_RPL_LUSERUNKNOWN(event);
            break;

        case IRCEventEnvelope::Type::RPL_LUSERCHANNELS:
            handle_RPL_LUSERCHANNELS(event);
            break;

        case IRCEventEnvelope::Type::RPL_LUSERME:
            handle_RPL_LUSERME(event);
            break;

        case IRCEventEnvelope::Type::RPL_ADMINME:
            handle_RPL_ADMINME(event);
            break;

        case IRCEventEnvelope::Type::RPL_ADMINLOC1:
            handle_RPL_ADMINLOC1(event);
            break;

        case IRCEventEnvelope::Type::RPL_ADMINLOC2:
            handle_RPL_ADMINLOC2(event);
            break;

        case IRCEventEnvelope::Type::RPL_ADMINEMAIL:
            handle_RPL_ADMINEMAIL(event);
            break;

        case IRCEventEnvelope::Type::RPL_TRYAGAIN:
            handle_RPL_TRYAGAIN(event);
            break;

        case IRCEventEnvelope::Type::RPL_LOCALUSERS:
            handle_RPL_LOCALUSERS(event);
            break;

        case IRCEventEnvelope::Type::RPL_GLOBALUSERS:
            handle_RPL_GLOBALUSERS(event);
            break;

        case IRCEventEnvelope::Type::RPL_WHOISCERTFP:
            handle_RPL_WHOISCERTFP(event);
            break;

        case IRCEventEnvelope::Type::RPL_NONE:
            handle_RPL_NONE(event);
            break;

        case IRCEventEnvelope::Type::RPL_AWAY:
            handle_RPL_AWAY(event);
            break;

        case IRCEventEnvelope::Type::RPL_USERHOST:
            handle_RPL_USERHOST(event);
            break;

        case IRCEventEnvelope::Type::RPL_UNAWAY:
            handle_RPL_UNAWAY(event);
            break;

        case IRCEventEnvelope::Type::RPL_NOWAWAY:
            handle_RPL_NOWAWAY(event);
            break;

        case IRCEventEnvelope::Type::RPL_WHOREPLY:
            handle_RPL_WHOREPLY(event);
            break;

        case IRCEventEnvelope::Type::RPL_ENDOFWHO:
            handle_RPL_ENDOFWHO(event);
            break;

        case IRCEventEnvelope::Type::RPL_WHOISREGNICK:
            handle_RPL_WHOISREGNICK(event);
            break;

        case IRCEventEnvelope::Type::RPL_WHOISUSER:
            handle_RPL_WHOISUSER(event);
            break;

        case IRCEventEnvelope::Type::RPL_WHOISSERVER:
            handle_RPL_WHOISSERVER(event);
            break;

        case IRCEventEnvelope::Type::RPL_WHOISOPERATOR:
            handle_RPL_WHOISOPERATOR(event);
            break;

        case IRCEventEnvelope::Type::RPL_WHOWASUSER:
            handle_RPL_WHOWASUSER(event);
            break;

        case IRCEventEnvelope::Type::RPL_WHOISIDLE:
            handle_RPL_WHOISIDLE(event);
            break;

        case IRCEventEnvelope::Type::RPL_ENDOFWHOIS:
            handle_RPL_ENDOFWHOIS(event);
            break;

        case IRCEventEnvelope::Type::RPL_WHOISCHANNELS:
            handle_RPL_WHOISCHANNELS(event);
            break;

        case IRCEventEnvelope::Type::RPL_WHOISSPECIAL:
            handle_RPL_WHOISSPECIAL(event);
            break;

        case IRCEventEnvelope::Type::RPL_LISTSTART:
            handle_RPL_LISTSTART(event);
            break;

        case IRCEventEnvelope::Type::RPL_LIST:
            handle_RPL_LIST(event);
            break;

        case IRCEventEnvelope::Type::RPL_LISTEND:
            handle_RPL_LISTEND(event);
            break;

        case IRCEventEnvelope::Type::RPL_CHANNELMODEIS:
            handle_RPL_CHANNELMODEIS(event);
            break;

        case IRCEventEnvelope::Type::RPL_CREATIONTIME:
            handle_RPL_CREATIONTIME(event);
            break;

        case IRCEventEnvelope::Type::RPL_WHOISACCOUNT:
            handle_RPL_WHOISACCOUNT(event);
            break;

        case IRCEventEnvelope::Type::RPL_NOTOPIC:
            handle_RPL_NOTOPIC(event);
            break;

        case IRCEventEnvelope::Type::RPL_TOPIC:
            handle_RPL_TOPIC(event);
            break;

        case IRCEventEnvelope::Type::RPL_TOPICWHOTIME:
            handle_RPL_TOPICWHOTIME(event);
            break;

        case IRCEventEnvelope::Type::RPL_INVITELIST:
            handle_RPL_INVITELIST(event);
            break;

        case IRCEventEnvelope::Type::RPL_ENDOFINVITELIST:
            handle_RPL_ENDOFINVITELIST(event);
            break;

        case IRCEventEnvelope::Type::RPL_WHOISACTUALLY:
            handle_RPL_WHOISACTUALLY(event);
            break;

        case IRCEventEnvelope::Type::RPL_INVITING:
            handle_RPL_INVITING(event);
            break;

        case IRCEventEnvelope::Type::RPL_INVEXLIST:
            handle_RPL_INVEXLIST(event);
            break;

        case IRCEventEnvelope::Type::RPL_ENDOFINVEXLIST:
            handle_RPL_ENDOFINVEXLIST(event);
            break;

        case IRCEventEnvelope::Type::RPL_EXCEPTLIST:
            handle_RPL_EXCEPTLIST(event);
            break;

        case IRCEventEnvelope::Type::RPL_ENDOFEXCEPTLIST:
            handle_RPL_ENDOFEXCEPTLIST(event);
            break;

        case IRCEventEnvelope::Type::RPL_VERSION:
            handle_RPL_VERSION(event);
            break;

        case IRCEventEnvelope::Type::RPL_NAMREPLY:
            handle_RPL_NAMREPLY(event);
            break;

        case IRCEventEnvelope::Type::RPL_ENDOFNAMES:
            handle_RPL_ENDOFNAMES(event);
            break;

        case IRCEventEnvelope::Type::RPL_LINKS:
            handle_RPL_LINKS(event);
            break;

        case IRCEventEnvelope::Type::RPL_ENDOFLINKS:
            handle_RPL_ENDOFLINKS(event);
            break;

        case IRCEventEnvelope::Type::RPL_BANLIST:
            handle_RPL_BANLIST(event);
            break;

        case IRCEventEnvelope::Type::RPL_ENDOFBANLIST:
            handle_RPL_ENDOFBANLIST(event);
            break;

        case IRCEventEnvelope::Type::RPL_ENDOFWHOWAS:
            handle_RPL_ENDOFWHOWAS(event);
            break;

        case IRCEventEnvelope::Type::RPL_INFO:
            handle_RPL_INFO(event);
            break;

        case IRCEventEnvelope::Type::RPL_ENDOFINFO:
            handle_RPL_ENDOFINFO(event);
            break;

        case IRCEventEnvelope::Type::RPL_MOTDSTART:
            handle_RPL_MOTDSTART(event);
            break;

        case IRCEventEnvelope::Type::RPL_MOTD:
            handle_RPL_MOTD(event);
            break;

        case IRCEventEnvelope::Type::RPL_ENDOFMOTD:
            handle_RPL_ENDOFMOTD(event);
            break;

        case IRCEventEnvelope::Type::RPL_WHOISHOST:
            handle_RPL_WHOISHOST(event);
            break;

        case IRCEventEnvelope::Type::RPL_WHOISMODES:
            handle_RPL_WHOISMODES(event);
            break;

        case IRCEventEnvelope::Type::RPL_YOUREOPER:
            handle_RPL_YOUREOPER(event);
            break;

        case IRCEventEnvelope::Type::RPL_REHASHING:
            handle_RPL_REHASHING(event);
            break;

        case IRCEventEnvelope::Type::RPL_TIME:
            handle_RPL_TIME(event);
            break;

        case IRCEventEnvelope::Type::ERR_UNKNOWNERROR:
            handle_ERR_UNKNOWNERROR(event);
            break;

        case IRCEventEnvelope::Type::ERR_NOSUCHNICK:
            handle_ERR_NOSUCHNICK(event);
            break;

        case IRCEventEnvelope::Type::ERR_NOSUCHSERVER:
            handle_ERR_NOSUCHSERVER(event);
            break;

        case IRCEventEnvelope::Type::ERR_NOSUCHCHANNEL:
            handle_ERR_NOSUCHCHANNEL(event);
            break;

        case IRCEventEnvelope::Type::ERR_CANNOTSENDTOCHAN:
            handle_ERR_CANNOTSENDTOCHAN(event);
            break;

        case IRCEventEnvelope::Type::ERR_TOOMANYCHANNELS:
            handle_ERR_TOOMANYCHANNELS(event);
            break;

        case IRCEventEnvelope::Type::ERR_WASNOSUCHNICK:
            handle_ERR_WASNOSUCHNICK(event);
            break;

        case IRCEventEnvelope::Type::ERR_NOORIGIN:
            handle_ERR_NOORIGIN(event);
            break;

        case IRCEventEnvelope::Type::ERR_NORECIPIENT:
            handle_ERR_NORECIPIENT(event);
            break;

        case IRCEventEnvelope::Type::ERR_NOTEXTTOSEND:
            handle_ERR_NOTEXTTOSEND(event);
            break;

        case IRCEventEnvelope::Type::ERR_INPUTTOOLONG:
            handle_ERR_INPUTTOOLONG(event);
            break;

        case IRCEventEnvelope::Type::ERR_UNKNOWNCOMMAND:
            handle_ERR_UNKNOWNCOMMAND(event);
            break;

        case IRCEventEnvelope::Type::ERR_NOMOTD:
            handle_ERR_NOMOTD(event);
            break;

        case IRCEventEnvelope::Type::ERR_NONICKNAMEGIVEN:
            handle_ERR_NONICKNAMEGIVEN(event);
            break;

        case IRCEventEnvelope::Type::ERR_ERRONEUSNICKNAME:
            handle_ERR_ERRONEUSNICKNAME(event);
            break;

        case IRCEventEnvelope::Type::ERR_NICKNAMEINUSE:
            handle_ERR_NICKNAMEINUSE(event);
            break;

        case IRCEventEnvelope::Type::ERR_NICKCOLLISION:
            handle_ERR_NICKCOLLISION(event);
            break;

        case IRCEventEnvelope::Type::ERR_USERNOTINCHANNEL:
            handle_ERR_USERNOTINCHANNEL(event);
            break;

        case IRCEventEnvelope::Type::ERR_NOTONCHANNEL:
            handle_ERR_NOTONCHANNEL(event);
            break;

        case IRCEventEnvelope::Type::ERR_USERONCHANNEL:
            handle_ERR_USERONCHANNEL(event);
            break;

        case IRCEventEnvelope::Type::ERR_NOTREGISTERED:
            handle_ERR_NOTREGISTERED(event);
            break;

        case IRCEventEnvelope::Type::ERR_NEEDMOREPARAMS:
            handle_ERR_NEEDMOREPARAMS(event);
            break;

        case IRCEventEnvelope::Type::ERR_ALREADYREGISTERED:
            handle_ERR_ALREADYREGISTERED(event);
            break;

        case IRCEventEnvelope::Type::ERR_PASSWDMISMATCH:
            handle_ERR_PASSWDMISMATCH(event);
            break;

        case IRCEventEnvelope::Type::ERR_YOUREBANNEDCREEP:
            handle_ERR_YOUREBANNEDCREEP(event);
            break;

        case IRCEventEnvelope::Type::ERR_CHANNELISFULL:
            handle_ERR_CHANNELISFULL(event);
            break;

        case IRCEventEnvelope::Type::ERR_UNKNOWNMODE:
            handle_ERR_UNKNOWNMODE(event);
            break;

        case IRCEventEnvelope::Type::ERR_INVITEONLYCHAN:
            handle_ERR_INVITEONLYCHAN(event);
            break;

        case IRCEventEnvelope::Type::ERR_BANNEDFROMCHAN:
            handle_ERR_BANNEDFROMCHAN(event);
            break;

        case IRCEventEnvelope::Type::ERR_BADCHANNELKEY:
            handle_ERR_BADCHANNELKEY(event);
            break;

        case IRCEventEnvelope::Type::ERR_BADCHANMASK:
            handle_ERR_BADCHANMASK(event);
            break;

        case IRCEventEnvelope::Type::ERR_NOPRIVILEGES:
            handle_ERR_NOPRIVILEGES(event);
            break;

        case IRCEventEnvelope::Type::ERR_CHANOPRIVSNEEDED:
            handle_ERR_CHANOPRIVSNEEDED(event);
            break;

        case IRCEventEnvelope::Type::ERR_CANTKILLSERVER:
            handle_ERR_CANTKILLSERVER(event);
            break;

        case IRCEventEnvelope::Type::ERR_NOOPERHOST:
            handle_ERR_NOOPERHOST(event);
            break;

        case IRCEventEnvelope::Type::ERR_UMODEUNKNOWNFLAG:
            handle_ERR_UMODEUNKNOWNFLAG(event);
            break;

        case IRCEventEnvelope::Type::ERR_USERSDONTMATCH:
            handle_ERR_USERSDONTMATCH(event);
            break;

        case IRCEventEnvelope::Type::ERR_HELPNOTFOUND:
            handle_ERR_HELPNOTFOUND(event);
            break;

        case IRCEventEnvelope::Type::ERR_INVALIDKEY:
            handle_ERR_INVALIDKEY(event);
            break;

        case IRCEventEnvelope::Type::RPL_STARTTLS:
            handle_RPL_STARTTLS(event);
            break;

        case IRCEventEnvelope::Type::RPL_WHOISSECURE:
            handle_RPL_WHOISSECURE(event);
            break;

        case IRCEventEnvelope::Type::ERR_STARTTLS:
            handle_ERR_STARTTLS(event);
            break;

        case IRCEventEnvelope::Type::ERR_INVALIDMODEPARAM:
            handle_ERR_INVALIDMODEPARAM(event);
            break;

        case IRCEventEnvelope::Type::RPL_HELPSTART:
            handle_RPL_HELPSTART(event);
            break;

        case IRCEventEnvelope::Type::RPL_HELPTXT:
            handle_RPL_HELPTXT(event);
            break;

        case IRCEventEnvelope::Type::RPL_ENDOFHELP:
            handle_RPL_ENDOFHELP(event);
            break;

        case IRCEventEnvelope::Type::ERR_NOPRIVS:
            handle_ERR_NOPRIVS(event);
            break;

        case IRCEventEnvelope::Type::RPL_LOGGEDIN:
            handle_RPL_LOGGEDIN(event);
            break;

        case IRCEventEnvelope::Type::RPL_LOGGEDOUT:
            handle_RPL_LOGGEDOUT(event);
            break;

        case IRCEventEnvelope::Type::ERR_NICKLOCKED:
            handle_ERR_NICKLOCKED(event);
            break;

        case IRCEventEnvelope::Type::RPL_SASLSUCCESS:
            handle_RPL_SASLSUCCESS(event);
            break;

        case IRCEventEnvelope::Type::ERR_SASLFAIL:
            handle_ERR_SASLFAIL(event);
            break;

        case IRCEventEnvelope::Type::ERR_SASLTOOLONG:
            handle_ERR_SASLTOOLONG(event);
            break;

        case IRCEventEnvelope::Type::ERR_SASLABORTED:
            handle_ERR_SASLABORTED(event);
            break;

        case IRCEventEnvelope::Type::ERR_SASLALREADY:
            handle_ERR_SASLALREADY(event);
            break;

        case IRCEventEnvelope::Type::RPL_SASLMECHS:
            handle_RPL_SASLMECHS(event);
            break;

        case IRCEventEnvelope::Type::RPL_CAP:
            // this will never happen if the parser is doing its job
            // so treat it like a parser error
            handle_S_RPL_CAP(event);
            break;

        case IRCEventEnvelope::Type::S_RPL_CAP_LS:
            handle_S_RPL_CAP_LS(event);
            break;

        case IRCEventEnvelope::Type::S_RPL_CAP_ACK:
            handle_S_RPL_CAP_ACK(event);
            break;

        case IRCEventEnvelope::Type::AUTHENTICATE:
            handle_AUTHENTICATE(event);
            break;

        default:
            handle_unassociated_event(event);
            break;
    }
}
