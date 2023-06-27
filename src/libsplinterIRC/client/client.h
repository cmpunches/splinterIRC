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
#include "../events/IRCEventv3.h"
#include "../actions/IRCActionEnvelope.h"
#include <algorithm>
#include <string>
#include <vector>

// the client that will be controlled by commands via PM on the control server
class splinterClient : public std::enable_shared_from_this<splinterClient> {
    public:
        splinterClient(
                const std::string& server,
                const std::string& port,
                const std::string& nick,
                const std::string& password,
                const bool use_sasl,
                const std::string& sasl_username,
                const std::string& sasl_password,
                const bool verbose
        );
        ~splinterClient();

        // actions
        void join_channel( std::string channel );
        void set_nick( std::string nick );
        void send_private_message( std::string nick, std::string message );
        void quit( std::string message );
        void list_server_capabilities();
        void send_ping_reply( std::string target );
        void request_sasl_capability();
        void end_capabilites_negotiation();
        void whois_user( std::string nick );

        // register the user
        void register_user(std::string user );

        // get the sender of the client
        const std::string& get_nick() const;

        // get the server we are connected to
        const std::string& get_server() const;

        // connect to the targeted server
        void connect();

        // internal use for managing the splinter index
        void add_to_clients();

        // main loop
        void run_event_loop();


    private:
        // adds an action to the queue
        void enqueue_action(IRCActionEnvelope action);

        // adds an event to the event queue
        void enqueue_event(IRCEventEnvelope event);

        // executes an action on the intended splinter
        void execute_action(IRCActionEnvelope& action);

        bool is_owner( const std::string& username );
        void set_owner( const std::string& username );
        bool has_valid_session( std::string& sender );

        void handle_command_authentication(std::string& sender, std::string& command );
        void handle_command_help(std::string& sender, std::string& command );
        void handle_command_quit( std::string& sender, std::string& command );
        void handle_command_splinter( std::string& sender, std::string& command );

        static std::map<std::string, std::shared_ptr<splinterClient>> clients_;

        // get the current splinter id
        const int get_id() const;

        // current splinter id
        int splinter_id_;

        // next available splinter id
        int get_next_id();

        void send(const std::string& message);

        void observation_loop();
        void orientation_loop();
        void action_loop();

        void make_decision(IRCEventEnvelope& event );

        bool password_is_valid( const std::string& command );

        // actions
        void list_clients( const std::string& reply_to );
        void destroy_client( const std::string& reply_to, const std::string& id );

        // EVENT HANDLERS
        // These are tied to the make_decision() function.
        // They are called when an event is received if mapped to that event command.
        void prompt_help_general(std::string& reply_to );
        void prompt_help_splinter(std::string& reply_to );
        void prompt_help_destroy(std::string& reply_to );
        void prompt_help_list(std::string& reply_to );
        void prompt_help_join(std::string& reply_to );
        void prompt_help_say(std::string& reply_to );
        void prompt_help_raw(std::string& reply_to );
        void prompt_help_auth(std::string& reply_to );
        void prompt_help_quit(std::string& reply_to );

        void handle_command( IRCEventEnvelope& event );
        void handle_command_list( std::string& sender, std::string& command );
        void handle_command_destroy( std::string& sender, std::string& command );
        void handle_command_join( std::string& sender, std::string& command );
        void handle_command_say( std::string& sender, std::string& command );
        void handle_command_raw( std::string& sender, std::string& command );

