#include "../client/client.h"

// checks if the supplied username is the owner
bool splinterClient::is_owner( const std::string &username )
{
    return username == owner_;
}

void splinterClient::set_owner(const std::string &username) {
    owner_ = username;
}

// checks owner
// checks if authenticated
bool splinterClient::has_valid_session( std::string& sender )
{
    if ( is_owner( sender ) )
    {
        if ( command_authenticated_ )
        {
            // success
            return true;
        } else {
            // silently fail
            return false;
        }
    } else {
        // send_private_message( sender, "You are no Hamato Yoshi." );
        // silently fail
        return false;
    }
}

void splinterClient::handle_command( IRCEventEnvelope& event )
{
    std::string message = event.get_scalar_attribute( "message" );
    std::string sender = event.get_scalar_attribute( "nick" );

    if ( message[0] != '!' )
    {
        if ( event.get_scalar_attribute("target") != nick_ )
        {
            // if it's not a command, and it's not from Hamato Yoshi, ignore it in this context.
            return;
        }
        // if it's from Hamato Yoshi, and it's not a command, print help
        if ( has_valid_session( sender ) )
        {
            send_private_message( sender, "I don't understand that command." );
            prompt_help_general( sender );
        }
        // it's not a command, ignore it.
        return;
    }

    std::string command = message.substr(1);

    // give the user an opportunity to identify
    handle_command_authentication( sender, command );
    // see if that was successful
    if ( ! has_valid_session( sender ) )
    {
        // if not, return silently
        return;
    }

    handle_command_help(        sender, command );
    handle_command_quit(        sender, command );
    handle_command_splinter(    sender, command );
    handle_command_list(        sender, command );
    handle_command_destroy(     sender, command );
    handle_command_say(         sender, command );
    handle_command_raw(         sender, command );
    handle_command_pipe(        sender, command );
    handle_command_channel(     sender, command );
}

void splinterClient::handle_command_channel( std::string& sender, std::string& command )
{
    std::string first_word =   get_word( 1, command );
    if ( !( first_word == "channel" ) )
    {
        // not a channel command, return.
        return;
    }

    std::string client_id = get_word( 2, command );

    if ( client_id.empty() )
    {
        send_private_message( sender, "You must specify a client to send the command to." );
        return;
    }

    std::string subcommand =  get_after_word( 2, command );

    if ( client_id == "*" )
    {
        for ( auto it = clients_.begin(); it != clients_.end(); ++it )
        {
            handle_subcommand_channel( it->second, sender, subcommand );
        }
        return;
    }

    auto it = clients_.find( client_id );
    if ( it == clients_.end() )
    {
        send_private_message( sender, "No client with '" + client_id + "' found." );
        return;
    }
    handle_subcommand_channel( it->second, sender, subcommand );
}

void splinterClient::handle_subcommand_channel( std::shared_ptr<splinterClient> client, std::string& sender, std::string& subcommand )
{
    std::string first_word =   get_word( 1, subcommand );

    if ( first_word == "join" )
    {
        std::string channel = get_word( 2, subcommand );
        if ( channel.empty() || channel[0] != '#' )
        {
            send_private_message( sender, "You must specify a channel to join." );
            return;
        }
        client->join_channel( channel );
        return;
    }

    if ( first_word == "part" )
    {
        std::string channel = get_word( 2, subcommand );
        if ( channel.empty() || channel[0] != '#' )
        {
            send_private_message( sender, "You must specify a channel to part." );
            return;
        }
        client->part_channel( channel );
        return;
    }

    if ( first_word == "list" )
    {
        for ( auto channel : client->current_channels )
        {
            send_private_message( sender, std::to_string( client->splinter_id_ ) + ": " + client->server_ + "/" + channel );
        }
        return;
    }
}

// authenticates a user
// only one authenticated user at a time.  that user owns the bot.
// will create the conditions for a session if the password is valid
void splinterClient::handle_command_authentication( std::string& sender, std::string& command ) {
    std::string first_word =    get_word( 1, command );

    // auth <login|logout> [<password>]

    if ( !( first_word == "auth" ) )
    {
        // not an authentication command, return.
        return;
    }

    std::string second_word =   get_word( 2, command );

    // check if login was supplied for the subcommand
    if ( second_word == "login" )
    {
        std::string supplied_password = get_word( 3, command );
        if ( password_is_valid( supplied_password ) )
        {
            // password is valid, create a session
            command_authenticated_ = true;
            send_private_message( sender, "Password accepted." );
            set_owner( sender );
            send_private_message( sender, "You are now the owner of this splinter." );
            return;
        } else {
            // password is invalid, do not authenticate the command
            // send_private_message( sender, "Password rejected." );
            // silently return
            return;
        }
        // if we get here, something went wrong
        return;
    }

    // check if logout was supplied for the subcommand
    if ( second_word == "logout" )
    {
        if ( has_valid_session( sender ) )
        {
            command_authenticated_ = false;
            owner_ = "";
            send_private_message( sender, "You are no longer the owner of this splinter." );
            return;
        } else {
            return;
        }
        // if we get here, something went wrong
        return;
    }
}

