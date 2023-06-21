#include "IRCEventv3.h"

IRCEventEnvelope::Type IRCEventEnvelope::verb_to_type(const std::string& token ) const
{
    if (token == "JOIN") { return Type::JOIN;
    } else if (token == "PART") { return Type::PART;
    } else if (token == "KICK") { return Type::KICK;
    } else if (token == "PRIVMSG") { return Type::PRIVMSG;
    } else if (token == "NOTICE") { return Type::NOTICE;
    } else if (token == "PING") { return Type::PING;
    } else if (token == "NICK" ) { return Type::NICK;
    } else if (token == "MODE" ) { return Type::RPL_UMODEIS;
    } else if (token == "QUIT" ) { return Type::QUIT;
    } else if (token == "TOPIC" ) { return Type::TOPIC;
    } else if (token == "INVITE" ) { return Type::INVITE;
    } else if (token == "ERROR" ) { return Type::ERROR;
    } else if (token == "CAP" ) { return Type::S_RPL_CAP;
    } else if (token == "S_RPL_CAP_LS" ) { return Type::S_RPL_CAP_LS;
    } else if (token == "S_RPL_CAP_ACK" ) { return Type::S_RPL_CAP_ACK;
    } else if (token == "S_RPL_CAP_UNKNOWN" ) { return Type::S_RPL_CAP_UNKNOWN;
    } else if (token == "S_RPL_CAP_NAK" ) { return Type::S_RPL_CAP_NAK;
    } else if ( token == "001" ) { return Type::RPL_WELCOME;
    } else if ( token == "002" ) { return Type::RPL_YOURHOST;
    } else if ( token == "003" ) { return Type::RPL_CREATED;
    } else if ( token == "004" ) { return Type::RPL_MYINFO;
    } else if ( token == "005" ) { return Type::RPL_ISUPPORT;
    } else if ( token == "010" ) { return Type::RPL_BOUNCE;
    } else if ( token == "212" ) { return Type::RPL_STATSCOMMANDS;
    } else if ( token == "219" ) { return Type::RPL_ENDOFSTATS;
    } else if ( token == "242" ) { return Type::RPL_STATSUPTIME;
    } else if ( token == "221" ) { return Type::RPL_UMODEIS;
    } else if ( token == "251" ) { return Type::RPL_LUSERCLIENT;
    } else if ( token == "252" ) { return Type::RPL_LUSEROP;
    } else if ( token == "253" ) { return Type::RPL_LUSERUNKNOWN;
    } else if ( token == "254" ) { return Type::RPL_LUSERCHANNELS;
    } else if ( token == "255" ) { return Type::RPL_LUSERME;
    } else if ( token == "256" ) { return Type::RPL_ADMINME;
    } else if ( token == "257" ) { return Type::RPL_ADMINLOC1;
    } else if ( token == "258" ) { return Type::RPL_ADMINLOC2;
    } else if ( token == "259" ) { return Type::RPL_ADMINEMAIL;
    } else if ( token == "263" ) { return Type::RPL_TRYAGAIN;
    } else if ( token == "265" ) { return Type::RPL_LOCALUSERS;
    } else if ( token == "266" ) { return Type::RPL_GLOBALUSERS;
    } else if ( token == "276" ) { return Type::RPL_WHOISCERTFP;
    } else if ( token == "300" ) { return Type::RPL_NONE;
    } else if ( token == "301" ) { return Type::RPL_AWAY;
    } else if ( token == "302" ) { return Type::RPL_USERHOST;
    } else if ( token == "305" ) { return Type::RPL_UNAWAY;
    } else if ( token == "306" ) { return Type::RPL_NOWAWAY;
    } else if ( token == "352" ) { return Type::RPL_WHOREPLY;
    } else if ( token == "315" ) { return Type::RPL_ENDOFWHO;
    } else if ( token == "307" ) { return Type::RPL_WHOISREGNICK;
    } else if ( token == "311" ) { return Type::RPL_WHOISUSER;
    } else if ( token == "312" ) { return Type::RPL_WHOISSERVER;
    } else if ( token == "313" ) { return Type::RPL_WHOISOPERATOR;
    } else if ( token == "314" ) { return Type::RPL_WHOWASUSER;
    } else if ( token == "317" ) { return Type::RPL_WHOISIDLE;
    } else if ( token == "318" ) { return Type::RPL_ENDOFWHOIS;
    } else if ( token == "319" ) { return Type::RPL_WHOISCHANNELS;
    } else if ( token == "320" ) { return Type::RPL_WHOISSPECIAL;
    } else if ( token == "321" ) { return Type::RPL_LISTSTART;
    } else if ( token == "322" ) { return Type::RPL_LIST;
    } else if ( token == "323" ) { return Type::RPL_LISTEND;
    } else if ( token == "324" ) { return Type::RPL_CHANNELMODEIS;
    } else if ( token == "329" ) { return Type::RPL_CREATIONTIME;
    } else if ( token == "330" ) { return Type::RPL_WHOISACCOUNT;
    } else if ( token == "331" ) { return Type::RPL_NOTOPIC;
    } else if ( token == "332" ) { return Type::RPL_TOPIC;
    } else if ( token == "333" ) { return Type::RPL_TOPICWHOTIME;
    } else if ( token == "336" ) { return Type::RPL_INVITELIST;
    } else if ( token == "337" ) { return Type::RPL_ENDOFINVITELIST;
    } else if ( token == "338" ) { return Type::RPL_WHOISACTUALLY;
    } else if ( token == "341" ) { return Type::RPL_INVITING;
    } else if ( token == "346" ) { return Type::RPL_INVEXLIST;
    } else if ( token == "347" ) { return Type::RPL_ENDOFINVEXLIST;
    } else if ( token == "348" ) { return Type::RPL_EXCEPTLIST;
    } else if ( token == "349" ) { return Type::RPL_ENDOFEXCEPTLIST;
    } else if ( token == "351" ) { return Type::RPL_VERSION;
    } else if ( token == "353" ) { return Type::RPL_NAMREPLY;
    } else if ( token == "366" ) { return Type::RPL_ENDOFNAMES;
    } else if ( token == "364" ) { return Type::RPL_LINKS;
    } else if ( token == "365" ) { return Type::RPL_ENDOFLINKS;
    } else if ( token == "367" ) { return Type::RPL_BANLIST;
    } else if ( token == "368" ) { return Type::RPL_ENDOFBANLIST;
    } else if ( token == "369" ) { return Type::RPL_ENDOFWHOWAS;
    } else if ( token == "371" ) { return Type::RPL_INFO;
    } else if ( token == "374" ) { return Type::RPL_ENDOFINFO;
    } else if ( token == "375" ) { return Type::RPL_MOTDSTART;
    } else if ( token == "372" ) { return Type::RPL_MOTD;
    } else if ( token == "376" ) { return Type::RPL_ENDOFMOTD;
    } else if ( token == "378" ) { return Type::RPL_WHOISHOST;
    } else if ( token == "379" ) { return Type::RPL_WHOISMODES;
    } else if ( token == "381" ) { return Type::RPL_YOUREOPER;
    } else if ( token == "382" ) { return Type::RPL_REHASHING;
    } else if ( token == "391" ) { return Type::RPL_TIME;
    } else if ( token == "400" ) { return Type::ERR_UNKNOWNERROR;
    } else if ( token == "401" ) { return Type::ERR_NOSUCHNICK;
    } else if ( token == "402" ) { return Type::ERR_NOSUCHSERVER;
    } else if ( token == "403" ) { return Type::ERR_NOSUCHCHANNEL;
    } else if ( token == "404" ) { return Type::ERR_CANNOTSENDTOCHAN;
    } else if ( token == "405" ) { return Type::ERR_TOOMANYCHANNELS;
    } else if ( token == "406" ) { return Type::ERR_WASNOSUCHNICK;
    } else if ( token == "409" ) { return Type::ERR_NOORIGIN;
    } else if ( token == "411" ) { return Type::ERR_NORECIPIENT;
    } else if ( token == "412" ) { return Type::ERR_NOTEXTTOSEND;
    } else if ( token == "417" ) { return Type::ERR_INPUTTOOLONG;
    } else if ( token == "421" ) { return Type::ERR_UNKNOWNCOMMAND;
    } else if ( token == "422" ) { return Type::ERR_NOMOTD;
    } else if ( token == "431" ) { return Type::ERR_NONICKNAMEGIVEN;
    } else if ( token == "432" ) { return Type::ERR_ERRONEUSNICKNAME;
    } else if ( token == "433" ) { return Type::ERR_NICKNAMEINUSE;
    } else if ( token == "436" ) { return Type::ERR_NICKCOLLISION;
    } else if ( token == "441" ) { return Type::ERR_USERNOTINCHANNEL;
    } else if ( token == "442" ) { return Type::ERR_NOTONCHANNEL;
    } else if ( token == "443" ) { return Type::ERR_USERONCHANNEL;
    } else if ( token == "451" ) { return Type::ERR_NOTREGISTERED;
    } else if ( token == "461" ) { return Type::ERR_NEEDMOREPARAMS;
    } else if ( token == "462" ) { return Type::ERR_ALREADYREGISTERED;
    } else if ( token == "464" ) { return Type::ERR_PASSWDMISMATCH;
    } else if ( token == "465" ) { return Type::ERR_YOUREBANNEDCREEP;
    } else if ( token == "471" ) { return Type::ERR_CHANNELISFULL;
    } else if ( token == "472" ) { return Type::ERR_UNKNOWNMODE;
    } else if ( token == "473" ) { return Type::ERR_INVITEONLYCHAN;
    } else if ( token == "474" ) { return Type::ERR_BANNEDFROMCHAN;
    } else if ( token == "475" ) { return Type::ERR_BADCHANNELKEY;
    } else if ( token == "476" ) { return Type::ERR_BADCHANMASK;
    } else if ( token == "481" ) { return Type::ERR_NOPRIVILEGES;
    } else if ( token == "482" ) { return Type::ERR_CHANOPRIVSNEEDED;
    } else if ( token == "483" ) { return Type::ERR_CANTKILLSERVER;
    } else if ( token == "491" ) { return Type::ERR_NOOPERHOST;
    } else if ( token == "501" ) { return Type::ERR_UMODEUNKNOWNFLAG;
    } else if ( token == "502" ) { return Type::ERR_USERSDONTMATCH;
    } else if ( token == "524" ) { return Type::ERR_HELPNOTFOUND;
    } else if ( token == "525" ) { return Type::ERR_INVALIDKEY;
    } else if ( token == "670" ) { return Type::RPL_STARTTLS;
    } else if ( token == "671" ) { return Type::RPL_WHOISSECURE;
    } else if ( token == "691" ) { return Type::ERR_STARTTLS;
    } else if ( token == "696" ) { return Type::ERR_INVALIDMODEPARAM;
    } else if ( token == "704" ) { return Type::RPL_HELPSTART;
    } else if ( token == "705" ) { return Type::RPL_HELPTXT;
    } else if ( token == "706" ) { return Type::RPL_ENDOFHELP;
    } else if ( token == "723" ) { return Type::ERR_NOPRIVS;
    } else if ( token == "900" ) { return Type::RPL_LOGGEDIN;
    } else if ( token == "901" ) { return Type::RPL_LOGGEDOUT;
    } else if ( token == "902" ) { return Type::ERR_NICKLOCKED;
    } else if ( token == "903" ) { return Type::RPL_SASLSUCCESS;
    } else if ( token == "904" ) { return Type::ERR_SASLFAIL;
    } else if ( token == "905" ) { return Type::ERR_SASLTOOLONG;
    } else if ( token == "906" ) { return Type::ERR_SASLABORTED;
    } else if ( token == "907" ) { return Type::ERR_SASLALREADY;
    } else if ( token == "908" ) { return Type::RPL_SASLMECHS;
    } else if ( token == "AUTHENTICATE" ) { return Type::AUTHENTICATE;
    }
    return Type::UNKNOWN;
}

