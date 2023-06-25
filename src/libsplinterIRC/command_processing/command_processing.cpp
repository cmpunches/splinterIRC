#include "../client/client.h"

void splinterClient::handle_command( IRCEventEnvelope& event )
{
    // TODO rework this to authenticate as a command to start a session linked with username
    // TODO add command to send raw IRC commands

    std::string message = event.get_scalar_attribute( "message");
    std::string sender = event.get_scalar_attribute( "nick" );

    if ( message[0] != '!' )
    {
        // it's not a command, ignore it.
        return;
    }

    std::string command = message.substr(1);

    std::istringstream pass_filter(command);
    std::string passtoken;
    while (pass_filter >> passtoken);

    if (! password_is_valid( passtoken ))
    {
        // password is invalid, ignore it.
        return;
    }

    if (command.find("help") == 0)
    {
        help_prompt( sender, command );
    }

    if (command.find("quit") == 0)
    {
        // TODO parameterize this to take quit message
        quit("Bye!");
        return;
    }

    if (command.find("splinter") == 0)
    {
        // Parse the command arguments
        std::istringstream ss(command);
        std::string token;
        ss >> token; // Skip the "create" command
        std::string server, port, nick, channel;

        send_private_message( sender, "Creating a new splinter client..." );

        if (ss >> server >> port >> nick >> channel)
        {
            // Create a new instance of IRCClient
            auto client = std::make_shared<splinterClient>(server, port, nick, passtoken, use_sasl_, sasl_username_, sasl_password_, verbose_ );

            // Spin up the new instance of IRCClient in a new thread
            std::thread([client] {
                            client->connect();
                            client->add_to_clients();
                            client->run_event_loop();
                        }).detach();

            send_private_message(sender, "New splinter spawned." );
            send_private_message(sender, std::to_string(client->get_id()) + ": " + client->get_nick() + "@" + client->get_server() );

        } else {
            send_private_message(sender, "Usage: !splinter <server> <port> <sender> <password>" );
        }
    }

    if (command.find("list") == 0)
    {
        list_clients( sender );
    }

    if (command.find("destroy") == 0)
    {
        // Parse the command arguments
        std::istringstream ss(command);
        std::string token;
        ss >> token; // Skip the "destroy" command
        std::string id;
        if (ss >> id)
        {
            destroy_client(sender, id );
        } else {
            send_private_message(sender, "Usage: !destroy <id>" );
        }
    }

    if (command.find("join") == 0)
    {
        // Parse the command arguments
        std::istringstream ss(command);
        std::string token;
        ss >> token; // Skip the "join" command
        std::string id, channel;
        if (ss >> id >> channel)
        {
            // Check if a client with the provided identifier exists
            auto it = clients_.find(id);
            if (it != clients_.end())
            {
                // Send the JOIN command to the specified client
                it->second->join_channel(channel);
                send_private_message(sender, std::to_string(it->second->get_id()) + ": joined channel " + channel );
            } else {
                send_private_message(sender, "No client with id '" + id + "' found" );
            }
        } else {
            send_private_message(sender, "Usage: !join <id> <channel>" );
        }
    }

    if (command.find("say") == 0)
    {
        // Parse the command arguments
        std::istringstream ss(command);
        std::string token;
        ss >> token; // Skip the "join" command
        std::string id, channel, tmessage;
        if (ss >> id >> channel >> tmessage)
        {
            // Check if a client with the provided identifier exists
            auto it = clients_.find(id);
            if (it != clients_.end())
            {
                // Send the JOIN command to the specified client
                it->second->send_private_message( channel, tmessage );
            } else {
                send_private_message( sender, "No client with id '" + id + "' found" );
            }
        } else {
            send_private_message( sender, "Usage: !say <id> <channel|sender> <message>" );
        }
    }
}

void splinterClient::help_prompt( std::string reply_to, std::string& command )
{
    send_private_message( reply_to, "Commands: !help, !quit, !splinter, !list, !destroy !join !say" );
}

void splinterClient::list_clients( const std::string& reply_to )
{
    for (const auto& client_pair : clients_) {
        const auto& id = client_pair.first;
        const auto& client = client_pair.second;
        if ( client->splinter_id_ == splinter_id_ )
        {
            send_private_message( reply_to, "'" + id + "': '" + client->get_nick() + "@"  + client->get_server() + "' (ME)");
        } else {
            send_private_message( reply_to, "'" + id + "': '" + client->get_nick()+ "@" + client->get_server() + "'");
        }
    }
}

void splinterClient::destroy_client( const std::string& reply_to, const std::string& id )
{
    // Check if a client with the provided identifier exists
    auto it = clients_.find(id);
    if (it != clients_.end())
    {
        // Send a QUIT command to the client
        it->second->quit("Bye!");
        it->second->critical_thread_failed = true;
        send_private_message( reply_to, "Client with id '" + id + "' disconnected." );

        // Remove the client from the clients_ member variable
        clients_.erase(it);
        send_private_message( reply_to, "Client with id '" + id + "' removed.");
    } else {
        send_private_message( reply_to, "No client with id '" + id + "' found" );
        std::cerr << "No client with id '" << id << "' found" << std::endl;
    }
}
