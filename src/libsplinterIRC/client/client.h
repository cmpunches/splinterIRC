#ifndef SPLINTERIRC_CLIENT_H
#define SPLINTERIRC_CLIENT_H

#include <iostream>
#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <sstream>
#include <map>
#include <atomic>
#include "../events/IRCEvent.h"
#include <algorithm>

// the client that will be controlled by commands via PM on the control server
class splinterClient {
    public:
        splinterClient(const std::string& server, const std::string& port, const std::string& nick, const std::string& password );
        ~splinterClient();

        void connect();

        // actions
        void join_channel( std::string channel );
        void set_nick( std::string nick );
        void send_private_message( std::string nick, std::string message );
        void set_user( std::string user );

        // main loop
        void run();

    private:
        void send(const std::string& message);

        void observation_loop();
        void processing_loop();
        void decision_loop(const IRCEvent& event );

        // EVENT HANDLERS
        // These are tied to the decision_loop() function.
        // They are called when an event is received if mapped to that event type.
        void handle_channel_message(const IRCEvent& event);
        void handle_private_message(const IRCEvent& event);
        const std::string& get_server() const;
        void list_clients( const std::string& reply_to );
        void destroy_client( const std::string& reply_to, const std::string& id );
        void help_prompt( std::string reply_to, std::string& command );
        bool password_is_valid( const std::string& command );
        void handle_command( const IRCEvent& event );

