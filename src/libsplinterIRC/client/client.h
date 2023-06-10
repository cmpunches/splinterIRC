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

// the client that will be controlled by commands via PM on the control server
class splinterClient {
    public:
        splinterClient(const std::string& server, const std::string& port, const std::string& nick, const std::string& password );
        ~splinterClient();

        void connect();
        void join_channel( std::string channel );

        void run();

    private:
        void send(const std::string& message);

        void observation_loop();
        void decision_loop(const IRCEvent& event );
        void processing_loop();

        // EVENT HANDLERS
        // These are tied to the decision_loop() function.
        // They are called when an event is received if mapped to that event type.
        void handle_ping(const IRCEvent& event);
        void handle_channel_message(const IRCEvent& event);
        void handle_private_message(const IRCEvent& event);
        const std::string& get_server() const;
        void list_clients( const std::string& reply_to );
        void destroy_client( const std::string& reply_to, const std::string& id );
        void send_private_message( std::string nick, std::string message );
        void help_prompt( std::string reply_to, std::string& command );
        bool password_is_valid( const std::string& command );
        void handle_command( const IRCEvent& event );
        void handle_join( const IRCEvent& event );
        void handle_part( const IRCEvent& event );
        void handle_quit( const IRCEvent& event );
        void handle_nick( const IRCEvent& event );
        void handle_mode( const IRCEvent& event );
        void handle_topic( const IRCEvent& event );
        void handle_kick( const IRCEvent& event );
        void handle_invite( const IRCEvent& event );
        void handle_notice( const IRCEvent& event );
        void handle_privmsg( const IRCEvent& event );
        void handle_welcome( const IRCEvent& event );
        void handle_error( const IRCEvent& event );
        void handle_motd( const IRCEvent& event );
        void handle_rpl_welcome( const IRCEvent& event );
        void handle_rpl_yourhost( const IRCEvent& event );
        void handle_rpl_created( const IRCEvent& event );
        void handle_rpl_myinfo( const IRCEvent& event );
        void handle_rpl_isupport( const IRCEvent& event );

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
