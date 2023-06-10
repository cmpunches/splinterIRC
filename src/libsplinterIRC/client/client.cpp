#include "client.h"

splinterClient::splinterClient(const std::string& server, const std::string& port, const std::string& nick, const std::string& password ) :
        server_(server), port_(port), nick_(nick), password_(password), socket_(-1)
        {
    this->connect();
    this->run();
}

splinterClient::~splinterClient()
{
    if (socket_ != -1)
    {
        close(socket_);
    }
}

void splinterClient::connect()
{
    struct addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo* result;
    if (getaddrinfo(server_.c_str(), port_.c_str(), &hints, &result) != 0)
    {
        std::cerr << "Failed to resolve server address:  " << server_ << std::endl;
        critical_thread_failed = true;
        return;
    }

    for (struct addrinfo* rp = result; rp != nullptr; rp = rp->ai_next)
    {
        socket_ = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (socket_ == -1)
        {
            continue;
        }
        if (::connect(socket_, rp->ai_addr, rp->ai_addrlen) != -1)
        {
            break;
        }
        close(socket_);
        socket_ = -1;
    }

    freeaddrinfo(result);
    if (socket_ == -1) {
        std::cerr << "Failed to connect to server" << std::endl;
        critical_thread_failed = true;
        return;
    }

    set_nick( nick_ );
    set_user( nick_ );
}