        void handle_unassociated_event( IRCEventEnvelope& event );
        void handle_UNKNOWN( IRCEventEnvelope& event );
        void handle_JOIN( IRCEventEnvelope& event );
        void handle_PART( IRCEventEnvelope& event );
        void handle_KICK( IRCEventEnvelope& event );
        void handle_NOTICE( IRCEventEnvelope& event );
        void handle_PING( IRCEventEnvelope& event );
        void handle_QUIT( IRCEventEnvelope& event );
        void handle_NICK( IRCEventEnvelope& event );
        void handle_TOPIC( IRCEventEnvelope& event );
        void handle_INVITE( IRCEventEnvelope& event );
        void handle_ERROR( IRCEventEnvelope& event );
        void handle_S_PRIVATE_MESSAGE( IRCEventEnvelope& event );
        void handle_S_CHANNEL_MESSAGE( IRCEventEnvelope& event );
        void handle_RPL_WELCOME( IRCEventEnvelope& event );
        void handle_RPL_YOURHOST( IRCEventEnvelope& event );
        void handle_RPL_CREATED( IRCEventEnvelope& event );
        void handle_RPL_MYINFO( IRCEventEnvelope& event );
        void handle_RPL_ISUPPORT( IRCEventEnvelope& event );
        void handle_RPL_BOUNCE( IRCEventEnvelope& event );
        void handle_RPL_STATSCOMMANDS( IRCEventEnvelope& event );
        void handle_RPL_ENDOFSTATS( IRCEventEnvelope& event );
        void handle_RPL_STATSUPTIME( IRCEventEnvelope& event );
        void handle_RPL_UMODEIS( IRCEventEnvelope& event );
        void handle_RPL_LUSERCLIENT( IRCEventEnvelope& event );
        void handle_RPL_LUSEROP( IRCEventEnvelope& event );
        void handle_RPL_LUSERUNKNOWN( IRCEventEnvelope& event );
        void handle_RPL_LUSERCHANNELS( IRCEventEnvelope& event );
        void handle_RPL_LUSERME( IRCEventEnvelope& event );
        void handle_RPL_ADMINME( IRCEventEnvelope& event );
        void handle_RPL_ADMINLOC1( IRCEventEnvelope& event );
        void handle_RPL_ADMINLOC2( IRCEventEnvelope& event );
        void handle_RPL_ADMINEMAIL( IRCEventEnvelope& event );
        void handle_RPL_TRYAGAIN( IRCEventEnvelope& event );
        void handle_RPL_LOCALUSERS( IRCEventEnvelope& event );
        void handle_RPL_GLOBALUSERS( IRCEventEnvelope& event );
        void handle_RPL_WHOISCERTFP( IRCEventEnvelope& event );
        void handle_RPL_NONE( IRCEventEnvelope& event );
        void handle_RPL_AWAY( IRCEventEnvelope& event );
        void handle_RPL_USERHOST( IRCEventEnvelope& event );
        void handle_RPL_UNAWAY( IRCEventEnvelope& event );
        void handle_RPL_NOWAWAY( IRCEventEnvelope& event );
        void handle_RPL_WHOREPLY( IRCEventEnvelope& event );
        void handle_RPL_ENDOFWHO( IRCEventEnvelope& event );
        void handle_RPL_WHOISREGNICK( IRCEventEnvelope& event );
        void handle_RPL_WHOISUSER( IRCEventEnvelope& event );
        void handle_RPL_WHOISSERVER( IRCEventEnvelope& event );
        void handle_RPL_WHOISOPERATOR( IRCEventEnvelope& event );
        void handle_RPL_WHOWASUSER( IRCEventEnvelope& event );
        void handle_RPL_WHOISIDLE( IRCEventEnvelope& event );
        void handle_RPL_ENDOFWHOIS( IRCEventEnvelope& event );
        void handle_RPL_WHOISCHANNELS( IRCEventEnvelope& event );
        void handle_RPL_WHOISSPECIAL( IRCEventEnvelope& event );
        void handle_RPL_LISTSTART( IRCEventEnvelope& event );
        void handle_RPL_LIST( IRCEventEnvelope& event );
        void handle_RPL_LISTEND( IRCEventEnvelope& event );
        void handle_RPL_CHANNELMODEIS( IRCEventEnvelope& event );
        void handle_RPL_CREATIONTIME( IRCEventEnvelope& event );
        void handle_RPL_WHOISACCOUNT( IRCEventEnvelope& event );
        void handle_RPL_NOTOPIC( IRCEventEnvelope& event );
        void handle_RPL_TOPIC( IRCEventEnvelope& event );
        void handle_RPL_TOPICWHOTIME( IRCEventEnvelope& event );
        void handle_RPL_INVITELIST( IRCEventEnvelope& event );
        void handle_RPL_ENDOFINVITELIST( IRCEventEnvelope& event );
        void handle_RPL_WHOISACTUALLY( IRCEventEnvelope& event );
        void handle_RPL_INVITING( IRCEventEnvelope& event );
        void handle_RPL_INVEXLIST( IRCEventEnvelope& event );
        void handle_RPL_ENDOFINVEXLIST( IRCEventEnvelope& event );
        void handle_RPL_EXCEPTLIST( IRCEventEnvelope& event );
        void handle_RPL_ENDOFEXCEPTLIST( IRCEventEnvelope& event );
        void handle_RPL_VERSION( IRCEventEnvelope& event );
        void handle_RPL_NAMREPLY( IRCEventEnvelope& event );
        void handle_RPL_ENDOFNAMES( IRCEventEnvelope& event );
        void handle_RPL_LINKS( IRCEventEnvelope& event );
        void handle_RPL_ENDOFLINKS( IRCEventEnvelope& event );
        void handle_RPL_BANLIST( IRCEventEnvelope& event );
        void handle_RPL_ENDOFBANLIST( IRCEventEnvelope& event );
        void handle_RPL_ENDOFWHOWAS( IRCEventEnvelope& event );
        void handle_RPL_INFO( IRCEventEnvelope& event );
        void handle_RPL_ENDOFINFO( IRCEventEnvelope& event );
        void handle_RPL_MOTDSTART( IRCEventEnvelope& event );
        void handle_RPL_MOTD( IRCEventEnvelope& event );
        void handle_RPL_ENDOFMOTD( IRCEventEnvelope& event );
        void handle_RPL_WHOISHOST( IRCEventEnvelope& event );
        void handle_RPL_WHOISMODES( IRCEventEnvelope& event );
        void handle_RPL_YOUREOPER( IRCEventEnvelope& event );
        void handle_RPL_REHASHING( IRCEventEnvelope& event );
        void handle_RPL_TIME( IRCEventEnvelope& event );
        void handle_ERR_UNKNOWNERROR( IRCEventEnvelope& event );
        void handle_ERR_NOSUCHNICK( IRCEventEnvelope& event );
        void handle_ERR_NOSUCHSERVER( IRCEventEnvelope& event );
        void handle_ERR_NOSUCHCHANNEL( IRCEventEnvelope& event );
        void handle_ERR_CANNOTSENDTOCHAN( IRCEventEnvelope& event );
        void handle_ERR_TOOMANYCHANNELS( IRCEventEnvelope& event );
        void handle_ERR_WASNOSUCHNICK( IRCEventEnvelope& event );
        void handle_ERR_NOORIGIN( IRCEventEnvelope& event );
        void handle_ERR_NORECIPIENT( IRCEventEnvelope& event );
        void handle_ERR_NOTEXTTOSEND( IRCEventEnvelope& event );
        void handle_ERR_INPUTTOOLONG( IRCEventEnvelope& event );
        void handle_ERR_UNKNOWNCOMMAND( IRCEventEnvelope& event );
        void handle_ERR_NOMOTD( IRCEventEnvelope& event );
        void handle_ERR_NONICKNAMEGIVEN( IRCEventEnvelope& event );
        void handle_ERR_ERRONEUSNICKNAME( IRCEventEnvelope& event );
        void handle_ERR_NICKNAMEINUSE( IRCEventEnvelope& event );
        void handle_ERR_NICKCOLLISION( IRCEventEnvelope& event );
        void handle_ERR_USERNOTINCHANNEL( IRCEventEnvelope& event );
        void handle_ERR_NOTONCHANNEL( IRCEventEnvelope& event );
        void handle_ERR_USERONCHANNEL( IRCEventEnvelope& event );
        void handle_ERR_NOTREGISTERED( IRCEventEnvelope& event );
        void handle_ERR_NEEDMOREPARAMS( IRCEventEnvelope& event );
        void handle_ERR_ALREADYREGISTERED( IRCEventEnvelope& event );
        void handle_ERR_PASSWDMISMATCH( IRCEventEnvelope& event );
        void handle_ERR_YOUREBANNEDCREEP( IRCEventEnvelope& event );
        void handle_ERR_CHANNELISFULL( IRCEventEnvelope& event );
        void handle_ERR_UNKNOWNMODE( IRCEventEnvelope& event );
        void handle_ERR_INVITEONLYCHAN( IRCEventEnvelope& event );
        void handle_ERR_BANNEDFROMCHAN( IRCEventEnvelope& event );
        void handle_ERR_BADCHANNELKEY( IRCEventEnvelope& event );
        void handle_ERR_BADCHANMASK( IRCEventEnvelope& event );
        void handle_ERR_NOPRIVILEGES( IRCEventEnvelope& event );
        void handle_ERR_CHANOPRIVSNEEDED( IRCEventEnvelope& event );
        void handle_ERR_CANTKILLSERVER( IRCEventEnvelope& event );
        void handle_ERR_NOOPERHOST( IRCEventEnvelope& event );
        void handle_ERR_UMODEUNKNOWNFLAG( IRCEventEnvelope& event );
        void handle_ERR_USERSDONTMATCH( IRCEventEnvelope& event );
        void handle_ERR_HELPNOTFOUND( IRCEventEnvelope& event );
        void handle_ERR_INVALIDKEY( IRCEventEnvelope& event );
        void handle_RPL_STARTTLS( IRCEventEnvelope& event );
        void handle_RPL_WHOISSECURE( IRCEventEnvelope& event );
        void handle_ERR_STARTTLS( IRCEventEnvelope& event );
        void handle_ERR_INVALIDMODEPARAM( IRCEventEnvelope& event );
        void handle_RPL_HELPSTART( IRCEventEnvelope& event );
        void handle_RPL_HELPTXT( IRCEventEnvelope& event );
        void handle_RPL_ENDOFHELP( IRCEventEnvelope& event );
        void handle_ERR_NOPRIVS( IRCEventEnvelope& event );
        void handle_RPL_LOGGEDIN( IRCEventEnvelope& event );
        void handle_RPL_LOGGEDOUT(  IRCEventEnvelope& event );
        void handle_ERR_NICKLOCKED( IRCEventEnvelope& event );
        void handle_RPL_SASLSUCCESS( IRCEventEnvelope& event );
        void handle_ERR_SASLFAIL(  IRCEventEnvelope& event );
        void handle_ERR_SASLTOOLONG(  IRCEventEnvelope& event );
        void handle_ERR_SASLABORTED(  IRCEventEnvelope& event );
        void handle_ERR_SASLALREADY(  IRCEventEnvelope& event );
        void handle_RPL_SASLMECHS(  IRCEventEnvelope& event );
        void handle_RPL_STATSDLINE(  IRCEventEnvelope& event );

