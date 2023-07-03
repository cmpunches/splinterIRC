#include "client.h"

std::map<std::string, std::shared_ptr<splinterClient>> splinterClient::clients_;

int splinterClient::get_next_id() {
    int next_id = -1;
    for (const auto& client_pair : clients_) {
        const auto& id = std::stoi(client_pair.first);
        if (id > next_id) {
            next_id = id;
        }
    }
    return next_id + 1;
}

// TODO: Readability: Introduce parameter objects to simplify the constructor
splinterClient::splinterClient(
        const std::string& server,
        const std::string& port,
        const std::string& nick,
        const std::string& password,
        const bool use_sasl,
        const std::string& sasl_username,
        const std::string& sasl_password,
        const bool verbose
        ) :
        server_(server),
        port_(port),
        nick_(nick),
        password_(password),
        socket_(-1),
        use_sasl_(use_sasl),
        sasl_username_(sasl_username),
        sasl_password_(sasl_password),
        verbose_(verbose)
        {
            splinter_id_ = get_next_id();
        }

void splinterClient::add_to_clients()
{
    int next_id = get_next_id();
    clients_[std::to_string(next_id)] = shared_from_this();
}

splinterClient::~splinterClient()
{
    if (socket_ != -1)
    {
        close(socket_);
    }
    destroy_self();
}

void splinterClient::destroy_self()
{
    // Check if a client with the provided identifier exists
    auto it = clients_.find( std::to_string( get_id()) );
    if (it != clients_.end())
    {
        std::cerr << "Dropping splinter with ID '" << std::to_string(splinter_id_) << "'." << std::endl;
        // Remove the client from the clients_ member variable
        clients_.erase(it);
    } else {
        std::cerr << "No client with id '" << std::to_string(splinter_id_) << "' found.  Report this as a bug." << std::endl;
    }
}

// called when a client is removed from a channel
void splinterClient::channels_del(std::string &channel) {
    auto it = std::remove(current_channels.begin(), current_channels.end(), channel );
    current_channels.erase(it, current_channels.end());
}

// called when a client is added to a channel to track which channels the client is in
void splinterClient::channels_add(std::string &channel)
{
    if (!is_in_channel(channel)) {
        current_channels.push_back(channel);
    }
}

// checks if this client is in a channel
bool splinterClient::is_in_channel(std::string &channel) {
    return std::find(current_channels.begin(), current_channels.end(), channel) != current_channels.end();
}

// checks if any client is in a channel
bool splinterClient::is_in_channel( const int& splinter_id, std::string& channel )
{
    auto it = clients_.find( std::to_string( splinter_id ) );
    if (it != clients_.end())
    {
        return it->second->is_in_channel(channel);
    }
    return false;
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

    if (use_sasl_)
    {
        /*
         * TODO: make this parameterized as we're not really using CAP LS as intended.
         *  It should be something to the tune of a client action that returns a list of
         *  capabilities to check for sasl against the result of that, and if SASL is
         *  supported, then we can proceed with the authentication process or fail if
         *  it's not supported.
        */
        list_server_capabilities();
        //sleep(1);

    } else {
        set_nick( nick_ );
        register_user(nick_);
    }
}

void splinterClient::send( const std::string& message )
{
    if (::send(socket_, message.c_str(), message.size(), 0) == -1)
    {
        std::cerr << "Failed to send message to server" << std::endl;
        return;
    }
    if (::send(socket_, "\r\n", 2, 0) == -1)
    {
        std::cerr << "Failed to send message to server" << std::endl;
        return;
    }
}

const std::string& splinterClient::get_server() const
{
    return server_;
}

const std::string& splinterClient::get_nick() const {
    return nick_;
}

const int splinterClient::get_id() const {
    return splinter_id_;
}

bool splinterClient::password_is_valid( const std::string& passtoken )
{
    bool result = (passtoken == password_);
    return result;
}

