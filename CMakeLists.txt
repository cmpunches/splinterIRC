cmake_minimum_required(VERSION 3.23)
project(splinterIRC)

set(CMAKE_CXX_STANDARD 14)

add_executable(splinter splinter.cpp src/libsplinterIRC/irc_event/irc_event.cpp src/libsplinterIRC/irc_event/irc_event.h src/libsplinterIRC/client/client.cpp src/libsplinterIRC/client/client.h src/libsplinterIRC/client/command_processing/command_processing.cpp src/libsplinterIRC/client/handlers/handlers.cpp)