// command mappings for string representation of types
std::string IRCEventEnvelope::type_to_verb(Type type) const {
    switch (type) {
        // It's pretty stupid that C++ doesn't have a way to
        // convert enum values to strings without this.
        case Type::UNKNOWN: return "UNKNOWN";
        case Type::JOIN: return "JOIN";
        case Type::PART: return "PART";
        case Type::KICK: return "KICK";
        case Type::PRIVMSG: return "PRIVMSG";
        case Type::S_RPL_CAP: return "S_RPL_CAP";
        case Type::NOTICE: return "NOTICE";
        case Type::PING: return "PING";
        case Type::QUIT: return "QUIT";
        case Type::NICK: return "NICK";
        case Type::TOPIC: return "TOPIC";
        case Type::INVITE: return "INVITE";
        case Type::ERROR: return "ERROR";
        case Type::S_PRIVATE_MESSAGE: return "S_PRIVATE_MESSAGE";
        case Type::S_CHANNEL_MESSAGE: return "S_CHANNEL_MESSAGE";
        case Type::S_RPL_CAP_LS: return "S_RPL_CAP_LS";
        case Type::S_RPL_CAP_ACK: return "S_RPL_CAP_ACK";
        case Type::S_RPL_CAP_NAK: return "S_RPL_CAP_NAK";
        case Type::S_RPL_CAP_UNKNOWN: return "S_RPL_CAP_UNKNOWN";
        case Type::RPL_WELCOME: return "RPL_WELCOME";
        case Type::RPL_YOURHOST: return "RPL_YOURHOST";
        case Type::RPL_CREATED: return "RPL_CREATED";
        case Type::RPL_MYINFO: return "RPL_MYINFO";
        case Type::RPL_ISUPPORT: return "RPL_ISUPPORT";
        case Type::RPL_BOUNCE: return "RPL_BOUNCE";
        case Type::RPL_STATSCOMMANDS: return "RPL_STATSCOMMANDS";
        case Type::RPL_ENDOFSTATS: return "RPL_ENDOFSTATS";
        case Type::RPL_STATSUPTIME: return "RPL_STATSUPTIME";
        case Type::RPL_UMODEIS: return "RPL_UMODEIS";
        case Type::RPL_LUSERCLIENT: return "RPL_LUSERCLIENT";
        case Type::RPL_LUSEROP: return "RPL_LUSEROP";
        case Type::RPL_LUSERUNKNOWN: return "RPL_LUSERUNKNOWN";
        case Type::RPL_LUSERCHANNELS: return "RPL_LUSERCHANNELS";
        case Type::RPL_LUSERME: return "RPL_LUSERME";
        case Type::RPL_ADMINME: return "RPL_ADMINME";
        case Type::RPL_ADMINLOC1: return "RPL_ADMINLOC1";
        case Type::RPL_ADMINLOC2: return "RPL_ADMINLOC2";
        case Type::RPL_ADMINEMAIL: return "RPL_ADMINEMAIL";
        case Type::RPL_TRYAGAIN: return "RPL_TRYAGAIN";
        case Type::RPL_LOCALUSERS: return "RPL_LOCALUSERS";
        case Type::RPL_GLOBALUSERS: return "RPL_GLOBALUSERS";
        case Type::RPL_WHOISCERTFP: return "RPL_WHOISCERTFP";
        case Type::RPL_NONE: return "RPL_NONE";
        case Type::RPL_AWAY: return "RPL_AWAY";
        case Type::RPL_USERHOST: return "RPL_USERHOST";
        case Type::RPL_UNAWAY: return "RPL_UNAWAY";
        case Type::RPL_NOWAWAY: return "RPL_NOWAWAY";
        case Type::RPL_WHOREPLY: return "RPL_WHOREPLY";
        case Type::RPL_ENDOFWHO: return "RPL_ENDOFWHO";
        case Type::RPL_WHOISREGNICK: return "RPL_WHOISREGNICK";
        case Type::RPL_WHOISUSER: return "RPL_WHOISUSER";
        case Type::RPL_WHOISSERVER: return "RPL_WHOISSERVER";
        case Type::RPL_WHOISOPERATOR: return "RPL_WHOISOPERATOR";
        case Type::RPL_WHOWASUSER: return "RPL_WHOWASUSER";
        case Type::RPL_WHOISIDLE: return "RPL_WHOISIDLE";
        case Type::RPL_ENDOFWHOIS: return "RPL_ENDOFWHOIS";
        case Type::RPL_WHOISCHANNELS: return "RPL_WHOISCHANNELS";
        case Type::RPL_WHOISSPECIAL: return "RPL_WHOISSPECIAL";
        case Type::RPL_LISTSTART: return "RPL_LISTSTART";
        case Type::RPL_LIST: return "RPL_LIST";
        case Type::RPL_LISTEND: return "RPL_LISTEND";
        case Type::RPL_CHANNELMODEIS: return "RPL_CHANNELMODEIS";
        case Type::RPL_CREATIONTIME: return "RPL_CREATIONTIME";
        case Type::RPL_WHOISACCOUNT: return "RPL_WHOISACCOUNT";
        case Type::RPL_NOTOPIC: return "RPL_NOTOPIC";
        case Type::RPL_TOPIC: return "RPL_TOPIC";
        case Type::RPL_TOPICWHOTIME: return "RPL_TOPICWHOTIME";
        case Type::RPL_INVITELIST: return "RPL_INVITELIST";
        case Type::RPL_ENDOFINVITELIST: return "RPL_ENDOFINVITELIST";
        case Type::RPL_WHOISACTUALLY: return "RPL_WHOISACTUALLY";
        case Type::RPL_INVITING: return "RPL_INVITING";
        case Type::RPL_INVEXLIST: return "RPL_INVEXLIST";
        case Type::RPL_ENDOFINVEXLIST: return "RPL_ENDOFINVEXLIST";
        case Type::RPL_EXCEPTLIST: return "RPL_EXCEPTLIST";
        case Type::RPL_ENDOFEXCEPTLIST: return "RPL_ENDOFEXCEPTLIST";
        case Type::RPL_VERSION: return "RPL_VERSION";
        case Type::RPL_NAMREPLY: return "RPL_NAMREPLY";
        case Type::RPL_ENDOFNAMES: return "RPL_ENDOFNAMES";
        case Type::RPL_LINKS: return "RPL_LINKS";
        case Type::RPL_ENDOFLINKS: return "RPL_ENDOFLINKS";
        case Type::RPL_BANLIST: return "RPL_BANLIST";
        case Type::RPL_ENDOFBANLIST: return "RPL_ENDOFBANLIST";
        case Type::RPL_ENDOFWHOWAS: return "RPL_ENDOFWHOWAS";
        case Type::RPL_INFO: return "RPL_INFO";
        case Type::RPL_ENDOFINFO: return "RPL_ENDOFINFO";
        case Type::RPL_MOTDSTART: return "RPL_MOTDSTART";
        case Type::RPL_MOTD: return "RPL_MOTD";
        case Type::RPL_ENDOFMOTD: return "RPL_ENDOFMOTD";
        case Type::RPL_WHOISHOST: return "RPL_WHOISHOST";
        case Type::RPL_WHOISMODES: return "RPL_WHOISMODES";
        case Type::RPL_YOUREOPER: return "RPL_YOUREOPER";
        case Type::RPL_REHASHING: return "RPL_REHASHING";
        case Type::RPL_TIME: return "RPL_TIME";
        case Type::ERR_UNKNOWNERROR: return "ERR_UNKNOWNERROR";
        case Type::ERR_NOSUCHNICK: return "ERR_NOSUCHNICK";
        case Type::ERR_NOSUCHSERVER: return "ERR_NOSUCHSERVER";
        case Type::ERR_NOSUCHCHANNEL: return "ERR_NOSUCHCHANNEL";
        case Type::ERR_CANNOTSENDTOCHAN: return "ERR_CANNOTSENDTOCHAN";
        case Type::ERR_TOOMANYCHANNELS: return "ERR_TOOMANYCHANNELS";
        case Type::ERR_WASNOSUCHNICK: return "ERR_WASNOSUCHNICK";
        case Type::ERR_NOORIGIN: return "ERR_NOORIGIN";
        case Type::ERR_NORECIPIENT: return "ERR_NORECIPIENT";
        case Type::ERR_NOTEXTTOSEND: return "ERR_NOTEXTTOSEND";
        case Type::ERR_INPUTTOOLONG: return "ERR_INPUTTOOLONG";
        case Type::ERR_UNKNOWNCOMMAND: return "ERR_UNKNOWNCOMMAND";
        case Type::ERR_NOMOTD: return "ERR_NOMOTD";
        case Type::ERR_NONICKNAMEGIVEN: return "ERR_NONICKNAMEGIVEN";
        case Type::ERR_ERRONEUSNICKNAME: return "ERR_ERRONEUSNICKNAME";
        case Type::ERR_NICKNAMEINUSE: return "ERR_NICKNAMEINUSE";
        case Type::ERR_NICKCOLLISION: return "ERR_NICKCOLLISION";
        case Type::ERR_USERNOTINCHANNEL: return "ERR_USERNOTINCHANNEL";
        case Type::ERR_NOTONCHANNEL: return "ERR_NOTONCHANNEL";
        case Type::ERR_USERONCHANNEL: return "ERR_USERONCHANNEL";
        case Type::ERR_NOTREGISTERED: return "ERR_NOTREGISTERED";
        case Type::ERR_NEEDMOREPARAMS: return "ERR_NEEDMOREPARAMS";
        case Type::ERR_ALREADYREGISTERED: return "ERR_ALREADYREGISTERED";
        case Type::ERR_PASSWDMISMATCH: return "ERR_PASSWDMISMATCH";
        case Type::ERR_YOUREBANNEDCREEP: return "ERR_YOUREBANNEDCREEP";
        case Type::ERR_CHANNELISFULL: return "ERR_CHANNELISFULL";
        case Type::ERR_UNKNOWNMODE: return "ERR_UNKNOWNMODE";
        case Type::ERR_INVITEONLYCHAN: return "ERR_INVITEONLYCHAN";
        case Type::ERR_BANNEDFROMCHAN: return "ERR_BANNEDFROMCHAN";
        case Type::ERR_BADCHANNELKEY: return "ERR_BADCHANNELKEY";
        case Type::ERR_BADCHANMASK: return "ERR_BADCHANMASK";
        case Type::ERR_NOPRIVILEGES: return "ERR_NOPRIVILEGES";
        case Type::ERR_CHANOPRIVSNEEDED: return "ERR_CHANOPRIVSNEEDED";
        case Type::ERR_CANTKILLSERVER: return "ERR_CANTKILLSERVER";
        case Type::ERR_NOOPERHOST: return "ERR_NOOPERHOST";
        case Type::ERR_UMODEUNKNOWNFLAG: return "ERR_UMODEUNKNOWNFLAG";
        case Type::ERR_USERSDONTMATCH: return "ERR_USERSDONTMATCH";
        case Type::ERR_HELPNOTFOUND: return "ERR_HELPNOTFOUND";
        case Type::ERR_INVALIDKEY: return "ERR_INVALIDKEY";
        case Type::RPL_STARTTLS: return "RPL_STARTTLS";
        case Type::RPL_WHOISSECURE: return "RPL_WHOISSECURE";
        case Type::ERR_STARTTLS: return "ERR_STARTTLS";
        case Type::ERR_INVALIDMODEPARAM: return "ERR_INVALIDMODEPARAM";
        case Type::RPL_HELPSTART: return "RPL_HELPSTART";
        case Type::RPL_HELPTXT: return "RPL_HELPTXT";
        case Type::RPL_ENDOFHELP: return "RPL_ENDOFHELP";
        case Type::ERR_NOPRIVS: return "ERR_NOPRIVS";
        case Type::RPL_LOGGEDIN: return "RPL_LOGGEDIN";
        case Type::RPL_LOGGEDOUT: return "RPL_LOGGEDOUT";
        case Type::ERR_NICKLOCKED: return "ERR_NICKLOCKED";
        case Type::RPL_SASLSUCCESS: return "RPL_SASLSUCCESS";
        case Type::ERR_SASLFAIL: return "ERR_SASLFAIL";
        case Type::ERR_SASLTOOLONG: return "ERR_SASLTOOLONG";
        case Type::ERR_SASLABORTED: return "ERR_SASLABORTED";
        case Type::ERR_SASLALREADY: return "ERR_SASLALREADY";
        case Type::RPL_SASLMECHS: return "RPL_SASLMECHS";
        case Type::AUTHENTICATE: return "AUTHENTICATE";
        default: return "UNMAPPED";
    }
}

