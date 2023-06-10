#include "IRCEvent.h"

// orientation
IRCEvent::IRCEvent(const std::string& raw_message, const std::string& server) : raw_message_(raw_message), server_(server), type_(Type::UNKNOWN) {
    // Parse the raw message as an IRC protocol event and populate properties
    // representing the event type, sender, target, etc.
    //std::cout << "(debug) raw_message: " << raw_message << std::endl;
    std::istringstream ss(raw_message.substr(0, raw_message.find_last_not_of("\r\n") + 1));
    std::string token;
    if (std::getline(ss, token, ' ')) {
        if (token[0] == ':') {
            sender_ = token.substr(1);
            // Break down sender into realname, nick, and ident
            std::string::size_type pos = sender_.find('!');
            if (pos != std::string::npos) {
                nick_ = sender_.substr(0, pos);
                std::string::size_type pos2 = sender_.find('@', pos + 1);
                if (pos2 != std::string::npos) {
                    ident_ = sender_.substr(pos + 1, pos2 - pos - 1);
                    realname_ = sender_.substr(pos2 + 1);
                }
            }
            // type mappings for parser
            if (std::getline(ss, token, ' ')) {
                if (token == "JOIN") { type_ = Type::JOIN;
                } else if (token == "PART") { type_ = Type::PART;
                } else if (token == "KICK") { type_ = Type::KICK;
                } else if (token == "PRIVMSG") { type_ = Type::PRIVMSG;
                } else if (token == "NOTICE") { type_ = Type::NOTICE;
                } else if (token == "PING") { type_ = Type::PING;
                } else if (token == "NICK" ) { type_ = Type::NICK;
                } else if (token == "MODE" ) { type_ = Type::RPL_UMODEIS;
                } else if (token == "QUIT" ) { type_ = Type::QUIT;
                } else if (token == "TOPIC" ) { type_ = Type::TOPIC;
                } else if (token == "INVITE" ) { type_ = Type::INVITE;
                } else if (token == "ERROR" ) { type_ = Type::ERROR;
                } else if ( token == "001" ) { type_ = Type::RPL_WELCOME;
                } else if ( token == "002" ) { type_ = Type::RPL_YOURHOST;
                } else if ( token == "003" ) { type_ = Type::RPL_CREATED;
                } else if ( token == "004" ) { type_ = Type::RPL_MYINFO;
                } else if ( token == "005" ) { type_ = Type::RPL_ISUPPORT;
                } else if ( token == "010" ) { type_ = Type::RPL_BOUNCE;
                } else if ( token == "212" ) { type_ = Type::RPL_STATSCOMMANDS;
                } else if ( token == "219" ) { type_ = Type::RPL_ENDOFSTATS;
                } else if ( token == "242" ) { type_ = Type::RPL_STATSUPTIME;
                } else if ( token == "221" ) { type_ = Type::RPL_UMODEIS;
                } else if ( token == "251" ) { type_ = Type::RPL_LUSERCLIENT;
                } else if ( token == "252" ) { type_ = Type::RPL_LUSEROP;
                } else if ( token == "253" ) { type_ = Type::RPL_LUSERUNKNOWN;
                } else if ( token == "254" ) { type_ = Type::RPL_LUSERCHANNELS;
                } else if ( token == "255" ) { type_ = Type::RPL_LUSERME;
                } else if ( token == "256" ) { type_ = Type::RPL_ADMINME;
                } else if ( token == "257" ) { type_ = Type::RPL_ADMINLOC1;
                } else if ( token == "258" ) { type_ = Type::RPL_ADMINLOC2;
                } else if ( token == "259" ) { type_ = Type::RPL_ADMINEMAIL;
                } else if ( token == "263" ) { type_ = Type::RPL_TRYAGAIN;
                } else if ( token == "265" ) { type_ = Type::RPL_LOCALUSERS;
                } else if ( token == "266" ) { type_ = Type::RPL_GLOBALUSERS;
                } else if ( token == "276" ) { type_ = Type::RPL_WHOISCERTFP;
                } else if ( token == "300" ) { type_ = Type::RPL_NONE;
                } else if ( token == "301" ) { type_ = Type::RPL_AWAY;
                } else if ( token == "302" ) { type_ = Type::RPL_USERHOST;
                } else if ( token == "305" ) { type_ = Type::RPL_UNAWAY;
                } else if ( token == "306" ) { type_ = Type::RPL_NOWAWAY;
                } else if ( token == "352" ) { type_ = Type::RPL_WHOREPLY;
                } else if ( token == "315" ) { type_ = Type::RPL_ENDOFWHO;
                } else if ( token == "307" ) { type_ = Type::RPL_WHOISREGNICK;
                } else if ( token == "311" ) { type_ = Type::RPL_WHOISUSER;
                } else if ( token == "312" ) { type_ = Type::RPL_WHOISSERVER;
                } else if ( token == "313" ) { type_ = Type::RPL_WHOISOPERATOR;
                } else if ( token == "314" ) { type_ = Type::RPL_WHOWASUSER;
                } else if ( token == "317" ) { type_ = Type::RPL_WHOISIDLE;
                } else if ( token == "318" ) { type_ = Type::RPL_ENDOFWHOIS;
                } else if ( token == "319" ) { type_ = Type::RPL_WHOISCHANNELS;
                } else if ( token == "320" ) { type_ = Type::RPL_WHOISSPECIAL;
                } else if ( token == "321" ) { type_ = Type::RPL_LISTSTART;
                } else if ( token == "322" ) { type_ = Type::RPL_LIST;
                } else if ( token == "323" ) { type_ = Type::RPL_LISTEND;
                } else if ( token == "324" ) { type_ = Type::RPL_CHANNELMODEIS;
                } else if ( token == "329" ) { type_ = Type::RPL_CREATIONTIME;
                } else if ( token == "330" ) { type_ = Type::RPL_WHOISACCOUNT;
                } else if ( token == "331" ) { type_ = Type::RPL_NOTOPIC;
                } else if ( token == "332" ) { type_ = Type::RPL_TOPIC;
                } else if ( token == "333" ) { type_ = Type::RPL_TOPICWHOTIME;
                } else if ( token == "336" ) { type_ = Type::RPL_INVITELIST;
                } else if ( token == "337" ) { type_ = Type::RPL_ENDOFINVITELIST;
                } else if ( token == "338" ) { type_ = Type::RPL_WHOISACTUALLY;
                } else if ( token == "341" ) { type_ = Type::RPL_INVITING;
                } else if ( token == "346" ) { type_ = Type::RPL_INVEXLIST;
                } else if ( token == "347" ) { type_ = Type::RPL_ENDOFINVEXLIST;
                } else if ( token == "348" ) { type_ = Type::RPL_EXCEPTLIST;
                } else if ( token == "349" ) { type_ = Type::RPL_ENDOFEXCEPTLIST;
                } else if ( token == "351" ) { type_ = Type::RPL_VERSION;
                } else if ( token == "353" ) { type_ = Type::RPL_NAMREPLY;
                } else if ( token == "366" ) { type_ = Type::RPL_ENDOFNAMES;
                } else if ( token == "364" ) { type_ = Type::RPL_LINKS;
                } else if ( token == "365" ) { type_ = Type::RPL_ENDOFLINKS;
                } else if ( token == "367" ) { type_ = Type::RPL_BANLIST;
                } else if ( token == "368" ) { type_ = Type::RPL_ENDOFBANLIST;
                } else if ( token == "369" ) { type_ = Type::RPL_ENDOFWHOWAS;
                } else if ( token == "371" ) { type_ = Type::RPL_INFO;
                } else if ( token == "374" ) { type_ = Type::RPL_ENDOFINFO;
                } else if ( token == "375" ) { type_ = Type::RPL_MOTDSTART;
                } else if ( token == "372" ) { type_ = Type::RPL_MOTD;
                } else if ( token == "376" ) { type_ = Type::RPL_ENDOFMOTD;
                } else if ( token == "378" ) { type_ = Type::RPL_WHOISHOST;
                } else if ( token == "379" ) { type_ = Type::RPL_WHOISMODES;
                } else if ( token == "381" ) { type_ = Type::RPL_YOUREOPER;
                } else if ( token == "382" ) { type_ = Type::RPL_REHASHING;
                } else if ( token == "391" ) { type_ = Type::RPL_TIME;
                } else if ( token == "400" ) { type_ = Type::ERR_UNKNOWNERROR;
                } else if ( token == "401" ) { type_ = Type::ERR_NOSUCHNICK;
                } else if ( token == "402" ) { type_ = Type::ERR_NOSUCHSERVER;
                } else if ( token == "403" ) { type_ = Type::ERR_NOSUCHCHANNEL;
                } else if ( token == "404" ) { type_ = Type::ERR_CANNOTSENDTOCHAN;
                } else if ( token == "405" ) { type_ = Type::ERR_TOOMANYCHANNELS;
                } else if ( token == "406" ) { type_ = Type::ERR_WASNOSUCHNICK;
                } else if ( token == "409" ) { type_ = Type::ERR_NOORIGIN;
                } else if ( token == "411" ) { type_ = Type::ERR_NORECIPIENT;
                } else if ( token == "412" ) { type_ = Type::ERR_NOTEXTTOSEND;
                } else if ( token == "417" ) { type_ = Type::ERR_INPUTTOOLONG;
                } else if ( token == "421" ) { type_ = Type::ERR_UNKNOWNCOMMAND;
                } else if ( token == "422" ) { type_ = Type::ERR_NOMOTD;
                } else if ( token == "431" ) { type_ = Type::ERR_NONICKNAMEGIVEN;
                } else if ( token == "432" ) { type_ = Type::ERR_ERRONEUSNICKNAME;
                } else if ( token == "433" ) { type_ = Type::ERR_NICKNAMEINUSE;
                } else if ( token == "436" ) { type_ = Type::ERR_NICKCOLLISION;
                } else if ( token == "441" ) { type_ = Type::ERR_USERNOTINCHANNEL;
                } else if ( token == "442" ) { type_ = Type::ERR_NOTONCHANNEL;
                } else if ( token == "443" ) { type_ = Type::ERR_USERONCHANNEL;
                } else if ( token == "451" ) { type_ = Type::ERR_NOTREGISTERED;
                } else if ( token == "461" ) { type_ = Type::ERR_NEEDMOREPARAMS;
                } else if ( token == "462" ) { type_ = Type::ERR_ALREADYREGISTERED;
                } else if ( token == "464" ) { type_ = Type::ERR_PASSWDMISMATCH;
                } else if ( token == "465" ) { type_ = Type::ERR_YOUREBANNEDCREEP;
                } else if ( token == "471" ) { type_ = Type::ERR_CHANNELISFULL;
                } else if ( token == "472" ) { type_ = Type::ERR_UNKNOWNMODE;
                } else if ( token == "473" ) { type_ = Type::ERR_INVITEONLYCHAN;
                } else if ( token == "474" ) { type_ = Type::ERR_BANNEDFROMCHAN;
                } else if ( token == "475" ) { type_ = Type::ERR_BADCHANNELKEY;
                } else if ( token == "476" ) { type_ = Type::ERR_BADCHANMASK;
                } else if ( token == "481" ) { type_ = Type::ERR_NOPRIVILEGES;
                } else if ( token == "482" ) { type_ = Type::ERR_CHANOPRIVSNEEDED;
                } else if ( token == "483" ) { type_ = Type::ERR_CANTKILLSERVER;
                } else if ( token == "491" ) { type_ = Type::ERR_NOOPERHOST;
                } else if ( token == "501" ) { type_ = Type::ERR_UMODEUNKNOWNFLAG;
                } else if ( token == "502" ) { type_ = Type::ERR_USERSDONTMATCH;
                } else if ( token == "524" ) { type_ = Type::ERR_HELPNOTFOUND;
                } else if ( token == "525" ) { type_ = Type::ERR_INVALIDKEY;
                } else if ( token == "670" ) { type_ = Type::RPL_STARTTLS;
                } else if ( token == "671" ) { type_ = Type::RPL_WHOISSECURE;
                } else if ( token == "691" ) { type_ = Type::ERR_STARTTLS;
                } else if ( token == "696" ) { type_ = Type::ERR_INVALIDMODEPARAM;
                } else if ( token == "704" ) { type_ = Type::RPL_HELPSTART;
                } else if ( token == "705" ) { type_ = Type::RPL_HELPTXT;
                } else if ( token == "706" ) { type_ = Type::RPL_ENDOFHELP;
                } else if ( token == "723" ) { type_ = Type::ERR_NOPRIVS;
                } else if ( token == "900" ) { type_ = Type::RPL_LOGGEDIN;
                } else if ( token == "901" ) { type_ = Type::RPL_LOGGEDOUT;
                } else if ( token == "902" ) { type_ = Type::ERR_NICKLOCKED;
                } else if ( token == "903" ) { type_ = Type::RPL_SASLSUCCESS;
                } else if ( token == "904" ) { type_ = Type::ERR_SASLFAIL;
                } else if ( token == "905" ) { type_ = Type::ERR_SASLTOOLONG;
                } else if ( token == "906" ) { type_ = Type::ERR_SASLABORTED;
                } else if ( token == "907" ) { type_ = Type::ERR_SASLALREADY;
                } else if ( token == "908" ) { type_ = Type::RPL_SASLMECHS;
                }
            }
        } else if (token == "PING") {
            type_ = Type::PING;
        } else if (token == "ERROR" ) {
            type_ = Type::ERROR;
        }
        if (std::getline(ss, token, ' ')) {
            target_ = token;
        }
        std::getline(ss, message_);
        if (!message_.empty() && message_[0] == ':') {
            message_ = message_.substr(1);
        }

        if (type_ == Type::ERROR) {
            // Set the target property to be blank
            target_ = "";
            // Get the rest of the line as the message
            std::getline(ss, message_);
            if (!message_.empty() && message_[0] == ':') {
                message_ = message_.substr(1);
            }
        }

        if (type_ == Type::PING) {
            sender_ = server_;
        }
    }

    // the irc protocol is a trash fire and we have to do this
    // because the format of the raw message is different depending
    // on the type of event, even for PRIVMSG which has two distinct types
    // of messages depending on whether it's a channel message or a private
    // message.
    if (type_ == Type::PRIVMSG) {
        if (target_.find('#') == 0) {
            channel_ = target_;
            target_ = sender_;
            type_ = Type::S_CHANNEL_MESSAGE;
        } else {
            channel_ = target_;
            type_ = Type::S_PRIVATE_MESSAGE;
        }
    }
}