        void handle_ctcp_version( IRCEventEnvelope& event);
        void handle_ctcp_time(  IRCEventEnvelope& event );
        void handle_ctcp_ping(  IRCEventEnvelope& event );
        void handle_ctcp( IRCEventEnvelope& event );

        // sasl handlers
        void handle_S_RPL_CAP( IRCEventEnvelope& event );
        void handle_S_RPL_CAP_LS( IRCEventEnvelope& event );
        void handle_S_RPL_CAP_ACK( IRCEventEnvelope& event );
        void handle_AUTHENTICATE( IRCEventEnvelope& event );

        void report_event( IRCEventEnvelope& event );

        // control server
        std::string server_;

        // control server port
        std::string port_;

        // control server sender
        std::string nick_;

        // control server connection socket
        int socket_;

        // thread management
        std::queue<IRCEventEnvelope> orientation_queue_;
        std::mutex orientation_mutex_;
        std::condition_variable orientation_cond_;

        std::queue<IRCActionEnvelope> action_queue_;
        std::mutex action_mutex_;
        std::condition_variable action_cond_;

        // password to use for orchestration of the splinterIRC client
        std::string password_;

        // whether SASL auth is desired
        bool use_sasl_;

        // whether SASL auth was successful
        bool sasl_authenticated_ = false;

        // the username and password to use for SASL auth
        std::string sasl_username_;
        std::string sasl_password_;

        // to_json debugging info
        bool verbose_;

        // flag that tells event loops to prepare to exit
        std::atomic<bool> critical_thread_failed{false};

        bool command_authenticated_ = false;
        std::string owner_;

        // the some features of the IRC protocol are stateful, so we need to
        // keep track of what state we're in for some of the handlers
        enum State {
            // initiating a connection to control
            CLIENT_CONNECTING,
            // business as usual once connected
            CLIENT_CONNECTED,
            // disconnecting from control
            CLIENT_DISCONNECTING,
            // disconnected from control
            CLIENT_DISCONNECTED,
            // authenticating with SASL
            SASL_AUTHENTICATING,
            // joining a channel
            CHANNEL_JOINING,
            // retrieving capabilities
            CAPABILITIES_GETTING
        };

        // initial state is disconnected
        int state_ = State::CLIENT_DISCONNECTED;
};




#endif //SPLINTERIRC_CLIENT_H