void splinterClient::handle_command_help(std::string& sender, std::string& command )
{
    std::string first_word = get_word( 1, command );

    // help [<command>]

    if ( !( first_word == "help" ) )
    {
        // not a help command, return.
        return;
    }

    std::string subcommand = get_word(2, command );

    if (subcommand == "quit" )
    {
        prompt_help_quit(sender);
        return;
    }

    if (subcommand == "auth" )
    {
        prompt_help_auth(sender);
        return;
    }

    if (subcommand == "splinter" )
    {
        prompt_help_splinter(sender);
        return;
    }

    if (subcommand == "list" )
    {
        prompt_help_list(sender);
        return;
    }

    if (subcommand == "destroy" )
    {
        prompt_help_destroy(sender);
        return;
    }

    if (subcommand == "channel" )
    {
        prompt_help_channel(sender);
        return;
    }

    if (subcommand == "say" )
    {
        prompt_help_say(sender);
        return;
    }

    if (subcommand == "raw" )
    {
        prompt_help_raw(sender);
        return;
    }

    if (subcommand == "pipe" )
    {
        prompt_help_pipe(sender);
        return;
    }

    // check if a command was supplied for the subcommand
    if (! subcommand.empty() )
    {
        send_private_message( sender, "I don't know how to help with that." );
        return;
    }

    // if we get here, we're just asking for general help
    prompt_help_general(sender);
}

void splinterClient::handle_command_quit(std::string& sender, std::string& command )
{
    std::string subcommand = get_word(1, command );

    // quit [<message>]

    if ( !(subcommand == "quit" ) )
    {
        // not a quit command, return.
        return;
    }

    std::string quit_message = get_after_word(1, command );

    // check if a message was supplied for the subcommand
    if ( ! quit_message.empty() )
    {
        quit(quit_message);
        return;
    } else {
        quit("Bye!");
    }
}

void splinterClient::handle_command_splinter(std::string& sender, std::string& command )
{
    // TODO: This syntax is garbage.  Clean this up.
    bool use_sslb;
    bool use_strict_sslb;

    std::string first_word = get_word( 1, command );

    // splinter <server> <port> <nick> <splinter_password> <use_ssl> <strict_ssl> <use_sasl> [<sasl_username> <sasl_password>]

    if ( !( first_word == "splinter" ) )
    {
        // not a splinter command, return.
        return;
    }

    std::string server = get_word( 2, command );
    std::string port = get_word( 3, command );
    std::string nick = get_word( 4, command );
    std::string child_pass = get_word( 5, command );
    std::string use_ssl = get_word( 6, command );
    std::string use_strict_ssl = get_word( 7, command );
    std::string optional_use_sasl = get_word( 8, command );
    std::string optional_sasl_username = get_word( 9, command );
    std::string optional_sasl_password = get_word( 10, command );
    bool client_uses_sasl = false;

    // check if command arguments were supplied correctly
    if ( server.empty() ) { return; }
    if ( port.empty() ) { return; }
    if ( nick.empty() ) { return; }
    if ( child_pass.empty() ) { return; }
    if ( use_ssl.empty() ) { return; }
    if ( use_strict_ssl.empty() ) { return; }

    if ( use_ssl == "true" )
    {
        use_sslb = true;
    }

    if ( use_ssl == "false" )
    {
        use_sslb = false;
    }

    if ( use_strict_ssl == "true" )
    {
        use_strict_sslb = true;
    }

    if ( use_strict_ssl == "false" )
    {
        use_strict_sslb = false;
    }

    if (! optional_use_sasl.empty() )
    {
        if ( optional_use_sasl == "true" )
        {
            client_uses_sasl = true;
            if ( optional_sasl_username.empty() )
            {
                send_private_message(sender, "You must supply a SASL username if you want to use SASL." );
                prompt_help_splinter(sender);
                return;
            }
            if ( optional_sasl_password.empty() )
            {
                send_private_message(sender, "You must supply a SASL password if you want to use SASL." );
                prompt_help_splinter(sender);
                return;
            }
        }
    }

    send_private_message( sender, "Creating a new splinter client..." );

    // Create a new instance of IRCClient
    auto client = std::make_shared<splinterClient>(
            server,
            port,
            nick,
            child_pass,
            client_uses_sasl,
            optional_sasl_username,
            optional_sasl_password,
            use_sslb,
            use_strict_sslb,
            verbose_
    );

    // Spin up the new instance of IRCClient in a new thread
    std::thread([client] {
        client->connect();
        client->add_to_clients();
        client->run_event_loop();
    }).detach();

    send_private_message( sender, "New splinter spawned." );
    notify_owner( std::to_string(client->get_id()) + ": " + client->get_nick() + "@" + client->get_server() );
}

