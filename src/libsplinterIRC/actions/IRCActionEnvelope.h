#ifndef SPLINTERIRC_IRCACTIONENVELOPE_H
#define SPLINTERIRC_IRCACTIONENVELOPE_H

#include <string>

class IRCActionEnvelope {
    public:
        // the raw string to send to the server
        std::string action;

        // the client that ought to take the action
        // should align with the client_id of the client intented to take the action
        // *not* necessarily the client that received the event it is responding to
        const int actor_id;

        // basic constructor
        // nothing terribly complex here
        IRCActionEnvelope( const int client_id, std::string action );
};

#endif //SPLINTERIRC_IRCACTIONENVELOPE_H
