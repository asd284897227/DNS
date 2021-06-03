cmake_minimum_required(VERSION 3.17)
project(DNS)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

set(CMAKE_CXX_STANDARD 20)

add_executable(DNS main.cpp headers/RelayDNSRequestHandler.h headers/DNSMessageHandler.h headers/DNSMessage.h)

