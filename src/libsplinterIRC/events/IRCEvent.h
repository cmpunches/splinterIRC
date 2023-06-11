#ifndef SPLINTERIRC_IRCEVENT_H
#define SPLINTERIRC_IRCEVENT_H

// orientation

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

class IRCEvent {
    public:

        enum class Type {
            // should cover all IRC event types per RFC 2812
            UNKNOWN,
            JOIN,
            PART,
            KICK,
            PRIVMSG,
            NOTICE,
            PING,
            QUIT,
            NICK,
            TOPIC,
            INVITE,
            ERROR,

            // synthetic types
            S_PRIVATE_MESSAGE,
            S_CHANNEL_MESSAGE,

            // numeric replies
            RPL_WELCOME,
            RPL_YOURHOST,
            RPL_CREATED,
            RPL_MYINFO,
            RPL_ISUPPORT,
            RPL_BOUNCE,
            RPL_STATSCOMMANDS,
            RPL_ENDOFSTATS,
            RPL_STATSUPTIME,
            RPL_UMODEIS,
            RPL_LUSERCLIENT,
            RPL_LUSEROP,
            RPL_LUSERUNKNOWN,
            RPL_LUSERCHANNELS,
            RPL_LUSERME,
            RPL_ADMINME,
            RPL_ADMINLOC1,
            RPL_ADMINLOC2,
            RPL_ADMINEMAIL,
            RPL_TRYAGAIN,
            RPL_LOCALUSERS,
            RPL_GLOBALUSERS,
            RPL_WHOISCERTFP,
            RPL_NONE,
            RPL_AWAY,
            RPL_USERHOST,
            RPL_UNAWAY,
            RPL_NOWAWAY,
            RPL_WHOREPLY,
            RPL_ENDOFWHO,
            RPL_WHOISREGNICK,
            RPL_WHOISUSER,
            RPL_WHOISSERVER,
            RPL_WHOISOPERATOR,
            RPL_WHOWASUSER,
            RPL_WHOISIDLE,
            RPL_ENDOFWHOIS,
            RPL_WHOISCHANNELS,
            RPL_WHOISSPECIAL,
            RPL_LISTSTART,
            RPL_LIST,
            RPL_LISTEND,
            RPL_CHANNELMODEIS,
            RPL_CREATIONTIME,
            RPL_WHOISACCOUNT,
            RPL_NOTOPIC,
            RPL_TOPIC,
            RPL_TOPICWHOTIME,
            RPL_INVITELIST,
            RPL_ENDOFINVITELIST,
            RPL_WHOISACTUALLY,
            RPL_INVITING,
            RPL_INVEXLIST,
            RPL_ENDOFINVEXLIST,
            RPL_EXCEPTLIST,
            RPL_ENDOFEXCEPTLIST,
            RPL_VERSION,
            RPL_NAMREPLY,
            RPL_ENDOFNAMES,
            RPL_LINKS,
            RPL_ENDOFLINKS,
            RPL_BANLIST,
            RPL_ENDOFBANLIST,
            RPL_ENDOFWHOWAS,
            RPL_INFO,
            RPL_ENDOFINFO,
            RPL_MOTDSTART,
            RPL_MOTD,
            RPL_ENDOFMOTD,
            RPL_WHOISHOST,
            RPL_WHOISMODES,
            RPL_YOUREOPER,
            RPL_REHASHING,
            RPL_TIME,
            ERR_UNKNOWNERROR,
            ERR_NOSUCHNICK,
            ERR_NOSUCHSERVER,
            ERR_NOSUCHCHANNEL,
            ERR_CANNOTSENDTOCHAN,
            ERR_TOOMANYCHANNELS,
            ERR_WASNOSUCHNICK,
            ERR_NOORIGIN,
            ERR_NORECIPIENT,
            ERR_NOTEXTTOSEND,
            ERR_INPUTTOOLONG,
            ERR_UNKNOWNCOMMAND,
            ERR_NOMOTD,
            ERR_NONICKNAMEGIVEN,
            ERR_ERRONEUSNICKNAME,
            ERR_NICKNAMEINUSE,
            ERR_NICKCOLLISION,
            ERR_USERNOTINCHANNEL,
            ERR_NOTONCHANNEL,
            ERR_USERONCHANNEL,
            ERR_NOTREGISTERED,
            ERR_NEEDMOREPARAMS,
            ERR_ALREADYREGISTERED,
            ERR_PASSWDMISMATCH,
            ERR_YOUREBANNEDCREEP,
            ERR_CHANNELISFULL,
            ERR_UNKNOWNMODE,
            ERR_INVITEONLYCHAN,
            ERR_BANNEDFROMCHAN,
            ERR_BADCHANNELKEY,
            ERR_BADCHANMASK,
            ERR_NOPRIVILEGES,
            ERR_CHANOPRIVSNEEDED,
            ERR_CANTKILLSERVER,
            ERR_NOOPERHOST,
            ERR_UMODEUNKNOWNFLAG,
            ERR_USERSDONTMATCH,
            ERR_HELPNOTFOUND,
            ERR_INVALIDKEY,
            RPL_STARTTLS,
            RPL_WHOISSECURE,
            ERR_STARTTLS,
            ERR_INVALIDMODEPARAM,
            RPL_HELPSTART,
            RPL_HELPTXT,
            RPL_ENDOFHELP,
            ERR_NOPRIVS,
            RPL_LOGGEDIN,
            RPL_LOGGEDOUT,
            ERR_NICKLOCKED,
            RPL_SASLSUCCESS,
            ERR_SASLFAIL,
            ERR_SASLTOOLONG,
            ERR_SASLABORTED,
            ERR_SASLALREADY,
            RPL_SASLMECHS
        };

        IRCEvent(const std::string& raw_message, const std::string& server);

        const std::string& raw_message() const;

        Type type() const;
        const std::string& sender() const;
        const std::string& nick() const;
        const std::string& ident() const;
        const std::string& realname() const;
        const std::string& target() const;
        const std::string& channel() const;
        const std::string& message() const;

        // type mappings for string representation of types
        std::string type_to_verb(Type type) const;
        Type verb_to_type( const std::string& token ) const;

        std::string to_json() const;

    private:
        std::string raw_message_;
        Type type_;
        std::string sender_;
        std::string nick_;
        std::string ident_;
        std::string realname_;
        std::string target_;
        std::string channel_;
        std::string message_;
        std::string server_;
};

#endif //SPLINTERIRC_IRCEVENT_H
