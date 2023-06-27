#include "../client/client.h"

void splinterClient::observation_loop()
{
    // reads raw server input delimited by newline
    // and processes the raw irc strings into IRCEventEnvelope objects
    // and then queues them for the processing loop
    std::string buffer;
    while ( !critical_thread_failed )
    {
        char buf[BUFSIZ];
        ssize_t n = recv(socket_, buf, sizeof(buf), 0);
        if (n == -1)
        {
            std::cerr << "Failed to receive message from server." << std::endl;
            critical_thread_failed = true;
            return;
        }
        if (n == 0)
        {
            break;
        }
        buffer.append(buf, n);
        // Remove solitary \n characters
        buffer.erase(std::remove(buffer.begin(), buffer.end(), '\n'), buffer.end());

        // Find the last complete line in the buffer
        size_t pos = buffer.rfind('\r');
        if (pos != std::string::npos)
        {
            // Split the buffer into complete lines and an incomplete line
            std::string lines = buffer.substr(0, pos);
            buffer = buffer.substr(pos + 1);

            // Process the complete lines
            std::istringstream stream(lines);
            std::string event_line;

            while (std::getline(stream, event_line, '\r'))
            {
                enqueue_event(IRCEventEnvelope{event_line, server_});
            }
        }
    }
}

void splinterClient::orientation_loop()
{
    // reads from the event queue and sends the events into the orientation/decision loop
    // the orientation/decision loop is where the event handlers are mapped to actions
    while ( !critical_thread_failed )
    {
        IRCEventEnvelope event = [&] {
            std::unique_lock<std::mutex> lock(orientation_mutex_);
            orientation_cond_.wait(lock, [this] { return !orientation_queue_.empty(); });
            IRCEventEnvelope event = orientation_queue_.front();
            orientation_queue_.pop();
            return event;
        }();
        // maps an event to a handler, which decides upon an action to take and enqueues
        // the action to be sent to the server
        make_decision(event);
    }
}

void splinterClient::action_loop()
{
    // reads from the action queue and sends the actions to the server
    while ( !critical_thread_failed )
    {
        IRCActionEnvelope action = [&] {
            std::unique_lock<std::mutex> lock(action_mutex_);
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
    std::thread observation_thread(&splinterClient::observation_loop, this);
    std::thread orientation_thread(&splinterClient::orientation_loop, this);
    std::thread action_thread(&splinterClient::action_loop, this);
    observation_thread.join();
    orientation_thread.join();
    action_thread.join();
}

void splinterClient::enqueue_action(IRCActionEnvelope action)
{
    std::unique_lock<std::mutex> lock(action_mutex_);
    action_queue_.push(action);
    action_cond_.notify_one();
}

void splinterClient::enqueue_event(IRCEventEnvelope event)
{
    std::unique_lock<std::mutex> lock(orientation_mutex_);
    orientation_queue_.push(event);
    orientation_cond_.notify_one();
}

void splinterClient::execute_action(IRCActionEnvelope &action)
{
    usleep(250000);
    //std::cout << action.actor_id << ": Executing action: " << action.action << std::endl;
    auto it = clients_.find(std::to_string( action.actor_id ));
    if (it == clients_.end())
    {
        std::cerr << "Action execution failed.  Failed to find client with id: " << action.actor_id << std::endl;
        return;
    }
    it->second->send(action.action);
}
