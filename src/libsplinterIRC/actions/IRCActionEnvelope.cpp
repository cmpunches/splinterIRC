#include "IRCActionEnvelope.h"

IRCActionEnvelope::IRCActionEnvelope( const int client_id, std::string action ) :
    actor_id(client_id),
    action(action)
{}