void splinterClient::handle_command_list( std::string& sender, std::string& command )
{
    std::string first_word = get_word( 1, command );

    if ( first_word == "list" )
    {
        list_clients( sender );
    } else {
        return;
    }
}

void splinterClient::handle_command_destroy( std::string& sender, std::string& command )
{
    // destroy <id>
    std::string first_word = get_word( 1, command );

    if ( first_word == "destroy" )
    {
        std::string second_word = get_word( 2, command );

        if ( second_word.empty() )
        {
            prompt_help_destroy(sender);
            return;
        } else {
            if (! is_num_or_any( second_word ))
            {
                prompt_help_destroy(sender);
                return;
            }
        }

        if ( second_word == "*" )
        {
            for (const auto& client_pair : clients_) {
                const auto& id = client_pair.first;
                const auto& client = client_pair.second;
                if ( client->get_id() == 0 ) { continue; }
                destroy_client(sender, id);
            }
            return;
        } else {
            destroy_client(sender, second_word );
        }
    } else {
        return;
    }
}

void splinterClient::handle_command_say( std::string& sender, std::string& command )
{
    // !say <id> <target> <message>
    std::string subcommand = get_word( 1, command );

    if ( subcommand == "say" )
    {
        std::string id = get_word( 2, command );
        if ( id.empty() )
        {
            prompt_help_say(sender);
            return;
        } else {
            if (! is_num_or_any( id ))
            {
                prompt_help_say(sender);
                return;
            }
            std::string target = get_word( 3, command );
            if ( target.empty() )
            {
                prompt_help_say(sender);
                return;
            }

            std::string mes = get_after_word( 3, command );
            if ( mes.empty() )
            {
                prompt_help_say(sender);
                return;
            }

            if ( id == "*" )
            {
                for (const auto& client_pair : clients_) {
                    const auto& this_id = client_pair.first;
                    const auto& client = client_pair.second;
                    if ( client->get_id() == 0 ) { continue; }
                    client->send_private_message( target, mes );
                    send_private_message(sender, client->server_ + "/" + target + "/" + std::to_string(client->get_id()) + ": " + mes );
                }
                return;
            } else {
                // Check if a client with the provided identifier exists
                auto it = clients_.find(id);
                if (it != clients_.end())
                {
                    // Send the SAY command to the specified client
                    it->second->send_private_message( target, mes );
                    send_private_message(sender, it->second->server_ + "/" + target + "/" + std::to_string(it->second->get_id()) + ": " + mes );
                } else {
                    send_private_message(sender, "No client with id '" + id + "' found" );
                }
            }
        }
    } else {
        return;
    }
}

void splinterClient::handle_command_raw( std::string& sender, std::string& command )
{
    // !raw <id> <raw IRC command>
    std::string subcommand = get_word(1, command );

    if (subcommand == "raw" )
    {
        std::string id = get_word(2, command );
        if ( id.empty() )
        {
            prompt_help_raw(sender);
            return;
        } else {
            if (! is_num_or_any( id ))
            {
                prompt_help_raw(sender);
                return;
            }
            std::string raw = get_after_word(2, command );
            if ( raw.empty() )
            {
                prompt_help_raw(sender);
                return;
            }

            if ( id == "*" )
            {
                for (const auto& client_pair : clients_) {
                    const auto& this_id = client_pair.first;
                    const auto& client = client_pair.second;
                    if ( client->get_id() == 0 ) { continue; }
                    IRCActionEnvelope action = IRCActionEnvelope(stoi(this_id), raw);
                    enqueue_action(action);

                    send_private_message(sender,
                                         client->server_ + "/" + std::to_string(client->get_id()) +
                                         ": (raw)" + raw);
                }
                return;
            } else {

                // Check if a client with the provided identifier exists
                auto it = clients_.find(id);
                if (it != clients_.end()) {
                    // Send the raw command to the specified client
                    IRCActionEnvelope action = IRCActionEnvelope(stoi(id), raw);
                    enqueue_action(action);

                    send_private_message(sender,
                                         it->second->server_ + "/" + std::to_string(it->second->get_id()) +
                                         ": (raw)" + raw);
                } else {
                    send_private_message(sender, "No client with id '" + id + "' found");
                }
            }
        }
    } else {
        return;
    }
}

