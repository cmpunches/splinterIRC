#include "client.h"

splinterClient::splinterClient(const std::string& server, const std::string& port, const std::string& nick, const std::string& password ) :
        server_(server), port_(port), nick_(nick), password_(password), socket_(-1) {}

splinterClient::~splinterClient()
{
    if (socket_ != -1)
    {
        close(socket_);
    }
}

void splinterClient::connect()
{
    struct addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo* result;
    if (getaddrinfo(server_.c_str(), port_.c_str(), &hints, &result) != 0)
    {
        std::cerr << "Failed to resolve server address:  " << server_ << std::endl;
        critical_thread_failed = true;
        return;
    }
    for (struct addrinfo* rp = result; rp != nullptr; rp = rp->ai_next)
    {
        socket_ = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (socket_ == -1)
        {
            continue;
        }
        if (::connect(socket_, rp->ai_addr, rp->ai_addrlen) != -1)
        {
            break;
        }
        close(socket_);
        socket_ = -1;
    }
    freeaddrinfo(result);
    if (socket_ == -1) {
        std::cerr << "Failed to connect to server" << std::endl;
        critical_thread_failed = true;
        return;
    }
    send( "NICK " + nick_ + "\r\n" );
    send( "USER " + nick_ + " 0 * :" + nick_ + "\r\n" );
}

void splinterClient::join_channel( std::string channel )
{
    send( "JOIN " + channel + "\r\n" );
}

void splinterClient::spawn_io_loop()
{
    std::thread ingestion_thread(&splinterClient::ingestion_loop, this);
    std::thread processing_thread(&splinterClient::processing_loop, this);
    ingestion_thread.join();
    processing_thread.join();
}

void splinterClient::send( const std::string& message )
{
    if (::send(socket_, message.c_str(), message.size(), 0) == -1)
    {
        std::cerr << "Failed to send message to server" << std::endl;
        return;
        //throw std::runtime_error("Failed to send message to server");
    }
}

void splinterClient::ingestion_loop()
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
        orientation_loop(event);
    }
}

const std::string& splinterClient::get_server() const
{
    return server_;
}

void splinterClient::send_private_message( std::string nick, std::string message )
{
    send("PRIVMSG " + nick + " :" + message + "\r\n");
}

void splinterClient::orientation_loop(const IRCEvent& event)
{
    // Further process the event here
    //std::cout << "Received message: ";
    //std::cout << event.raw_message() << std::endl;

    // type mappings for actions and events
    switch ( event.type() )
    {
        // TODO add a handler to this for routing
        case IRCEvent::Type::UNKNOWN:
            //std::cout << "UNKNOWN: ";
            //std::cout << event.to_json() << std::endl;
            std::cerr << "Unknown message type: ";
            std::cerr << event.raw_message() << std::endl;
            break;

        case IRCEvent::Type::JOIN:
            handle_join(event);
            break;

        case IRCEvent::Type::PART:
            handle_part(event);
            break;

        case IRCEvent::Type::KICK:
            handle_kick(event);
            break;

        case IRCEvent::Type::NOTICE:
            handle_notice(event);
            break;

        case IRCEvent::Type::PING:
            handle_ping(event);
            break;

        case IRCEvent::Type::NICK:
            handle_nick(event);
            break;

        case IRCEvent::Type::RPL_UMODEIS:
            handle_mode(event);
            break;

        case IRCEvent::Type::QUIT:
            handle_quit(event);
            break;

        case IRCEvent::Type::TOPIC:
            handle_topic(event);
            break;

        case IRCEvent::Type::INVITE:
            handle_invite(event);
            break;

        case IRCEvent::Type::ERROR:
            handle_error(event);
            break;

        case IRCEvent::Type::S_CHANNEL_MESSAGE:
            handle_channel_message(event);
            break;

        case IRCEvent::Type::S_PRIVATE_MESSAGE:
            handle_private_message(event);
            break;

        case IRCEvent::Type::RPL_WELCOME:
            handle_rpl_welcome(event);
            break;

        case IRCEvent::Type::RPL_YOURHOST:
            handle_rpl_yourhost(event);
            break;

        case IRCEvent::Type::RPL_CREATED:
            handle_rpl_created(event);
            break;

        case IRCEvent::Type::RPL_MYINFO:
            handle_rpl_myinfo(event);
            break;

        case IRCEvent::Type::RPL_ISUPPORT:
            handle_rpl_isupport(event);
            break;

        case IRCEvent::Type::RPL_LUSERCLIENT:
            handle_motd(event);
            break;

        case IRCEvent::Type::RPL_LUSEROP:
            handle_motd(event);
            break;

        case IRCEvent::Type::RPL_LUSERUNKNOWN:
            handle_motd(event);
            break;

        case IRCEvent::Type::RPL_LUSERCHANNELS:
            handle_motd(event);
            break;

        case IRCEvent::Type::RPL_LUSERME:
            handle_motd(event);
            break;

        case IRCEvent::Type::RPL_LOCALUSERS:
            handle_motd(event);
            break;

        case IRCEvent::Type::RPL_GLOBALUSERS:
            handle_motd(event);
            break;

        case IRCEvent::Type::RPL_NAMREPLY:
            handle_motd(event);
            break;

        case IRCEvent::Type::RPL_ENDOFNAMES:
            handle_motd(event);
            break;

        case IRCEvent::Type::RPL_MOTD:
            handle_motd(event);
            break;

        case IRCEvent::Type::RPL_MOTDSTART:
            handle_motd(event);
            break;

        case IRCEvent::Type::RPL_ENDOFMOTD:
            handle_motd(event);
            break;

        case IRCEvent::Type::ERR_YOUREBANNEDCREEP:
            handle_motd(event);
            break;

        default:
            std::cout << "Unforeseen message type.  Report this message as a bug." << std::endl;
            break;
    }
    std::cout << event.to_json();
}