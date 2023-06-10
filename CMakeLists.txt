cmake_minimum_required(VERSION 3.23)
project(splinterIRC)

set(CMAKE_CXX_STANDARD 14)

add_executable(splinter splinter.cpp src/libsplinterIRC/events/IRCEvent.cpp src/libsplinterIRC/events/IRCEvent.h src/libsplinterIRC/client/client.cpp src/libsplinterIRC/client/client.h src/libsplinterIRC/command_processing/command_processing.cpp src/libsplinterIRC/handlers/handlers.cpp src/libsplinterIRC/threads/client_threads.cpp src/libsplinterIRC/actions/actions.cpp)
