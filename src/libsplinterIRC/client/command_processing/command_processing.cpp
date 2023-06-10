#include "../client.h"

bool splinterClient::password_is_valid( const std::string& command )
{
    std::istringstream ss(command);
    std::string token;
    while (ss >> token);
    bool result = (token == password_);

    return result;
}

void splinterClient::handle_command( const IRCEvent& event )
{
    std::string message = event.message();
    if ( message[0] != '!' )
    {
        // it's not a command, ignore it.
        return;
    }

    std::string command = message.substr(1);

    if (! password_is_valid( command ))
    {
        // password is invalid, ignore it.
        return;
    }

    if (command.find("help") == 0)
    {
        help_prompt( event.nick(), command );
    }

    if (command.find("quit") == 0)
    {
        send("QUIT :Bye!\r\n");
        return;
    }

    if (command.find("create") == 0)
    {
        // Parse the command arguments
        std::istringstream ss(command);
        std::string token;
        ss >> token; // Skip the "create" command
        std::string server, port, nick, channel;
        if (ss >> server >> port >> nick >> channel)
        {
            // Create a new instance of IRCClient
            auto client = std::make_shared<splinterClient>(server, port, nick, password_);
            // Spin up the new instance of IRCClient in a new thread
            std::thread([client]
                        {
                            client->connect();
                            client->spawn_io_loop();
                        }).detach();
            // Store the new instance of IRCClient in the clients_ member variable using the next available ID
            clients_[std::to_string(next_id_++)] = client;
        } else {
            send_private_message( event.nick(), "Usage: !create <server> <port> <nick> <password>" );
        }
    }

    if (command.find("list") == 0)
    {
        list_clients( event.nick() );
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
            destroy_client( event.nick(), id );
        } else {
            send_private_message( event.nick(), "Usage: !destroy <id>" );
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
            if ( id == "0" )
            {
                join_channel( channel);
                send_private_message( event.nick(), "0: joined channel " + channel );
                return;
            }
            // Check if a client with the provided identifier exists
            auto it = clients_.find(id);
            if (it != clients_.end())
            {
                // Send the JOIN command to the specified client
                it->second->join_channel(channel);
            } else {
                send_private_message( event.nick(), "No client with id '" + id + "' found" );
            }
        } else {
            send_private_message( event.nick(), "Usage: !join <id> <channel>" );
        }
    }

    if (command.find("say") == 0)
    {
        // Parse the command arguments
        std::istringstream ss(command);
        std::string token;
        ss >> token; // Skip the "join" command
        std::string id, channel, message;
        if (ss >> id >> channel >> message)
        {
            if ( id == "0" )
            {
                send_private_message( channel, message );
                return;
            }
            // Check if a client with the provided identifier exists
            auto it = clients_.find(id);
            if (it != clients_.end())
            {
                // Send the JOIN command to the specified client
                it->second->send_private_message( channel, message );
            } else {
                send_private_message( event.nick(), "No client with id '" + id + "' found" );
            }
        } else {
            send_private_message( event.nick(), "Usage: !say <id> <channel|nick>" );
        }
    }
}

void splinterClient::help_prompt( std::string reply_to, std::string& command )
{
    send_private_message( reply_to, "Commands: !help, !quit, !create, !list, !destroy !join !say" );
}

void splinterClient::list_clients( const std::string& reply_to )
{
    send_private_message( reply_to, "'0': '" + server_ + "' (Me)");
    for (const auto& client_pair : clients_) {
        const auto& id = client_pair.first;
        const auto& client = client_pair.second;
        send_private_message( reply_to, "'" + id + "': '" + client->get_server() + "'");
    }
}

void splinterClient::destroy_client( const std::string& reply_to, const std::string& id )
{
    if (id == "0")
    {
        send_private_message( reply_to, "Nodes can only kill child nodes, not themselves.");
        return;
    }
    // Check if a client with the provided identifier exists
    auto it = clients_.find(id);
    if (it != clients_.end())
    {
        // Send a QUIT command to the client
        it->second->send("QUIT :Bye!\r\n");
        send_private_message( reply_to, "Client with id '" + id + "' disconnected." );
        // Remove the client from the clients_ member variable
        clients_.erase(it);
        send_private_message( reply_to, "Client with id '" + id + "' removed.");
    } else {
        send_private_message( reply_to, "No client with id '" + id + "' found" );
        std::cerr << "No client with id '" << id << "' found" << std::endl;
    }
}