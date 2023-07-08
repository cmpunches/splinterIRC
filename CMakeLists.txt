cmake_minimum_required(VERSION 3.23)
project(splinterIRC)
find_package(OpenSSL REQUIRED)
set(CMAKE_CXX_STANDARD 14)


add_executable(splinter splinter.cpp src/libsplinterIRC/events/IRCEventEnvelope.h src/libsplinterIRC/events/IRCEventEnvelope.cpp src/libsplinterIRC/events/mappers.cpp src/libsplinterIRC/events/postprocessors.cpp src/libsplinterIRC/client/client.cpp src/libsplinterIRC/client/client.h src/libsplinterIRC/command_processing/command_processing.cpp src/libsplinterIRC/handlers/handlers.cpp src/libsplinterIRC/threads/client_threads.cpp src/libsplinterIRC/actions/actions.cpp src/libsplinterIRC/events/mappers.cpp src/libsplinterIRC/handlers/mappings.cpp src/libsplinterIRC/events/IRCEventEnvelope.cpp src/libsplinterIRC/events/IRCEventEnvelope.h src/libsplinterIRC/events/postprocessors.cpp src/libsplinterIRC/actions/IRCActionEnvelope.cpp src/libsplinterIRC/actions/IRCActionEnvelope.h src/libsplinterIRC/helpers/helpers.cpp src/libsplinterIRC/helpers/helpers.h src/libsplinterIRC/command_processing/help_prompts.cpp)
target_link_libraries(splinter PRIVATE OpenSSL::SSL)

add_executable(test_parser test_parser.cpp src/libsplinterIRC/events/IRCEventEnvelope.h src/libsplinterIRC/events/IRCEventEnvelope.cpp src/libsplinterIRC/events/mappers.cpp src/libsplinterIRC/events/postprocessors.cpp src/libsplinterIRC/helpers/helpers.cpp src/libsplinterIRC/helpers/helpers.h)