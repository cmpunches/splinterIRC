#ifndef SPLINTERIRC_IRCEVENTV3_H
#define SPLINTERIRC_IRCEVENTV3_H

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <iomanip>
#include <iostream>

class IRCEventEnvelope {
    public:
        enum class Type {
            // should cover all IRC event types per RFC 2812
            UNKNOWN,
            JOIN,
            PART,
            KICK,
            // use generic PRIVMSG then use a synthetic type during processing
            PRIVMSG,
            // use a generic RPL_CAP then use a synthetic type during processing
            RPL_CAP,
            NOTICE,
            PING,
            QUIT,
            NICK,
            TOPIC,
            INVITE,
            ERROR,

            // use a generic AUTHENTICATE for now
            AUTHENTICATE,

            // synthetic types
            S_PRIVATE_MESSAGE,
            S_CHANNEL_MESSAGE,
            S_RPL_CAP_LS,
            S_RPL_CAP_ACK,
            S_RPL_CAP_UNKNOWN,
            S_RPL_CAP_NAK,

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

        // constructor for event envelope
        IRCEventEnvelope(const std::string& raw, const std::string& server );

        // turn the type enum into a string representation
        std::string type_to_verb(Type type) const;

        // turn a string representation of a type into the enum
        Type verb_to_type( const std::string& token ) const;

        // set arbitrary attributes to the resulting type.
        // The envelope will have some basic generic attributes whose symbol names begin with an underscore,
        // and which do not appear except in verbose/debug mode.  These will always be present in envelope
        // representation.
        void set_attribute(const std::string& key, const std::string& value);
        void set_attribute(const std::string& key, const std::vector<std::string>& value);

        // retrieve previously set arbitrary attributes by type
        std::string get_scalar_attribute(const std::string& key);
        std::vector<std::string> get_array_attribute(const std::string& key) const;
        const std::map<std::string, std::string>& get_all_scalar_attributes() const;
        const std::map<std::string, std::vector<std::string>>& get_all_array_attributes() const;

        // get the enum representation of the event type
        const Type get_type() const;

        // get the json representation of the event type
        std::string to_json(bool pretty = false, int indent = 4, bool debug = false ) const;

    private:
        // parser helper, splits strings based on a char delimiter into a vector of strings
        std::vector<std::string> split(const std::string& str, char delimiter);


        // basic parser to extract the verb and parameters
        void parse();

        // maps event types to get_type-specific postprocessor
        void postprocess();

