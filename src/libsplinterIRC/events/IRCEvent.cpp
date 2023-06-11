#include "IRCEvent.h"

// orientation
IRCEvent::IRCEvent(const std::string& raw_message, const std::string& server) : raw_message_(raw_message), server_(server), type_(Type::UNKNOWN) {
    // Parse the raw message as an IRC protocol event and populate properties
    // representing the event type, sender, target, etc.
    std::istringstream ss(raw_message.substr(0, raw_message.find_last_not_of("\r\n") + 1));
    std::string token;
    if (std::getline(ss, token, ' ')) {
        if (token[0] == ':') {
            sender_ = token.substr(1);
            // Break down sender into realname, nick, and ident
            std::string::size_type pos = sender_.find('!');
            if (pos != std::string::npos) {
                nick_ = sender_.substr(0, pos);
                std::string::size_type pos2 = sender_.find('@', pos + 1);
                if (pos2 != std::string::npos) {
                    ident_ = sender_.substr(pos + 1, pos2 - pos - 1);
                    realname_ = sender_.substr(pos2 + 1);
                }
            }

            // type mappings for parser
            if (std::getline(ss, token, ' '))
            {
                type_ = verb_to_type(token);
            }

        } else if (token == "PING") {
            type_ = Type::PING;
        } else if (token == "ERROR" ) {
            type_ = Type::ERROR;
        }
        if (std::getline(ss, token, ' ')) {
            target_ = token;
        }
        std::getline(ss, message_);
        if (!message_.empty() && message_[0] == ':') {
            message_ = message_.substr(1);
        }

        if (type_ == Type::ERROR) {
            // Set the target property to be blank
            target_ = "";
            // Get the rest of the line as the message
            std::getline(ss, message_);
            if (!message_.empty() && message_[0] == ':') {
                message_ = message_.substr(1);
            }
        }

        if (type_ == Type::PING) {
            sender_ = server_;
        }
    }

    // the irc protocol is a trash fire and we have to do this
    // because the format of the raw message is different depending
    // on the type of event, even for PRIVMSG which has two distinct types
    // of messages depending on whether it's a channel message or a private
    // message.
    if (type_ == Type::PRIVMSG) {
        if (target_.find('#') == 0) {
            channel_ = target_;
            target_ = sender_;
            type_ = Type::S_CHANNEL_MESSAGE;
        } else {
            channel_ = target_;
            type_ = Type::S_PRIVATE_MESSAGE;
        }
    }
}

const std::string& IRCEvent::raw_message() const {
    return raw_message_;
}

IRCEvent::Type IRCEvent::type() const {
    return type_;
}

const std::string& IRCEvent::sender() const {
    return sender_;
}

const std::string& IRCEvent::nick() const {
    return nick_;
}

const std::string& IRCEvent::ident() const {
    return ident_;
}

const std::string& IRCEvent::realname() const {
    return realname_;
}

const std::string& IRCEvent::target() const {
    return target_;
}

const std::string& IRCEvent::channel() const {
    return channel_;
}

const std::string& IRCEvent::message() const {
    return message_;
}

std::string IRCEvent::to_json() const {
    std::ostringstream ss;
    ss << "{\n";
    std::string raw_message = raw_message_;
    if (raw_message.size() >= 2 && raw_message.substr(raw_message.size() - 2) == "\r\n") {
        raw_message = raw_message.substr(0, raw_message.size() - 2);
    } else if (raw_message.size() >= 1 && raw_message.back() == '\r') {
        raw_message = raw_message.substr(0, raw_message.size() - 1);
    } else if (raw_message.size() >= 1 && raw_message.back() == '\n') {
        raw_message = raw_message.substr(0, raw_message.size() - 1);
    }
    ss << "\t\"raw_message\": " << std::quoted(raw_message) << ",\n"; // Use std::quoted to escape special characters
    ss << "\t\"type\": \"" << type_to_verb(this->type_) << "\",\n";
    ss << "\t\"sender\": \"" << sender_ << "\",\n";
    ss << "\t\"nick\": \"" << nick_ << "\",\n";
    ss << "\t\"ident\": \"" << ident_ << "\",\n";
    ss << "\t\"realname\": \"" << realname_ << "\",\n";
    ss << "\t\"target\": \"" << target_ << "\",\n";
    ss << "\t\"channel\": \"" << channel_ << "\",\n";
    ss << "\t\"message\": " << std::quoted(message_) << ",\n";
    ss << "\t\"server\": \"" << server_ << "\"\n";
    ss << "}\n";
    return ss.str();
}