void splinterClient::handle_command_pipe( std::string& sender, std::string& command )
{
    // !pipe <id> <target> <message>
    std::string first_word = get_word(1, command );

    if ( first_word != "pipe" )
    {
        return;
    }

    std::string subcommand = get_word(2, command );

    if ( subcommand == "create" )
    {
        // !pipe create <sender_id> <sender_source> <target_id> <target_source> <preserve_sender>
        std::string sender_id = get_word(3, command );
        std::string sender_source = get_word(4, command );

        std::string target_id = get_word(5, command );
        std::string target_source = get_word(6, command );

        bool preserve_sender_bool = true;
        std::string preserve_sender = get_word(7, command );

        if ( sender_id.empty() || sender_source.empty() || target_id.empty() || target_source.empty() || preserve_sender.empty() )
        {
            prompt_help_pipe(sender);
            return;
        }

        if ( preserve_sender == "true" )
        {
            preserve_sender_bool = true;
        }
        if ( preserve_sender == "false" )
        {
            preserve_sender_bool = false;
        }

        pipe_add( sender, std::stoi(sender_id), sender_source, std::stoi(target_id), target_source, preserve_sender_bool );
    }

    if ( subcommand == "list" )
    {
        pipe_list( sender );
    }

    if ( subcommand == "destroy" )
    {
        std::string pipeID = get_word(3, command );
        if ( pipeID.empty() )
        {
            prompt_help_pipe(sender);
            return;
        } else {
            if ( pipeID == "*" )
            {
                for (const auto& pipe_pair : pipes_ ) {
                    const auto& this_id = pipe_pair.first;
                    const auto& pipe = pipe_pair.second;
                    pipe_destroy( sender, std::stoi(this_id) );
                }
                return;
            } else {
                pipe_destroy( sender, std::stoi(pipeID) );
            }
        }
    }
}

// Method to add an entry to the shared property
void splinterClient::pipe_add(std::string reply_to, int source_splinter, const std::string& source_name, int target_splinter, const std::string& target_name, bool preserve_sender )
{
    // Generate a new pipeID by incrementing the size of the map
    std::string pipeID = std::to_string(++last_pipe_id_);
    pipes_[pipeID] = std::make_tuple(source_splinter, source_name, target_splinter, target_name, preserve_sender);
    send_private_message( reply_to, pipeID + ": "
        + std::to_string(source_splinter) + "/" + source_name
        + " -> " + std::to_string(target_splinter)
        + "/" + target_name );
}

// Method to print the shared property
void splinterClient::pipe_list( std::string reply_to )
{
    for (const auto& entry : pipes_)
    {
        send_private_message(reply_to, entry.first + ": "
                  + std::to_string(std::get<0>(entry.second))
                  + "/" + std::get<1>(entry.second)
                  + " -> " + std::to_string(std::get<2>(entry.second))
                  + "/" + std::get<3>(entry.second) );
    }
}

void splinterClient::pipe_destroy(std::string reply_to, int pipeID)
{
    pipes_.erase(std::to_string(pipeID));
    send_private_message(reply_to, "Pipe ID '" + std::to_string(pipeID) + "' destroyed");
}



void splinterClient::list_clients( const std::string& reply_to )
{
    for ( const auto& client_pair : clients_ ) {
        const auto& id = client_pair.first;
        const auto& client = client_pair.second;
        if ( client->splinter_id_ == splinter_id_ )
        {
            send_private_message( reply_to, "'" + id + "': '" + client->get_nick() + "@"  + client->get_server() + "' (ME)" );
        } else {
            send_private_message( reply_to, "'" + id + "': '" + client->get_nick()+ "@" + client->get_server() + "'" );
        }
    }
}

void splinterClient::destroy_client( const std::string& reply_to, const std::string& id )
{
    // Check if a client with the provided identifier exists
    auto it = clients_.find(id);
    if (it != clients_.end())
    {
        // TODO: add pipeline for quit message
        // Send a QUIT command to the client
        it->second->quit("Bye!");
        it->second->set_to_fail();
        send_private_message( reply_to, "Client with id '" + id + "' disconnected." );
        notify_owner( "Client with id '" + id + "' disconnected.");

        // Remove the client from the clients_ member variable
        clients_.erase(it);
        send_private_message( reply_to, "Client with id '" + id + "' removed.");
    } else {
        send_private_message( reply_to, "No client with id '" + id + "' found" );
        std::cerr << "No client with id '" << id << "' found" << std::endl;
    }
}

// TODO: add a command to list channels the splinter is in
// TODO: add a command to part a channel
// TODO: add a command to change topic on a channel
// TODO: add support for sending an event to an AMQP exchange or to a FIFO for dedicated component