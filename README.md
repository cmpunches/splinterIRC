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

Creates a new client on a new set of detached threads.  This allows you to connect to either other servers or the same server, and should allow for seamless identity shifting across an infinite number of networks if orchestrated correctly with sufficient preparation as children survive the parents and are not bound to the same network.

## !list

Lists all splinters that thread is aware of.  This version is currently using a spanning-tree style index, so, only a parent will see a child.  Future versions will have a shared index of splinters to allow a centralized command and control model.

## !destroy

Destroys a splinter that the receiving thread is aware of.

## !join

Join a channel.

## !say

Say something either to a channel or another user.

# Event Handling

See 'src/libsplinterIRC/handlers/handlers.cpp'.  You are able to have splinter respond to any IRC events that an RFC-compliant server can send.

# Future Features

- Centralized indexing of children. 
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