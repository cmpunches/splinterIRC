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

splinterClient::splinterClient(const std::string& server, const std::string& port, const std::string& nick, const std::string& password, int parent_id ) :
        server_(server), port_(port), nick_(nick), password_(password), socket_(-1)
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

    set_nick( nick_ );
    set_user( nick_ );
}

void splinterClient::send( const std::string& message )
{
    if (::send(socket_, message.c_str(), message.size(), 0) == -1)
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
