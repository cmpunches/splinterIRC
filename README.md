# splinterIRC

A "highly hackable", event-driven IRC bot with full protocol event coverage (RFC 2812).

## Original Author

Chris Punches

# Commands

splinterIRC supports some basic commands out of box, but is intended for you to hack it via code you place in its handler and command processing methods.  Commands are ignored unless they are prefixed with an exclamation point ('!') and appended with that splinter's password.
 
## !help <password>

Shows all available commands.

## !quit

Quits the current client.

## !splinter 

Creates a new client on a new set of detached threads.  This allows you to connect to either other servers or the same server, and should allow for seamless identity shifting across a number of networks if orchestrated correctly with sufficient preparation as children survive the parents and are not bound to the same network.

## !list

Lists all splinters.  This presents a shared index of splinters to allow a centralized command and control model.

## !destroy

Destroys a splinter.  Beware that while new splinters survive their parents and each other, splinter ID 0 is the master splinter, and destroying it will end all other threads and exit the process.

## !join

Join a channel.

## !say

Say something either to a channel or another user.

# Event Handling

See `src/libsplinterIRC/handlers/handlers.cpp`.  You are able to have splinter respond to any IRC events that an RFC-compliant server can send.

# Future Features

- More refined/nuanced event parsing
- More client actions.  While all events in the IRC protocol are currently handled, not all client actions of the IRC protocol are implemented.  Future versions will be able to perform any client action in the RFC protocol with orchestration through the command processing subsystem.
- SASL and SSL support

# Building

To compile splinterIRC, you will need no external dependencies beyond a C++14 compliant compiler, cmake, make.  

To build, simply run these commands in the root directory:

```bash
cmake .
make
```

# Running splinterIRC

To run, simply execute `./splinterIRC`.  

splinterIRC requires some commandline options.  To see what those are, run `./splinterIRC --help`.