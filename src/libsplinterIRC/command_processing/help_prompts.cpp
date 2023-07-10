#include "../client/client.h"

void splinterClient::prompt_help_general(std::string& reply_to )
{
    send_private_message( reply_to, "I handle several commands:" );
    send_private_message( reply_to, "!help, !quit, !splinter, !list, !destroy !channel !say !raw !auth !pipe" );
    send_private_message( reply_to, "For more information on a specific command, type '!help <command>'" );
}

void splinterClient::prompt_help_channel( std::string& reply_to )
{
    send_private_message( reply_to, "Manages channels." );
    send_private_message( reply_to, "!channel <id> <join|part|list>" );
    send_private_message( reply_to, "Example: !channel 1 join #channel" );
    send_private_message( reply_to, "Example: !channel 1 part #channel" );
    send_private_message( reply_to, "Example: !channel 1 list" );
    send_private_message( reply_to, "Example: !channel * list" );
}

void splinterClient::prompt_help_destroy( std::string& reply_to )
{
    send_private_message( reply_to, "Destroys a splinter descendant." );
    send_private_message( reply_to, "!destroy <id>" );
    send_private_message( reply_to, "Get IDs by using !list (see: \"!help list\")" );
}

void splinterClient::prompt_help_say( std::string& reply_to )
{
    send_private_message( reply_to, "Says something to a user or in a channel." );
    send_private_message( reply_to, "!say <id> <nick|channel> <message>" );
    send_private_message( reply_to, "Example: !say 1 #channel Hello, world!" );
}

void splinterClient::prompt_help_quit( std::string& reply_to )
{
    send_private_message( reply_to, "Quits a connection.  Essentially the same as destroying a client." );
    send_private_message( reply_to, "!quit <id> <message>" );
    send_private_message( reply_to, "Example: !quit 93 I LEAVE IN PROTEST" );
}

void splinterClient::prompt_help_raw( std::string& reply_to )
{
    send_private_message( reply_to, "Sends a RAW IRC protocol command.  Use wisely." );
    send_private_message( reply_to, "!raw <id> <IRC_COMMAND>" );
    send_private_message( reply_to, "Example: !raw 0 CTCP bagira version" );
}

void splinterClient::prompt_help_splinter( std::string& reply_to )
{
    // splinter <server> <port> <nick> <splinter_password> <use_ssl> <strict_ssl> <use_sasl> [<sasl_username> <sasl_password>]

    send_private_message( reply_to, "Creates a new splinter client." );
    send_private_message( reply_to, "splinter <server> <port> <nick> <splinter_password> <use_ssl> <strict_ssl> [ <use_sasl> <sasl_username> <sasl_password> ]" );
    send_private_message( reply_to, "Example: !splinter irc.example.com 6667 HamatoYoshi mypassword false false" );
    send_private_message( reply_to, "Example: !splinter irc.example.com 6667 HamatoYoshi mypassword true false true mysaslusername mysaslpassword" );
    send_private_message( reply_to, "Example: !splinter irc.example.com 6667 HamatoYoshi mypassword false false" );
}

void splinterClient::prompt_help_pipe( std::string& reply_to )
{
    send_private_message( reply_to, "Manage pipes." );
    send_private_message( reply_to, "Subcommands are: create, list, destroy" );
    send_private_message( reply_to, " " );
    send_private_message( reply_to, "create:" );
    send_private_message( reply_to, "Pipes one source to one target.  Use two for bi-directional support." );
    send_private_message( reply_to, "pipe create <splinter_id> <source> <splinter_id> <target> <preserve_sender>" );
    send_private_message( reply_to, " " );
    send_private_message( reply_to, "Example: !pipe create 1 #debian 0 bagira true" );
    send_private_message( reply_to, "Example: !pipe create 0 bagira 1 #debian false" );
    send_private_message( reply_to, " " );
    send_private_message( reply_to, "Example: !pipe create 0 #darkhorse 0 #darkhorse-banned true" );
    send_private_message( reply_to, " " );
    send_private_message( reply_to, "Example: !pipe create 0 #channel1 1 #channel2 true" );
    send_private_message( reply_to, "Example: !pipe create 1 #channel2 0 #channel1 true" );
    send_private_message( reply_to, " " );
    send_private_message( reply_to, "Example: !pipe create 0 dummyuser1 0 dummyuser2 false" );
    send_private_message( reply_to, "Example: !pipe create 0 dummyuser2 0 dummyuser1 false" );
    send_private_message( reply_to, " " );
    send_private_message( reply_to, "Note: The <preserve_sender> is a boolean indicating whether or not to preserve the context of the message or not (splinterID, sending user/channel)." );
    send_private_message( reply_to, " " );
    send_private_message( reply_to, "list:" );
    send_private_message( reply_to, "Lists the pipes by pipe ID." );
    send_private_message( reply_to, "pipe list" );
    send_private_message( reply_to, " " );
    send_private_message( reply_to, "destroy:" );
    send_private_message( reply_to, "Destroys a pipe by pipe ID." );
    send_private_message( reply_to, "pipe destroy <pipe_id>" );
}

void splinterClient::prompt_help_list( std::string& reply_to )
{
    send_private_message( reply_to, "Lists all splinter clients, everywhere, including descendants." );
    send_private_message( reply_to, "!list" );
    send_private_message( reply_to, "Did you really need help with that?" );
}

void splinterClient::prompt_help_auth(std::string &reply_to)
{
    send_private_message(reply_to, "Takes or relinquishes control of a splinter.");
    send_private_message(reply_to, "auth <login|logout> [<password>]");
}

