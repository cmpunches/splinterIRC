#include "IRCEventEnvelope.h"

IRCEventEnvelope::IRCEventEnvelope(const std::string& raw, const std::string& server, std::string client_id ): client_id_(client_id) {
    // set the server attribute so that event processors know which server this event came from
    set_attribute( "server", server );
    set_attribute( "_raw", raw );

    // parse the event to a basic, normalized structure
    parse();

    // postprocess the event to extract event-specific information
    postprocess();

}

void IRCEventEnvelope::parse() {
    enum State { PREFIX, COMMAND, PARAMS, TRAILING };
    State state = PREFIX;
    std::string raw = get_scalar_attribute( "_raw" );
    std::istringstream ss( raw );
    std::string token;
    std::string prefix, command;
    while (std::getline(ss, token, ' ')) {
        switch (state) {
            case PREFIX:
                if (token[0] == ':') {
                    prefix = token.substr(1);
                    state = COMMAND;
                } else {
                    command = token;
                    state = PARAMS;
                }
                break;
            case COMMAND:
                command = token;
                state = PARAMS;
                break;

            case PARAMS:
                if (token[0] == ':') {
                    autoextract_params.push_back(token.substr(1));
                    state = TRAILING;
                } else {
                    autoextract_params.push_back(token);
                }
                break;
            case TRAILING:
                autoextract_params.back() += " " + token;
                break;
        }
    }

    // use our identifiers, not the server's
    type_ = verb_to_type( command );
    set_attribute( "type", type_to_verb( type_ ) );

    set_attribute( "_command", command );

    // will only show when printed with debug output
    set_attribute( "_prefix", prefix );

    // set the raw attribute to the raw string, but escaped for human-readable serialization
    // will only show when printed with debug output
    std::ostringstream quoted_raw;
    quoted_raw << std::quoted( raw );
    std::string escaped_raw = quoted_raw.str();
    escaped_raw.erase(0, 1);
    escaped_raw.erase(escaped_raw.size() - 1);
    set_attribute( "_raw", escaped_raw );
    set_attribute( "_postprocessed", "false" );
}

const IRCEventEnvelope::Type IRCEventEnvelope::get_type() const {
    return type_;
}

void IRCEventEnvelope::set_attribute(const std::string& key, const std::string& value) {
    scalar_attributes[key] = value;
}

void IRCEventEnvelope::set_attribute(const std::string& key, const std::vector<std::string>& value) {
    array_attributes[key] = value;
}

std::string IRCEventEnvelope::get_scalar_attribute(const std::string& key) {
    if (scalar_attributes.count(key)) {
        return scalar_attributes[key];
    } else {
        return "";
    }
}

std::vector<std::string> IRCEventEnvelope::get_array_attribute(const std::string& key) const {
    if (array_attributes.count(key)) {
        return array_attributes.at(key);
    } else {
        return {};
    }
}

const std::map<std::string, std::string>& IRCEventEnvelope::get_all_scalar_attributes() const {
    return scalar_attributes;
}

const std::map<std::string, std::vector<std::string>>& IRCEventEnvelope::get_all_array_attributes() const {
    return array_attributes;
}

std::string IRCEventEnvelope::to_json(bool pretty, int indent, bool debug) const {
    std::ostringstream ss;
    std::string indent_str(indent, ' ');
    if (pretty) {
        ss << "{\n";
        for (auto it = scalar_attributes.begin(); it != scalar_attributes.end(); ++it) {
            if (!debug && it->first[0] == '_') continue;
            ss << indent_str << "\"" << it->first << "\": \"" << it->second << "\"";
            if (std::next(it) != scalar_attributes.end() || !autoextract_params.empty() || !array_attributes.empty()) {
                ss << ",";
            }
            ss << "\n";
        }
        for (auto it = array_attributes.begin(); it != array_attributes.end(); ++it) {
            if (!debug && it->first[0] == '_') continue;
            ss << indent_str << "\"" << it->first << "\": ";
            if (it->second.empty()) {
                ss << "[]";
            } else {
                ss << "[\n";
                for (int i = 0; i < it->second.size(); ++i) {
                    ss << indent_str << indent_str << "\"" << it->second[i] << "\"";
                    if (i != it->second.size() - 1) {
                        ss << ",";
                    }
                    ss << "\n";
                }
                ss << indent_str << "]";
            }
            if (std::next(it) != array_attributes.end() || !autoextract_params.empty()) {
                ss << ",";
            }
            ss << "\n";
        }
        if (!autoextract_params.empty() && debug) {
            ss<< indent_str + "\"_autoextract\": [\n";
            for (int i = 0; i < autoextract_params.size(); ++i) {
                ss<< indent_str + indent_str + "\"" + autoextract_params[i] + "\"";
                if (i != autoextract_params.size() - 1) {
                    ss<< ",";
                }
                ss<< "\n";
            }
            ss<< indent_str + "]\n";
        }
        ss<< "}";
    } else {
        for (auto it = scalar_attributes.begin(); it != scalar_attributes.end(); ++it) {
            if (!debug && it->first[0] == '_') continue;
            ss<< "\"" + it->first + "\":\"" + it->second + "\"";
            if (std::next(it) != scalar_attributes.end() || !autoextract_params.empty() || !array_attributes.empty()) {
                ss<< ",";
            }
        }
        for (auto it = array_attributes.begin(); it != array_attributes.end(); ++it) {
            if (!debug && it->first[0] == '_') continue;
            ss<< "\"" + it->first + "\":[";
            for (int i = 0; i < it->second.size(); ++i) {
                ss<< "\"" + it->second[i] + "\"";
                if (i != it->second.size() - 1) {
                    ss<< ",";
                }
            }
            ss<< "]";
            if (std::next(it) != array_attributes.end() || !autoextract_params.empty()) {
                ss<< ",";
            }
        }
        if (!autoextract_params.empty() && debug) {
            ss<< "\"params\":[";
            for (int i = 0; i < autoextract_params.size(); ++i) {
                ss<< "\"" + autoextract_params[i] + "\"";
                if (i != autoextract_params.size() - 1) {
                    ss<< ",";
                }
            }
            ss<< "]}";
        } else {
            ss<< "}";
        }
    }
    return ss.str();
}