void splinterClient::decision_loop(const IRCEvent& event)
{
    // type mappings for actions and events
    switch ( event.type() )
    {
        case IRCEvent::Type::UNKNOWN:
            handle_UNKNOWN(event);
            break;

        case IRCEvent::Type::JOIN:
            handle_JOIN(event);
            break;

        case IRCEvent::Type::PART:
            handle_PART(event);
            break;

        case IRCEvent::Type::KICK:
            handle_KICK(event);
            break;

        case IRCEvent::Type::NOTICE:
            handle_NOTICE(event);
            break;

        case IRCEvent::Type::PING:
            handle_PING(event);
            break;

        case IRCEvent::Type::QUIT:
            handle_QUIT(event);
            break;

        case IRCEvent::Type::NICK:
            handle_NICK(event);
            break;

        case IRCEvent::Type::TOPIC:
            handle_TOPIC(event);
            break;

        case IRCEvent::Type::INVITE:
            handle_INVITE(event);
            break;

        case IRCEvent::Type::ERROR:
            handle_ERROR(event);
            break;

        case IRCEvent::Type::S_PRIVATE_MESSAGE:
            handle_S_PRIVATE_MESSAGE(event);
            break;

        case IRCEvent::Type::S_CHANNEL_MESSAGE:
            handle_S_CHANNEL_MESSAGE(event);
            break;

        case IRCEvent::Type::RPL_WELCOME:
            handle_RPL_WELCOME(event);
            break;

        case IRCEvent::Type::RPL_YOURHOST:
            handle_RPL_YOURHOST(event);
            break;

        case IRCEvent::Type::RPL_CREATED:
            handle_RPL_CREATED(event);
            break;

        case IRCEvent::Type::RPL_MYINFO:
            handle_RPL_MYINFO(event);
            break;

        case IRCEvent::Type::RPL_ISUPPORT:
            handle_RPL_ISUPPORT(event);
            break;

        case IRCEvent::Type::RPL_BOUNCE:
            handle_RPL_BOUNCE(event);
            break;

        case IRCEvent::Type::RPL_STATSCOMMANDS:
            handle_RPL_STATSCOMMANDS(event);
            break;

        case IRCEvent::Type::RPL_ENDOFSTATS:
            handle_RPL_ENDOFSTATS(event);
            break;

        case IRCEvent::Type::RPL_STATSUPTIME:
            handle_RPL_STATSUPTIME(event);
            break;

        case IRCEvent::Type::RPL_UMODEIS:
            handle_RPL_UMODEIS(event);
            break;

        case IRCEvent::Type::RPL_LUSERCLIENT:
            handle_RPL_LUSERCLIENT(event);
            break;

        case IRCEvent::Type::RPL_LUSEROP:
            handle_RPL_LUSEROP(event);
            break;

        case IRCEvent::Type::RPL_LUSERUNKNOWN:
            handle_RPL_LUSERUNKNOWN(event);
            break;

        case IRCEvent::Type::RPL_LUSERCHANNELS:
            handle_RPL_LUSERCHANNELS(event);
            break;

        case IRCEvent::Type::RPL_LUSERME:
            handle_RPL_LUSERME(event);
            break;

        case IRCEvent::Type::RPL_ADMINME:
            handle_RPL_ADMINME(event);
            break;

        case IRCEvent::Type::RPL_ADMINLOC1:
            handle_RPL_ADMINLOC1(event);
            break;

        case IRCEvent::Type::RPL_ADMINLOC2:
            handle_RPL_ADMINLOC2(event);
            break;

        case IRCEvent::Type::RPL_ADMINEMAIL:
            handle_RPL_ADMINEMAIL(event);
            break;

        case IRCEvent::Type::RPL_TRYAGAIN:
            handle_RPL_TRYAGAIN(event);
            break;

        case IRCEvent::Type::RPL_LOCALUSERS:
            handle_RPL_LOCALUSERS(event);
            break;

        case IRCEvent::Type::RPL_GLOBALUSERS:
            handle_RPL_GLOBALUSERS(event);
            break;

        case IRCEvent::Type::RPL_WHOISCERTFP:
            handle_RPL_WHOISCERTFP(event);
            break;

        case IRCEvent::Type::RPL_NONE:
            handle_RPL_NONE(event);
            break;

        case IRCEvent::Type::RPL_AWAY:
            handle_RPL_AWAY(event);
            break;

        case IRCEvent::Type::RPL_USERHOST:
            handle_RPL_USERHOST(event);
            break;

        case IRCEvent::Type::RPL_UNAWAY:
            handle_RPL_UNAWAY(event);
            break;

        case IRCEvent::Type::RPL_NOWAWAY:
            handle_RPL_NOWAWAY(event);
            break;

        case IRCEvent::Type::RPL_WHOREPLY:
            handle_RPL_WHOREPLY(event);
            break;

        case IRCEvent::Type::RPL_ENDOFWHO:
            handle_RPL_ENDOFWHO(event);
            break;

        case IRCEvent::Type::RPL_WHOISREGNICK:
            handle_RPL_WHOISREGNICK(event);
            break;

        case IRCEvent::Type::RPL_WHOISUSER:
            handle_RPL_WHOISUSER(event);
            break;

        case IRCEvent::Type::RPL_WHOISSERVER:
            handle_RPL_WHOISSERVER(event);
            break;

        case IRCEvent::Type::RPL_WHOISOPERATOR:
            handle_RPL_WHOISOPERATOR(event);
            break;

        case IRCEvent::Type::RPL_WHOWASUSER:
            handle_RPL_WHOWASUSER(event);
            break;

        case IRCEvent::Type::RPL_WHOISIDLE:
            handle_RPL_WHOISIDLE(event);
            break;

        case IRCEvent::Type::RPL_ENDOFWHOIS:
            handle_RPL_ENDOFWHOIS(event);
            break;

        case IRCEvent::Type::RPL_WHOISCHANNELS:
            handle_RPL_WHOISCHANNELS(event);
            break;

        case IRCEvent::Type::RPL_WHOISSPECIAL:
            handle_RPL_WHOISSPECIAL(event);
            break;

        case IRCEvent::Type::RPL_LISTSTART:
            handle_RPL_LISTSTART(event);
            break;

        case IRCEvent::Type::RPL_LIST:
            handle_RPL_LIST(event);
            break;

        case IRCEvent::Type::RPL_LISTEND:
            handle_RPL_LISTEND(event);
            break;

        case IRCEvent::Type::RPL_CHANNELMODEIS:
            handle_RPL_CHANNELMODEIS(event);
            break;

        case IRCEvent::Type::RPL_CREATIONTIME:
            handle_RPL_CREATIONTIME(event);
            break;

        case IRCEvent::Type::RPL_WHOISACCOUNT:
            handle_RPL_WHOISACCOUNT(event);
            break;

        case IRCEvent::Type::RPL_NOTOPIC:
            handle_RPL_NOTOPIC(event);
            break;

        case IRCEvent::Type::RPL_TOPIC:
            handle_RPL_TOPIC(event);
            break;

        case IRCEvent::Type::RPL_TOPICWHOTIME:
            handle_RPL_TOPICWHOTIME(event);
            break;

        case IRCEvent::Type::RPL_INVITELIST:
            handle_RPL_INVITELIST(event);
            break;

        case IRCEvent::Type::RPL_ENDOFINVITELIST:
            handle_RPL_ENDOFINVITELIST(event);
            break;

        case IRCEvent::Type::RPL_WHOISACTUALLY:
            handle_RPL_WHOISACTUALLY(event);
            break;

        case IRCEvent::Type::RPL_INVITING:
            handle_RPL_INVITING(event);
            break;

        case IRCEvent::Type::RPL_INVEXLIST:
            handle_RPL_INVEXLIST(event);
            break;

        case IRCEvent::Type::RPL_ENDOFINVEXLIST:
            handle_RPL_ENDOFINVEXLIST(event);
            break;

        case IRCEvent::Type::RPL_EXCEPTLIST:
            handle_RPL_EXCEPTLIST(event);
            break;

        case IRCEvent::Type::RPL_ENDOFEXCEPTLIST:
            handle_RPL_ENDOFEXCEPTLIST(event);
            break;

        case IRCEvent::Type::RPL_VERSION:
            handle_RPL_VERSION(event);
            break;

        case IRCEvent::Type::RPL_NAMREPLY:
            handle_RPL_NAMREPLY(event);
            break;

        case IRCEvent::Type::RPL_ENDOFNAMES:
            handle_RPL_ENDOFNAMES(event);
            break;

        case IRCEvent::Type::RPL_LINKS:
            handle_RPL_LINKS(event);
            break;

        case IRCEvent::Type::RPL_ENDOFLINKS:
            handle_RPL_ENDOFLINKS(event);
            break;

        case IRCEvent::Type::RPL_BANLIST:
            handle_RPL_BANLIST(event);
            break;

        case IRCEvent::Type::RPL_ENDOFBANLIST:
            handle_RPL_ENDOFBANLIST(event);
            break;

        case IRCEvent::Type::RPL_ENDOFWHOWAS:
            handle_RPL_ENDOFWHOWAS(event);
            break;

        case IRCEvent::Type::RPL_INFO:
            handle_RPL_INFO(event);
            break;

        case IRCEvent::Type::RPL_ENDOFINFO:
            handle_RPL_ENDOFINFO(event);
            break;

        case IRCEvent::Type::RPL_MOTDSTART:
            handle_RPL_MOTDSTART(event);
            break;

        case IRCEvent::Type::RPL_MOTD:
            handle_RPL_MOTD(event);
            break;

        case IRCEvent::Type::RPL_ENDOFMOTD:
            handle_RPL_ENDOFMOTD(event);
            break;

        case IRCEvent::Type::RPL_WHOISHOST:
            handle_RPL_WHOISHOST(event);
            break;

        case IRCEvent::Type::RPL_WHOISMODES:
            handle_RPL_WHOISMODES(event);
            break;

        case IRCEvent::Type::RPL_YOUREOPER:
            handle_RPL_YOUREOPER(event);
            break;

        case IRCEvent::Type::RPL_REHASHING:
            handle_RPL_REHASHING(event);
            break;

        case IRCEvent::Type::RPL_TIME:
            handle_RPL_TIME(event);
            break;

        case IRCEvent::Type::ERR_UNKNOWNERROR:
            handle_ERR_UNKNOWNERROR(event);
            break;

        case IRCEvent::Type::ERR_NOSUCHNICK:
            handle_ERR_NOSUCHNICK(event);
            break;

        case IRCEvent::Type::ERR_NOSUCHSERVER:
            handle_ERR_NOSUCHSERVER(event);
            break;

        case IRCEvent::Type::ERR_NOSUCHCHANNEL:
            handle_ERR_NOSUCHCHANNEL(event);
            break;

        case IRCEvent::Type::ERR_CANNOTSENDTOCHAN:
            handle_ERR_CANNOTSENDTOCHAN(event);
            break;

        case IRCEvent::Type::ERR_TOOMANYCHANNELS:
            handle_ERR_TOOMANYCHANNELS(event);
            break;

        case IRCEvent::Type::ERR_WASNOSUCHNICK:
            handle_ERR_WASNOSUCHNICK(event);
            break;

        case IRCEvent::Type::ERR_NOORIGIN:
            handle_ERR_NOORIGIN(event);
            break;

        case IRCEvent::Type::ERR_NORECIPIENT:
            handle_ERR_NORECIPIENT(event);
            break;

        case IRCEvent::Type::ERR_NOTEXTTOSEND:
            handle_ERR_NOTEXTTOSEND(event);
            break;

        case IRCEvent::Type::ERR_INPUTTOOLONG:
            handle_ERR_INPUTTOOLONG(event);
            break;

        case IRCEvent::Type::ERR_UNKNOWNCOMMAND:
            handle_ERR_UNKNOWNCOMMAND(event);
            break;

        case IRCEvent::Type::ERR_NOMOTD:
            handle_ERR_NOMOTD(event);
            break;

        case IRCEvent::Type::ERR_NONICKNAMEGIVEN:
            handle_ERR_NONICKNAMEGIVEN(event);
            break;

        case IRCEvent::Type::ERR_ERRONEUSNICKNAME:
            handle_ERR_ERRONEUSNICKNAME(event);
            break;

        case IRCEvent::Type::ERR_NICKNAMEINUSE:
            handle_ERR_NICKNAMEINUSE(event);
            break;

        case IRCEvent::Type::ERR_NICKCOLLISION:
            handle_ERR_NICKCOLLISION(event);
            break;

        case IRCEvent::Type::ERR_USERNOTINCHANNEL:
            handle_ERR_USERNOTINCHANNEL(event);
            break;

        case IRCEvent::Type::ERR_NOTONCHANNEL:
            handle_ERR_NOTONCHANNEL(event);
            break;

        case IRCEvent::Type::ERR_USERONCHANNEL:
            handle_ERR_USERONCHANNEL(event);
            break;

        case IRCEvent::Type::ERR_NOTREGISTERED:
            handle_ERR_NOTREGISTERED(event);
            break;

        case IRCEvent::Type::ERR_NEEDMOREPARAMS:
            handle_ERR_NEEDMOREPARAMS(event);
            break;

        case IRCEvent::Type::ERR_ALREADYREGISTERED:
            handle_ERR_ALREADYREGISTERED(event);
            break;

        case IRCEvent::Type::ERR_PASSWDMISMATCH:
            handle_ERR_PASSWDMISMATCH(event);
            break;

        case IRCEvent::Type::ERR_YOUREBANNEDCREEP:
            handle_ERR_YOUREBANNEDCREEP(event);
            break;

        case IRCEvent::Type::ERR_CHANNELISFULL:
            handle_ERR_CHANNELISFULL(event);
            break;

        case IRCEvent::Type::ERR_UNKNOWNMODE:
            handle_ERR_UNKNOWNMODE(event);
            break;

        case IRCEvent::Type::ERR_INVITEONLYCHAN:
            handle_ERR_INVITEONLYCHAN(event);
            break;

        case IRCEvent::Type::ERR_BANNEDFROMCHAN:
            handle_ERR_BANNEDFROMCHAN(event);
            break;

        case IRCEvent::Type::ERR_BADCHANNELKEY:
            handle_ERR_BADCHANNELKEY(event);
            break;

        case IRCEvent::Type::ERR_BADCHANMASK:
            handle_ERR_BADCHANMASK(event);
            break;

        case IRCEvent::Type::ERR_NOPRIVILEGES:
            handle_ERR_NOPRIVILEGES(event);
            break;

        case IRCEvent::Type::ERR_CHANOPRIVSNEEDED:
            handle_ERR_CHANOPRIVSNEEDED(event);
            break;

        case IRCEvent::Type::ERR_CANTKILLSERVER:
            handle_ERR_CANTKILLSERVER(event);
            break;

        case IRCEvent::Type::ERR_NOOPERHOST:
            handle_ERR_NOOPERHOST(event);
            break;

        case IRCEvent::Type::ERR_UMODEUNKNOWNFLAG:
            handle_ERR_UMODEUNKNOWNFLAG(event);
            break;

        case IRCEvent::Type::ERR_USERSDONTMATCH:
            handle_ERR_USERSDONTMATCH(event);
            break;

        case IRCEvent::Type::ERR_HELPNOTFOUND:
            handle_ERR_HELPNOTFOUND(event);
            break;

        case IRCEvent::Type::ERR_INVALIDKEY:
            handle_ERR_INVALIDKEY(event);
            break;

        case IRCEvent::Type::RPL_STARTTLS:
            handle_RPL_STARTTLS(event);
            break;

        case IRCEvent::Type::RPL_WHOISSECURE:
            handle_RPL_WHOISSECURE(event);
            break;

        case IRCEvent::Type::ERR_STARTTLS:
            handle_ERR_STARTTLS(event);
            break;

        case IRCEvent::Type::ERR_INVALIDMODEPARAM:
            handle_ERR_INVALIDMODEPARAM(event);
            break;

        case IRCEvent::Type::RPL_HELPSTART:
            handle_RPL_HELPSTART(event);
            break;

        case IRCEvent::Type::RPL_HELPTXT:
            handle_RPL_HELPTXT(event);
            break;

        case IRCEvent::Type::RPL_ENDOFHELP:
            handle_RPL_ENDOFHELP(event);
            break;

        case IRCEvent::Type::ERR_NOPRIVS:
            handle_ERR_NOPRIVS(event);
            break;

        case IRCEvent::Type::RPL_LOGGEDIN:
            handle_RPL_LOGGEDIN(event);
            break;

        case IRCEvent::Type::RPL_LOGGEDOUT:
            handle_RPL_LOGGEDOUT(event);
            break;

        case IRCEvent::Type::ERR_NICKLOCKED:
            handle_ERR_NICKLOCKED(event);
            break;

        case IRCEvent::Type::RPL_SASLSUCCESS:
            handle_RPL_SASLSUCCESS(event);
            break;

        case IRCEvent::Type::ERR_SASLFAIL:
            handle_ERR_SASLFAIL(event);
            break;

        case IRCEvent::Type::ERR_SASLTOOLONG:
            handle_ERR_SASLTOOLONG(event);
            break;

        case IRCEvent::Type::ERR_SASLABORTED:
            handle_ERR_SASLABORTED(event);
            break;

        case IRCEvent::Type::ERR_SASLALREADY:
            handle_ERR_SASLALREADY(event);
            break;

        case IRCEvent::Type::RPL_SASLMECHS:
            handle_RPL_SASLMECHS(event);
            break;

        default:
            handle_unassociated_event(event);
            break;
    }
}

void splinterClient::send( const std::string& message )
{
    if (::send(socket_, message.c_str(), message.size(), 0) == -1)
    {
        std::cerr << "Failed to send message to server" << std::endl;
        return;
    }
}

const std::string& splinterClient::get_server() const
{
    return server_;
}