void IRCEventEnvelope::postprocess() {
    switch (this->type_) {
        case Type::JOIN:
            postprocess_JOIN();
            break;
        case Type::PART:
            postprocess_PART();
            break;
        case Type::KICK:
            postprocess_KICK();
            break;
        case Type::PRIVMSG:
            postprocess_PRIVMSG();
            break;
        case Type::S_RPL_CAP:
            postprocess_S_RPL_CAP();
            break;
        case Type::NOTICE:
            postprocess_NOTICE();
            break;
        case Type::PING:
            postprocess_PING();
            break;
        case Type::QUIT:
            postprocess_QUIT();
            break;
        case Type::NICK:
            postprocess_NICK();
            break;
        case Type::TOPIC:
            postprocess_TOPIC();
            break;
        case Type::INVITE:
            postprocess_INVITE();
            break;
        case Type::ERROR:
            postprocess_ERROR();
            break;
        case Type::AUTHENTICATE:
            postprocess_AUTHENTICATE();
            break;
        case Type::S_PRIVATE_MESSAGE:
            postprocess_S_PRIVATE_MESSAGE();
            break;
        case Type::S_CHANNEL_MESSAGE:
            postprocess_S_CHANNEL_MESSAGE();
            break;
        case Type::S_RPL_CAP_LS:
            postprocess_S_RPL_CAP_LS();
            break;
        case Type::S_RPL_CAP_ACK:
            postprocess_S_RPL_CAP_ACK();
            break;
        case Type::RPL_WELCOME:
            postprocess_RPL_WELCOME();
            break;
        case Type::RPL_YOURHOST:
            postprocess_RPL_YOURHOST();
            break;
        case Type::RPL_CREATED:
            postprocess_RPL_CREATED();
            break;
        case Type::RPL_MYINFO:
            postprocess_RPL_MYINFO();
            break;
        case Type::RPL_ISUPPORT:
            postprocess_RPL_ISUPPORT();
            break;
        case Type::RPL_BOUNCE:
            postprocess_RPL_BOUNCE();
            break;
        case Type::RPL_STATSCOMMANDS:
            postprocess_RPL_STATSCOMMANDS();
            break;
        case Type::RPL_ENDOFSTATS:
            postprocess_RPL_ENDOFSTATS();
            break;
        case Type::RPL_STATSUPTIME:
            postprocess_RPL_STATSUPTIME();
            break;
        case Type::RPL_UMODEIS:
            postprocess_RPL_UMODEIS();
            break;
        case Type::RPL_LUSERCLIENT:
            postprocess_RPL_LUSERCLIENT();
            break;
        case Type::RPL_LUSEROP:
            postprocess_RPL_LUSEROP();
            break;
        case Type::RPL_LUSERUNKNOWN:
            postprocess_RPL_LUSERUNKNOWN();
            break;
        case Type::RPL_LUSERCHANNELS:
            postprocess_RPL_LUSERCHANNELS();
            break;
        case Type::RPL_LUSERME:
            postprocess_RPL_LUSERME();
            break;
        case Type::RPL_ADMINME:
            postprocess_RPL_ADMINME();
            break;
        case Type::RPL_ADMINLOC1:
            postprocess_RPL_ADMINLOC1();
            break;
        case Type::RPL_ADMINLOC2:
            postprocess_RPL_ADMINLOC2();
            break;
        case Type::RPL_ADMINEMAIL:
            postprocess_RPL_ADMINEMAIL();
            break;
        case Type::RPL_TRYAGAIN:
            postprocess_RPL_TRYAGAIN();
            break;
        case Type::RPL_LOCALUSERS:
            postprocess_RPL_LOCALUSERS();
            break;
        case Type::RPL_GLOBALUSERS:
            postprocess_RPL_GLOBALUSERS();
            break;
        case Type::RPL_WHOISCERTFP:
            postprocess_RPL_WHOISCERTFP();
            break;
        case Type::RPL_NONE:
            postprocess_RPL_NONE();
            break;
        case Type::RPL_AWAY:
            postprocess_RPL_AWAY();
            break;
        case Type::RPL_USERHOST:
            postprocess_RPL_USERHOST();
            break;
        case Type::RPL_UNAWAY:
            postprocess_RPL_UNAWAY();
            break;
        case Type::RPL_NOWAWAY:
            postprocess_RPL_NOWAWAY();
            break;
        case Type::RPL_WHOREPLY:
            postprocess_RPL_WHOREPLY();
            break;
        case Type::RPL_ENDOFWHO:
            postprocess_RPL_ENDOFWHO();
            break;
        case Type::RPL_WHOISREGNICK:
            postprocess_RPL_WHOISREGNICK();
            break;
        case Type::RPL_WHOISUSER:
            postprocess_RPL_WHOISUSER();
            break;
        case Type::RPL_WHOISSERVER:
            postprocess_RPL_WHOISSERVER();
            break;
        case Type::RPL_WHOISOPERATOR:
            postprocess_RPL_WHOISOPERATOR();
            break;
        case Type::RPL_WHOWASUSER:
            postprocess_RPL_WHOWASUSER();
            break;
        case Type::RPL_WHOISIDLE:
            postprocess_RPL_WHOISIDLE();
            break;
        case Type::RPL_ENDOFWHOIS:
            postprocess_RPL_ENDOFWHOIS();
            break;
        case Type::RPL_WHOISCHANNELS:
            postprocess_RPL_WHOISCHANNELS();
            break;
        case Type::RPL_WHOISSPECIAL:
            postprocess_RPL_WHOISSPECIAL();
            break;
        case Type::RPL_LISTSTART:
            postprocess_RPL_LISTSTART();
            break;
        case Type::RPL_LIST:
            postprocess_RPL_LIST();
            break;
        case Type::RPL_LISTEND:
            postprocess_RPL_LISTEND();
            break;
        case Type::RPL_CHANNELMODEIS:
            postprocess_RPL_CHANNELMODEIS();
            break;
        case Type::RPL_CREATIONTIME:
            postprocess_RPL_CREATIONTIME();
            break;
        case Type::RPL_WHOISACCOUNT:
            postprocess_RPL_WHOISACCOUNT();
            break;
        case Type::RPL_NOTOPIC:
            postprocess_RPL_NOTOPIC();
            break;
        case Type::RPL_TOPIC:
            postprocess_RPL_TOPIC();
            break;
        case Type::RPL_TOPICWHOTIME:
            postprocess_RPL_TOPICWHOTIME();
            break;
        case Type::RPL_INVITELIST:
            postprocess_RPL_INVITELIST();
            break;
        case Type::RPL_ENDOFINVITELIST:
            postprocess_RPL_ENDOFINVITELIST();
            break;
        case Type::RPL_WHOISACTUALLY:
            postprocess_RPL_WHOISACTUALLY();
            break;
        case Type::RPL_INVITING:
            postprocess_RPL_INVITING();
            break;
        case Type::RPL_INVEXLIST:
            postprocess_RPL_INVEXLIST();
            break;
        case Type::RPL_ENDOFINVEXLIST:
            postprocess_RPL_ENDOFINVEXLIST();
            break;
        case Type::RPL_EXCEPTLIST:
            postprocess_RPL_EXCEPTLIST();
            break;
        case Type::RPL_ENDOFEXCEPTLIST:
            postprocess_RPL_ENDOFEXCEPTLIST();
            break;
        case Type::RPL_VERSION:
            postprocess_RPL_VERSION();
            break;
        case Type::RPL_NAMREPLY:
            postprocess_RPL_NAMREPLY();
            break;
        case Type::RPL_ENDOFNAMES:
            postprocess_RPL_ENDOFNAMES();
            break;
        case Type::RPL_LINKS:
            postprocess_RPL_LINKS();
            break;
        case Type::RPL_ENDOFLINKS:
            postprocess_RPL_ENDOFLINKS();
            break;
        case Type::RPL_BANLIST:
            postprocess_RPL_BANLIST();
            break;
        case Type::RPL_ENDOFBANLIST:
            postprocess_RPL_ENDOFBANLIST();
            break;
        case Type::RPL_ENDOFWHOWAS:
            postprocess_RPL_ENDOFWHOWAS();
            break;
        case Type::RPL_INFO:
            postprocess_RPL_INFO();
            break;
        case Type::RPL_ENDOFINFO:
            postprocess_RPL_ENDOFINFO();
            break;
        case Type::RPL_MOTDSTART:
            postprocess_RPL_MOTDSTART();
            break;
        case Type::RPL_MOTD:
            postprocess_RPL_MOTD();
            break;
        case Type::RPL_ENDOFMOTD:
            postprocess_RPL_ENDOFMOTD();
            break;
        case Type::RPL_WHOISHOST:
            postprocess_RPL_WHOISHOST();
            break;
        case Type::RPL_WHOISMODES:
            postprocess_RPL_WHOISMODES();
            break;
        case Type::RPL_YOUREOPER:
            postprocess_RPL_YOUREOPER();
            break;
        case Type::RPL_REHASHING:
            postprocess_RPL_REHASHING();
            break;
        case Type::RPL_TIME:
            postprocess_RPL_TIME();
            break;
        case Type::ERR_UNKNOWNERROR:
            postprocess_ERR_UNKNOWNERROR();
            break;
        case Type::ERR_NOSUCHNICK:
            postprocess_ERR_NOSUCHNICK();
            break;
        case Type::ERR_NOSUCHSERVER:
            postprocess_ERR_NOSUCHSERVER();
            break;
        case Type::ERR_NOSUCHCHANNEL:
            postprocess_ERR_NOSUCHCHANNEL();
            break;
        case Type::ERR_CANNOTSENDTOCHAN:
            postprocess_ERR_CANNOTSENDTOCHAN();
            break;
        case Type::ERR_TOOMANYCHANNELS:
            postprocess_ERR_TOOMANYCHANNELS();
            break;
        case Type::ERR_WASNOSUCHNICK:
            postprocess_ERR_WASNOSUCHNICK();
            break;
        case Type::ERR_NOORIGIN:
            postprocess_ERR_NOORIGIN();
            break;
        case Type::ERR_NORECIPIENT:
            postprocess_ERR_NORECIPIENT();
            break;
        case Type::ERR_NOTEXTTOSEND:
            postprocess_ERR_NOTEXTTOSEND();
            break;
        case Type::ERR_INPUTTOOLONG:
            postprocess_ERR_INPUTTOOLONG();
            break;
        case Type::ERR_UNKNOWNCOMMAND:
            postprocess_ERR_UNKNOWNCOMMAND();
            break;
        case Type::ERR_NOMOTD:
            postprocess_ERR_NOMOTD();
            break;
        case Type::ERR_NONICKNAMEGIVEN:
            postprocess_ERR_NONICKNAMEGIVEN();
            break;
        case Type::ERR_ERRONEUSNICKNAME:
            postprocess_ERR_ERRONEUSNICKNAME();
            break;
        case Type::ERR_NICKNAMEINUSE:
            postprocess_ERR_NICKNAMEINUSE();
            break;
        case Type::ERR_NICKCOLLISION:
            postprocess_ERR_NICKCOLLISION();
            break;
        case Type::ERR_USERNOTINCHANNEL:
            postprocess_ERR_USERNOTINCHANNEL();
            break;
        case Type::ERR_NOTONCHANNEL:
            postprocess_ERR_NOTONCHANNEL();
            break;
        case Type::ERR_USERONCHANNEL:
            postprocess_ERR_USERONCHANNEL();
            break;
        case Type::ERR_NOTREGISTERED:
            postprocess_ERR_NOTREGISTERED();
            break;
        case Type::ERR_NEEDMOREPARAMS:
            postprocess_ERR_NEEDMOREPARAMS();
            break;
        case Type::ERR_ALREADYREGISTERED:
            postprocess_ERR_ALREADYREGISTERED();
            break;
        case Type::ERR_PASSWDMISMATCH:
            postprocess_ERR_PASSWDMISMATCH();
            break;
        case Type::ERR_YOUREBANNEDCREEP:
            postprocess_ERR_YOUREBANNEDCREEP();
            break;
        case Type::ERR_CHANNELISFULL:
            postprocess_ERR_CHANNELISFULL();
            break;
        case Type::ERR_UNKNOWNMODE:
            postprocess_ERR_UNKNOWNMODE();
            break;
        case Type::ERR_INVITEONLYCHAN:
            postprocess_ERR_INVITEONLYCHAN();
            break;
        case Type::ERR_BANNEDFROMCHAN:
            postprocess_ERR_BANNEDFROMCHAN();
            break;
        case Type::ERR_BADCHANNELKEY:
            postprocess_ERR_BADCHANNELKEY();
            break;
        case Type::ERR_BADCHANMASK:
            postprocess_ERR_BADCHANMASK();
            break;
        case Type::ERR_NOPRIVILEGES:
            postprocess_ERR_NOPRIVILEGES();
            break;
        case Type::ERR_CHANOPRIVSNEEDED:
            postprocess_ERR_CHANOPRIVSNEEDED();
            break;
        case Type::ERR_CANTKILLSERVER:
            postprocess_ERR_CANTKILLSERVER();
            break;
        case Type::ERR_NOOPERHOST:
            postprocess_ERR_NOOPERHOST();
            break;
        case Type::ERR_UMODEUNKNOWNFLAG:
            postprocess_ERR_UMODEUNKNOWNFLAG();
            break;
        case Type::ERR_USERSDONTMATCH:
            postprocess_ERR_USERSDONTMATCH();
            break;
        case Type::ERR_HELPNOTFOUND:
            postprocess_ERR_HELPNOTFOUND();
            break;
        case Type::ERR_INVALIDKEY:
            postprocess_ERR_INVALIDKEY();
            break;
        case Type::RPL_STARTTLS:
            postprocess_RPL_STARTTLS();
            break;
        case Type::RPL_WHOISSECURE:
            postprocess_RPL_WHOISSECURE();
            break;
        case Type::ERR_STARTTLS:
            postprocess_ERR_STARTTLS();
            break;
        case Type::ERR_INVALIDMODEPARAM:
            postprocess_ERR_INVALIDMODEPARAM();
            break;
        case Type::RPL_HELPSTART:
            postprocess_RPL_HELPSTART();
            break;
        case Type::RPL_HELPTXT:
            postprocess_RPL_HELPTXT();
            break;
        case Type::RPL_ENDOFHELP:
            postprocess_RPL_ENDOFHELP();
            break;
        case Type::ERR_NOPRIVS:
            postprocess_ERR_NOPRIVS();
            break;
        case Type::RPL_LOGGEDIN:
            postprocess_RPL_LOGGEDIN();
            break;
        case Type::RPL_LOGGEDOUT:
            postprocess_RPL_LOGGEDOUT();
            break;
        case Type::ERR_NICKLOCKED:
            postprocess_ERR_NICKLOCKED();
            break;
        case Type::RPL_SASLSUCCESS:
            postprocess_RPL_SASLSUCCESS();
            break;
        case Type::ERR_SASLFAIL:
            postprocess_ERR_SASLFAIL();
            break;
        case Type::ERR_SASLTOOLONG:
            postprocess_ERR_SASLTOOLONG();
            break;
        case Type::ERR_SASLABORTED:
            postprocess_ERR_SASLABORTED();
            break;
        case Type::ERR_SASLALREADY:
            postprocess_ERR_SASLALREADY();
            break;
        case Type::RPL_SASLMECHS:
            postprocess_RPL_SASLMECHS();
            break;
        case Type::UNKNOWN:
            postprocess_UNKNOWN();
            break;
    }
}