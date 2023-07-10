#include "../client/client.h"

void splinterClient::observation_loop()
{
    std::string buffer;
    while (!critical_thread_failed)
    {
        char buf[BUFSIZ];
        ssize_t n;

        // Use select() to check if data is available to be read
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(socket_, &readfds);
        struct timeval timeout;
        timeout.tv_sec = 1; // 1 second timeout
        timeout.tv_usec = 0;
        int ready = select(socket_ + 1, &readfds, nullptr, nullptr, &timeout);

        if (ready == -1)
        {
            std::cerr << "select() failed" << std::endl;
            set_to_fail();
            return;
        } else if (ready == 0) {
            // No data available to be read, continue to next iteration
            continue;
        }

        if ( use_ssl_ )
        {
            // use SSL
            n = SSL_read( ssl_, buf, sizeof( buf ) );
            if (n <= 0)
            {
                int err = SSL_get_error( ssl_, n );
                if ( err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE )
                {
                    continue;
                }
                std::cerr << "Failed to receive message from SSL-socket." << std::endl;
                unsigned long err_code = ERR_get_error();
                std::cerr << "SSL error: " << ERR_error_string( err_code, nullptr ) << std::endl;
                set_to_fail();
                return;
            }
        } else {
            // use plaintext
            n = recv( socket_, buf, sizeof( buf ), 0 );
            if ( n == -1 )
            {
                std::cerr << "Failed to receive message from plaintext-socket." << std::endl;
                set_to_fail();
                return;
            }
            if ( n == 0 )
            {
                break;
            }
        }
        buffer.append( buf, n );
        // Remove solitary \n characters
        buffer.erase( std::remove( buffer.begin(), buffer.end(), '\n' ), buffer.end() );

        // Find the last complete line in the buffer
        size_t pos = buffer.rfind( '\r' );
        if ( pos != std::string::npos )
        {
            // Split the buffer into complete lines and an incomplete line
            std::string lines = buffer.substr( 0, pos );
            buffer = buffer.substr( pos + 1 );

            // Process the complete lines
            std::istringstream stream( lines );
            std::string event_line;

            while ( std::getline( stream, event_line, '\r' ) )
            {
                enqueue_event( IRCEventEnvelope{ event_line, server_, std::to_string( splinter_id_ ) } );
            }
        }
    }
}

void splinterClient::orientation_loop()
{
    while ( !critical_thread_failed )
    {
        bool event_received = false;
        IRCEventEnvelope event( "", "", std::to_string( splinter_id_ ) );
        {
            std::unique_lock<std::mutex> lock(orientation_mutex_);
            if (orientation_cond_.wait_for(lock, std::chrono::seconds(1), [this] { return !orientation_queue_.empty(); }))
            {
                event = orientation_queue_.front();
                orientation_queue_.pop();
                event_received = true;
            }
        }
        if ( event_received )
        {
            // only process the event if it is for this client
            if ( event.client_id_ == std::to_string( splinter_id_ ) )
            {
                make_decision(event);
            }
        }
    }
}

void splinterClient::action_loop() {
    while ( !critical_thread_failed )
    {
        IRCActionEnvelope action = [&] {
            std::unique_lock<std::mutex> lock( action_mutex_ );
            action_cond_.wait(lock, [this] { return !action_queue_.empty(); });
            IRCActionEnvelope action = action_queue_.front();
            action_queue_.pop();
            return action;
        }();
        execute_action(action);
    }
}

void splinterClient::run_event_loop()
{
    std::thread observation_thread( &splinterClient::observation_loop, this );
    std::thread orientation_thread( &splinterClient::orientation_loop, this );
    std::thread action_thread( &splinterClient::action_loop, this );
    observation_thread.join();
    orientation_thread.join();
    action_thread.join();
}

void splinterClient::enqueue_action( IRCActionEnvelope action )
{
    std::unique_lock<std::mutex> lock( action_mutex_ );
    action_queue_.push( action );
    action_cond_.notify_one();
}

void splinterClient::enqueue_event( IRCEventEnvelope event )
{
    std::unique_lock<std::mutex> lock( orientation_mutex_ );
    orientation_queue_.push( event );
    orientation_cond_.notify_one();
}

void splinterClient::execute_action( IRCActionEnvelope &action )
{
    usleep( 125000 );
    //std::cout << action.actor_id << ": Executing action: " << action.action << std::endl;
    auto it = clients_.find( std::to_string( action.actor_id ) );
    if ( it == clients_.end() )
    {
        std::cerr << "Action execution failed.  Failed to find client with id: " << action.actor_id << std::endl;
        std::cerr << "Action: " << action.action << std::endl;
        return;
    }

    if ( action.action == "DIE" )
    {
        it->second->set_to_fail();
        it->second->destroy_self();
        std::cerr << "Client " << action.actor_id << " has died." << std::endl;
        return;
    }

    it->second->send( action.action );
}