        // postprocessors
        // the main parser method is generic to all events and gets in a state ready to be post-processed.
        // postprocessors are methods corresponding to the type since the IRC protocol is a raging trash fire and
        // uses variable formats and field placement for different events.  Not to be confused with or interact with
        // event handlers in any way.  Postprocessing is part fo the event creation process, not the event handling
        // process.
        void postprocess_UNKNOWN();
        void postprocess_JOIN();
        void postprocess_PART();
        void postprocess_KICK();
        void postprocess_PRIVMSG();
        void postprocess_S_RPL_CAP();
        void postprocess_S_RPL_CAP_LS();
        void postprocess_S_RPL_CAP_ACK();
        void postprocess_S_RPL_CAP_UNKNOWN();
        void postprocess_NOTICE();
        void postprocess_PING();
        void postprocess_QUIT();
        void postprocess_NICK();
        void postprocess_TOPIC();
        void postprocess_INVITE();
        void postprocess_ERROR();
        void postprocess_AUTHENTICATE();
        void postprocess_S_PRIVATE_MESSAGE();
        void postprocess_S_CHANNEL_MESSAGE();
        void postprocess_RPL_WELCOME();
        void postprocess_RPL_YOURHOST();
        void postprocess_RPL_CREATED();
        void postprocess_RPL_MYINFO();
        void postprocess_RPL_ISUPPORT();
        void postprocess_RPL_BOUNCE();
        void postprocess_RPL_STATSCOMMANDS();
        void postprocess_RPL_ENDOFSTATS();
        void postprocess_RPL_STATSUPTIME();
        void postprocess_RPL_UMODEIS();
        void postprocess_RPL_LUSERCLIENT();
        void postprocess_RPL_LUSEROP();
        void postprocess_RPL_LUSERUNKNOWN();
        void postprocess_RPL_LUSERCHANNELS();
        void postprocess_RPL_LUSERME();
        void postprocess_RPL_ADMINME();
        void postprocess_RPL_ADMINLOC1();
        void postprocess_RPL_ADMINLOC2();
        void postprocess_RPL_ADMINEMAIL();
        void postprocess_RPL_TRYAGAIN();
        void postprocess_RPL_LOCALUSERS();
        void postprocess_RPL_GLOBALUSERS();
        void postprocess_RPL_WHOISCERTFP();
        void postprocess_RPL_NONE();
        void postprocess_RPL_AWAY();
        void postprocess_RPL_USERHOST();
        void postprocess_RPL_UNAWAY();
        void postprocess_RPL_NOWAWAY();
        void postprocess_RPL_WHOREPLY();
        void postprocess_RPL_ENDOFWHO();
        void postprocess_RPL_WHOISREGNICK();
        void postprocess_RPL_WHOISUSER();
        void postprocess_RPL_WHOISSERVER();
        void postprocess_RPL_WHOISOPERATOR();
        void postprocess_RPL_WHOWASUSER();
        void postprocess_RPL_WHOISIDLE();
        void postprocess_RPL_ENDOFWHOIS();
        void postprocess_RPL_WHOISCHANNELS();
        void postprocess_RPL_WHOISSPECIAL();
        void postprocess_RPL_LISTSTART();
        void postprocess_RPL_LIST();
        void postprocess_RPL_LISTEND();
        void postprocess_RPL_CHANNELMODEIS();
        void postprocess_RPL_CREATIONTIME();
        void postprocess_RPL_WHOISACCOUNT();
        void postprocess_RPL_NOTOPIC();
        void postprocess_RPL_TOPIC();
        void postprocess_RPL_TOPICWHOTIME();
        void postprocess_RPL_INVITELIST();
        void postprocess_RPL_ENDOFINVITELIST();
        void postprocess_RPL_WHOISACTUALLY();
        void postprocess_RPL_INVITING();
        void postprocess_RPL_INVEXLIST();
        void postprocess_RPL_ENDOFINVEXLIST();
        void postprocess_RPL_EXCEPTLIST();
        void postprocess_RPL_ENDOFEXCEPTLIST();
        void postprocess_RPL_VERSION();
        void postprocess_RPL_NAMREPLY();
        void postprocess_RPL_ENDOFNAMES();
        void postprocess_RPL_LINKS();
        void postprocess_RPL_ENDOFLINKS();
        void postprocess_RPL_BANLIST();
        void postprocess_RPL_ENDOFBANLIST();
        void postprocess_RPL_ENDOFWHOWAS();
        void postprocess_RPL_INFO();
        void postprocess_RPL_ENDOFINFO();
        void postprocess_RPL_MOTDSTART();
        void postprocess_RPL_MOTD();
        void postprocess_RPL_ENDOFMOTD();
        void postprocess_RPL_WHOISHOST();
        void postprocess_RPL_WHOISMODES();
        void postprocess_RPL_YOUREOPER();
        void postprocess_RPL_REHASHING();
        void postprocess_RPL_TIME();
        void postprocess_ERR_UNKNOWNERROR();
        void postprocess_ERR_NOSUCHNICK();
        void postprocess_ERR_NOSUCHSERVER();
        void postprocess_ERR_NOSUCHCHANNEL();
        void postprocess_ERR_CANNOTSENDTOCHAN();
        void postprocess_ERR_TOOMANYCHANNELS();
        void postprocess_ERR_WASNOSUCHNICK();
        void postprocess_ERR_NOORIGIN();
        void postprocess_ERR_NORECIPIENT();
        void postprocess_ERR_NOTEXTTOSEND();
        void postprocess_ERR_INPUTTOOLONG();
        void postprocess_ERR_UNKNOWNCOMMAND();
        void postprocess_ERR_NOMOTD();
        void postprocess_ERR_NONICKNAMEGIVEN();
        void postprocess_ERR_ERRONEUSNICKNAME();
        void postprocess_ERR_NICKNAMEINUSE();
        void postprocess_ERR_NICKCOLLISION();
        void postprocess_ERR_USERNOTINCHANNEL();
        void postprocess_ERR_NOTONCHANNEL();
        void postprocess_ERR_USERONCHANNEL();
        void postprocess_ERR_NOTREGISTERED();
        void postprocess_ERR_NEEDMOREPARAMS();
        void postprocess_ERR_ALREADYREGISTERED();
        void postprocess_ERR_PASSWDMISMATCH();
        void postprocess_ERR_YOUREBANNEDCREEP();
        void postprocess_ERR_CHANNELISFULL();
        void postprocess_ERR_UNKNOWNMODE();
        void postprocess_ERR_INVITEONLYCHAN();
        void postprocess_ERR_BANNEDFROMCHAN();
        void postprocess_ERR_BADCHANNELKEY();
        void postprocess_ERR_BADCHANMASK();
        void postprocess_ERR_NOPRIVILEGES();
        void postprocess_ERR_CHANOPRIVSNEEDED();
        void postprocess_ERR_CANTKILLSERVER();
        void postprocess_ERR_NOOPERHOST();
        void postprocess_ERR_UMODEUNKNOWNFLAG();
        void postprocess_ERR_USERSDONTMATCH();
        void postprocess_ERR_HELPNOTFOUND();
        void postprocess_ERR_INVALIDKEY();
        void postprocess_RPL_STARTTLS();
        void postprocess_RPL_WHOISSECURE();
        void postprocess_ERR_STARTTLS();
        void postprocess_ERR_INVALIDMODEPARAM();
        void postprocess_RPL_HELPSTART();
        void postprocess_RPL_HELPTXT();
        void postprocess_RPL_ENDOFHELP();
        void postprocess_ERR_NOPRIVS();
        void postprocess_RPL_LOGGEDIN();
        void postprocess_RPL_LOGGEDOUT();
        void postprocess_ERR_NICKLOCKED();
        void postprocess_RPL_SASLSUCCESS();
        void postprocess_ERR_SASLFAIL();
        void postprocess_ERR_SASLTOOLONG();
        void postprocess_ERR_SASLABORTED();
        void postprocess_ERR_SASLALREADY();
        void postprocess_RPL_SASLMECHS();

        // internal properties used by post-processors

        // enum representing the type of IRC event
        // includes native IRC protocol events as well as synthetic types as
        // some IRC event types are not directly represented in the IRC protocol
        Type type_;

        // automatically extracted parameters
        // should not be relied on for all forms of parsing but are useful for extraction of fields by position
        std::vector<std::string> autoextract_params;

        // k/v pairs used by the to_json() method
        std::map<std::string, std::string> scalar_attributes;
        std::map<std::string, std::vector<std::string>> array_attributes;
};

#endif //SPLINTERIRC_IRCEVENTV3_H