const std::string& IRCEvent::raw_message() const {
    return raw_message_;
}


IRCEvent::Type IRCEvent::type() const {
    return type_;
}

const std::string& IRCEvent::sender() const {
    return sender_;
}

const std::string& IRCEvent::nick() const {
    return nick_;
}

const std::string& IRCEvent::ident() const {
    return ident_;
}

const std::string& IRCEvent::realname() const {
    return realname_;
}

const std::string& IRCEvent::target() const {
    return target_;
}

const std::string& IRCEvent::channel() const {
    return channel_;
}

const std::string& IRCEvent::message() const {
    return message_;
}

// type mappings for string representation of types
std::string IRCEvent::type_to_string(Type type) const {
    switch (type) {
        // TODO populate for full IRCEvent::Type coverage
        // It's pretty stupid that C++ doesn't have a way to
        // convert enum values to strings without this.
        case Type::UNKNOWN: return "UNKNOWN";
        case Type::JOIN: return "JOIN";
        case Type::PART: return "PART";
        case Type::KICK: return "KICK";
        case Type::PRIVMSG: return "PRIVMSG";
        case Type::NOTICE: return "NOTICE";
        case Type::PING: return "PING";
        case Type::NICK: return "NICK";
        case Type::RPL_UMODEIS: return "UMODEIS";
        case Type::QUIT: return "QUIT";
        case Type::TOPIC: return "TOPIC";
        case Type::INVITE: return "INVITE";
        case Type::ERROR: return "ERROR";
        case Type::S_PRIVATE_MESSAGE: return "S_PRIVATE_MESSAGE";
        case Type::S_CHANNEL_MESSAGE: return "S_CHANNEL_MESSAGE";
        case Type::RPL_WELCOME: return "RPL_WELCOME";
        case Type::RPL_YOURHOST: return "RPL_YOURHOST";
        case Type::RPL_CREATED: return "RPL_CREATED";
        case Type::RPL_MYINFO: return "RPL_MYINFO";
        case Type::RPL_ISUPPORT: return "RPL_ISUPPORT";
        case Type::RPL_LUSERCLIENT: return "RPL_LUSERCLIENT";
        case Type::RPL_LUSEROP: return "RPL_LUSEROP";
        case Type::RPL_LUSERUNKNOWN: return "RPL_LUSERUNKNOWN";
        case Type::RPL_LUSERCHANNELS: return "RPL_LUSERCHANNELS";
        case Type::RPL_LUSERME: return "RPL_LUSERME";
        case Type::RPL_LOCALUSERS: return "RPL_LOCALUSERS";
        case Type::RPL_GLOBALUSERS: return "RPL_GLOBALUSERS";
        case Type::RPL_NAMREPLY: return "RPL_NAMREPLY";
        case Type::RPL_ENDOFNAMES: return "RPL_ENDOFNAMES";
        case Type::RPL_MOTD: return "RPL_MOTD";
        case Type::RPL_MOTDSTART: return "RPL_MOTDSTART";
        case Type::RPL_ENDOFMOTD: return "RPL_ENDOFMOTD";
        case Type::ERR_YOUREBANNEDCREEP: return "ERR_YOUREBANNEDCREEP";
    }
    return "UNMAPPED";
}

