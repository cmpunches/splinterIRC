#include "../client/client.h"

void splinterClient::observation_loop()
{
    // reads raw server input delimited by newline
    // and processes the raw irc strings into IRCEvent objects
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
                // Create a new IRCEvent object for each line
                std::unique_lock<std::mutex> lock(mutex_);

                // push it to the ingestion queue
                event_queue_.push(IRCEvent{event_line, server_});
                cond_.notify_one();
            }
        }
    }
}

void splinterClient::processing_loop()
{
    // reads from the event queue and sends the events into the orientation/decision loop
    // the orientation/decision loop is where the event handlers are mapped to actions
    while ( !critical_thread_failed )
    {
        IRCEvent event = [&] {
            std::unique_lock<std::mutex> lock(mutex_);
            cond_.wait(lock, [this] { return !event_queue_.empty(); });
            IRCEvent event = event_queue_.front();
            event_queue_.pop();
            return event;
        }();
        decision_loop(event);
    }
}

void splinterClient::run_event_loop()
{
    std::thread observation_thread(&splinterClient::observation_loop, this);
    std::thread processing_thread(&splinterClient::processing_loop, this);
    observation_thread.join();
    processing_thread.join();
}