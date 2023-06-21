cmake_minimum_required(VERSION 3.23)
project(splinterIRC)

set(CMAKE_CXX_STANDARD 17)

add_executable(splinter splinter.cpp src/libsplinterIRC/events/IRCEventv3.h src/libsplinterIRC/events/IRCEventv3.cpp src/libsplinterIRC/events/mappers.cpp src/libsplinterIRC/events/postprocessors.cpp src/libsplinterIRC/client/client.cpp src/libsplinterIRC/client/client.h src/libsplinterIRC/command_processing/command_processing.cpp src/libsplinterIRC/handlers/handlers.cpp src/libsplinterIRC/threads/client_threads.cpp src/libsplinterIRC/actions/actions.cpp src/libsplinterIRC/events/mappers.cpp src/libsplinterIRC/handlers/mappings.cpp src/libsplinterIRC/events/IRCEventv3.cpp src/libsplinterIRC/events/IRCEventv3.h src/libsplinterIRC/events/postprocessors.cpp src/libsplinterIRC/events/helpers.cpp)

add_executable(test_parser test_parser.cpp src/libsplinterIRC/events/IRCEventv3.h src/libsplinterIRC/events/IRCEventv3.cpp src/libsplinterIRC/events/mappers.cpp src/libsplinterIRC/events/postprocessors.cpp src/libsplinterIRC/events/helpers.cpp)