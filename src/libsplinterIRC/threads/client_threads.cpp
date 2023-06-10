#include "../client/client.h"

void splinterClient::observation_loop()
{
    // reads raw server input delimited by newline
    // and processes the raw irc strings into IRCEvent objects
    // and then queues them for the processing loop
    while ( !critical_thread_failed )
    {
        char buf[BUFSIZ];
        ssize_t n = recv(socket_, buf, sizeof(buf), 0);
        if (n == -1)
        {
            std::cerr << "Failed to receive message from server." << std::endl;
            critical_thread_failed = true;
            return;
            //throw std::runtime_error("Failed to receive message from server");
        }
        if (n == 0)
        {
            break;
        }
        std::string line(buf, n);
        // Split the line string into separate lines
        std::istringstream lines(line);
        std::string event_line;
        while (std::getline(lines, event_line))
        {
            // Create a new IRCEvent object for each line
            std::unique_lock<std::mutex> lock(mutex_);
            event_queue_.push(IRCEvent{event_line, server_});
            cond_.notify_one();
        }
    }
}

void splinterClient::processing_loop()
{
    // reads from the event queue and sends the events into the orientation loop
    // the orientation loop is where the event handlers are called
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


void splinterClient::run()
{
    std::thread ingestion_thread(&splinterClient::observation_loop, this);
    std::thread processing_thread(&splinterClient::processing_loop, this);
    ingestion_thread.join();
    processing_thread.join();
}