        void handle_UNKNOWN( const IRCEvent& event );
        void handle_JOIN( const IRCEvent& event );
        void handle_PART( const IRCEvent& event );
        void handle_KICK( const IRCEvent& event );
        void handle_NOTICE( const IRCEvent& event );
        void handle_PING( const IRCEvent& event );
        void handle_QUIT( const IRCEvent& event );
        void handle_NICK( const IRCEvent& event );
        void handle_TOPIC( const IRCEvent& event );
        void handle_INVITE( const IRCEvent& event );
        void handle_ERROR( const IRCEvent& event );
        void handle_S_PRIVATE_MESSAGE( const IRCEvent& event );
        void handle_S_CHANNEL_MESSAGE( const IRCEvent& event );
        void handle_RPL_WELCOME( const IRCEvent& event );
        void handle_RPL_YOURHOST( const IRCEvent& event );
        void handle_RPL_CREATED( const IRCEvent& event );
        void handle_RPL_MYINFO( const IRCEvent& event );
        void handle_RPL_ISUPPORT( const IRCEvent& event );
        void handle_RPL_BOUNCE( const IRCEvent& event );
        void handle_RPL_STATSCOMMANDS( const IRCEvent& event );
        void handle_RPL_ENDOFSTATS( const IRCEvent& event );
        void handle_RPL_STATSUPTIME( const IRCEvent& event );
        void handle_RPL_UMODEIS( const IRCEvent& event );
        void handle_RPL_LUSERCLIENT( const IRCEvent& event );
        void handle_RPL_LUSEROP( const IRCEvent& event );
        void handle_RPL_LUSERUNKNOWN( const IRCEvent& event );
        void handle_RPL_LUSERCHANNELS( const IRCEvent& event );
        void handle_RPL_LUSERME( const IRCEvent& event );
        void handle_RPL_ADMINME( const IRCEvent& event );
        void handle_RPL_ADMINLOC1( const IRCEvent& event );
        void handle_RPL_ADMINLOC2( const IRCEvent& event );
        void handle_RPL_ADMINEMAIL( const IRCEvent& event );
        void handle_RPL_TRYAGAIN( const IRCEvent& event );
        void handle_RPL_LOCALUSERS( const IRCEvent& event );
        void handle_RPL_GLOBALUSERS( const IRCEvent& event );
        void handle_RPL_WHOISCERTFP( const IRCEvent& event );
        void handle_RPL_NONE( const IRCEvent& event );
        void handle_RPL_AWAY( const IRCEvent& event );
        void handle_RPL_USERHOST( const IRCEvent& event );
        void handle_RPL_UNAWAY( const IRCEvent& event );
        void handle_RPL_NOWAWAY( const IRCEvent& event );
        void handle_RPL_WHOREPLY( const IRCEvent& event );
        void handle_RPL_ENDOFWHO( const IRCEvent& event );
        void handle_RPL_WHOISREGNICK( const IRCEvent& event );
        void handle_RPL_WHOISUSER( const IRCEvent& event );
        void handle_RPL_WHOISSERVER( const IRCEvent& event );
        void handle_RPL_WHOISOPERATOR( const IRCEvent& event );
        void handle_RPL_WHOWASUSER( const IRCEvent& event );
        void handle_RPL_WHOISIDLE( const IRCEvent& event );
        void handle_RPL_ENDOFWHOIS( const IRCEvent& event );
        void handle_RPL_WHOISCHANNELS( const IRCEvent& event );
        void handle_RPL_WHOISSPECIAL( const IRCEvent& event );
        void handle_RPL_LISTSTART( const IRCEvent& event );
        void handle_RPL_LIST( const IRCEvent& event );
        void handle_RPL_LISTEND( const IRCEvent& event );
        void handle_RPL_CHANNELMODEIS( const IRCEvent& event );
        void handle_RPL_CREATIONTIME( const IRCEvent& event );
        void handle_RPL_WHOISACCOUNT( const IRCEvent& event );
        void handle_RPL_NOTOPIC( const IRCEvent& event );
        void handle_RPL_TOPIC( const IRCEvent& event );
        void handle_RPL_TOPICWHOTIME( const IRCEvent& event );
        void handle_RPL_INVITELIST( const IRCEvent& event );
        void handle_RPL_ENDOFINVITELIST( const IRCEvent& event );
        void handle_RPL_WHOISACTUALLY( const IRCEvent& event );
        void handle_RPL_INVITING( const IRCEvent& event );
        void handle_RPL_INVEXLIST( const IRCEvent& event );
        void handle_RPL_ENDOFINVEXLIST( const IRCEvent& event );
        void handle_RPL_EXCEPTLIST( const IRCEvent& event );
        void handle_RPL_ENDOFEXCEPTLIST( const IRCEvent& event );
        void handle_RPL_VERSION( const IRCEvent& event );
        void handle_RPL_NAMREPLY( const IRCEvent& event );
        void handle_RPL_ENDOFNAMES( const IRCEvent& event );
        void handle_RPL_LINKS( const IRCEvent& event );
        void handle_RPL_ENDOFLINKS( const IRCEvent& event );
        void handle_RPL_BANLIST( const IRCEvent& event );
        void handle_RPL_ENDOFBANLIST( const IRCEvent& event );
        void handle_RPL_ENDOFWHOWAS( const IRCEvent& event );
        void handle_RPL_INFO( const IRCEvent& event );
        void handle_RPL_ENDOFINFO( const IRCEvent& event );
        void handle_RPL_MOTDSTART( const IRCEvent& event );
        void handle_RPL_MOTD( const IRCEvent& event );
        void handle_RPL_ENDOFMOTD( const IRCEvent& event );
        void handle_RPL_WHOISHOST( const IRCEvent& event );
        void handle_RPL_WHOISMODES( const IRCEvent& event );
        void handle_RPL_YOUREOPER( const IRCEvent& event );
        void handle_RPL_REHASHING( const IRCEvent& event );
        void handle_RPL_TIME( const IRCEvent& event );
        void handle_ERR_UNKNOWNERROR( const IRCEvent& event );
        void handle_ERR_NOSUCHNICK( const IRCEvent& event );
        void handle_ERR_NOSUCHSERVER( const IRCEvent& event );
        void handle_ERR_NOSUCHCHANNEL( const IRCEvent& event );
        void handle_ERR_CANNOTSENDTOCHAN( const IRCEvent& event );
        void handle_ERR_TOOMANYCHANNELS( const IRCEvent& event );
        void handle_ERR_WASNOSUCHNICK( const IRCEvent& event );
        void handle_ERR_NOORIGIN( const IRCEvent& event );
        void handle_ERR_NORECIPIENT( const IRCEvent& event );
        void handle_ERR_NOTEXTTOSEND( const IRCEvent& event );
        void handle_ERR_INPUTTOOLONG( const IRCEvent& event );
        void handle_ERR_UNKNOWNCOMMAND( const IRCEvent& event );
        void handle_ERR_NOMOTD( const IRCEvent& event );
        void handle_ERR_NONICKNAMEGIVEN( const IRCEvent& event );
        void handle_ERR_ERRONEUSNICKNAME( const IRCEvent& event );
        void handle_ERR_NICKNAMEINUSE( const IRCEvent& event );
        void handle_ERR_NICKCOLLISION( const IRCEvent& event );
        void handle_ERR_USERNOTINCHANNEL( const IRCEvent& event );
        void handle_ERR_NOTONCHANNEL( const IRCEvent& event );
        void handle_ERR_USERONCHANNEL( const IRCEvent& event );
        void handle_ERR_NOTREGISTERED( const IRCEvent& event );
        void handle_ERR_NEEDMOREPARAMS( const IRCEvent& event );
        void handle_ERR_ALREADYREGISTERED( const IRCEvent& event );
        void handle_ERR_PASSWDMISMATCH( const IRCEvent& event );
        void handle_ERR_YOUREBANNEDCREEP( const IRCEvent& event );
        void handle_ERR_CHANNELISFULL( const IRCEvent& event );
        void handle_ERR_UNKNOWNMODE( const IRCEvent& event );
        void handle_ERR_INVITEONLYCHAN( const IRCEvent& event );
        void handle_ERR_BANNEDFROMCHAN( const IRCEvent& event );
        void handle_ERR_BADCHANNELKEY( const IRCEvent& event );
        void handle_ERR_BADCHANMASK( const IRCEvent& event );
        void handle_ERR_NOPRIVILEGES( const IRCEvent& event );
        void handle_ERR_CHANOPRIVSNEEDED( const IRCEvent& event );
        void handle_ERR_CANTKILLSERVER( const IRCEvent& event );
        void handle_ERR_NOOPERHOST( const IRCEvent& event );
        void handle_ERR_UMODEUNKNOWNFLAG( const IRCEvent& event );
        void handle_ERR_USERSDONTMATCH( const IRCEvent& event );
        void handle_ERR_HELPNOTFOUND( const IRCEvent& event );
        void handle_ERR_INVALIDKEY( const IRCEvent& event );
        void handle_RPL_STARTTLS( const IRCEvent& event );
        void handle_RPL_WHOISSECURE( const IRCEvent& event );
        void handle_ERR_STARTTLS( const IRCEvent& event );
        void handle_ERR_INVALIDMODEPARAM( const IRCEvent& event );
        void handle_RPL_HELPSTART( const IRCEvent& event );
        void handle_RPL_HELPTXT( const IRCEvent& event );
        void handle_RPL_ENDOFHELP( const IRCEvent& event );
        void handle_ERR_NOPRIVS( const IRCEvent& event );
        void handle_RPL_LOGGEDIN( const IRCEvent& event );
        void handle_RPL_LOGGEDOUT( const IRCEvent& event );
        void handle_ERR_NICKLOCKED( const IRCEvent& event );
        void handle_RPL_SASLSUCCESS( const IRCEvent& event );
        void handle_ERR_SASLFAIL( const IRCEvent& event );
        void handle_ERR_SASLTOOLONG( const IRCEvent& event );
        void handle_ERR_SASLABORTED( const IRCEvent& event );
        void handle_ERR_SASLALREADY( const IRCEvent& event );
        void handle_RPL_SASLMECHS( const IRCEvent& event );

        void report_event( const IRCEvent& event );


    std::string server_;
        std::string port_;
        std::string nick_;
        std::string channel_;
        int socket_;
        std::queue<IRCEvent> event_queue_;
        std::mutex mutex_;
        std::condition_variable cond_;
        std::string password_;
        std::map<std::string, std::shared_ptr<splinterClient>> clients_;
        int next_id_ = 1;
        std::atomic<bool> critical_thread_failed{false};


};



#endif //SPLINTERIRC_CLIENT_H