std::string IRCEvent::to_json() const {
    std::ostringstream ss;
    ss << "{\n";
    std::string raw_message = raw_message_;
    if (raw_message.size() >= 2 && raw_message.substr(raw_message.size() - 2) == "\r\n") {
        raw_message = raw_message.substr(0, raw_message.size() - 2);
    } else if (raw_message.size() >= 1 && raw_message.back() == '\r') {
        raw_message = raw_message.substr(0, raw_message.size() - 1);
    } else if (raw_message.size() >= 1 && raw_message.back() == '\n') {
        raw_message = raw_message.substr(0, raw_message.size() - 1);
    }
    ss << "\t\"raw_message\": " << std::quoted(raw_message) << ",\n"; // Use std::quoted to escape special characters
    ss << "\t\"type\": \"" << type_to_string( this->type_ ) << "\",\n";
    ss << "\t\"sender\": \"" << sender_ << "\",\n";
    ss << "\t\"nick\": \"" << nick_ << "\",\n";
    ss << "\t\"ident\": \"" << ident_ << "\",\n";
    ss << "\t\"realname\": \"" << realname_ << "\",\n";
    ss << "\t\"target\": \"" << target_ << "\",\n";
    ss << "\t\"channel\": \"" << channel_ << "\",\n";
    ss << "\t\"message\": " << std::quoted(message_) << ",\n";
    ss << "\t\"server\": \"" << server_ << "\"\n";
    ss << "}\n";
    return ss.str();
}