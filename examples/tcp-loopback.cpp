#include "network/network.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

auto main() -> int
{
    try {
        const Network::IpSocketHints hints {SOCK_STREAM, 0};
        const Network::EndpointView bind_endpoint {"127.0.0.1", "0"};

        const auto bound {Network::bind(bind_endpoint, hints, false)};
        Network::Socket* server {nullptr};

        for (auto& result : bound) {
            if (result) {
                server = result->get();
                break;
            }
        }

        if (server == nullptr) {
            std::cerr << "Could not bind TCP loopback socket\n";
            return EXIT_FAILURE;
        }

        if (const auto error {server->listen(1)}) {
            std::cerr << error.string() << '\n';
            return EXIT_FAILURE;
        }

        const Network::Address bound_address {server->get_sockname()};
        const std::string port {std::to_string(bound_address.port())};
        const Network::EndpointView connect_endpoint {
            "127.0.0.1",
            port
        };

        const auto connected {Network::connect(connect_endpoint, hints, false)};
        Network::Socket* client {nullptr};

        for (auto& result : connected) {
            if (result) {
                client = result->get();
                break;
            }
        }

        if (client == nullptr) {
            std::cerr << "Could not connect to TCP loopback socket\n";
            return EXIT_FAILURE;
        }

        auto peer {Network::accept(*server)};
        if (!peer) {
            std::cerr << "Could not accept TCP loopback connection\n";
            return EXIT_FAILURE;
        }

        if (client->write("hello") == Network::socket_error) {
            std::cerr << "Could not write from TCP client\n";
            return EXIT_FAILURE;
        }

        Network::TextBuffer buffer {64};
        if (peer->read(buffer) == Network::socket_error) {
            std::cerr << "Could not read TCP server message\n";
            return EXIT_FAILURE;
        }

        std::string message {buffer};
        std::cout << "Received: " << message << '\n';

        if (peer->write("ack") == Network::socket_error) {
            std::cerr << "Could not write from TCP server\n";
            return EXIT_FAILURE;
        }

        Network::TextBuffer response {64};
        if (client->read(response) == Network::socket_error) {
            std::cerr << "Could not read TCP client response\n";
            return EXIT_FAILURE;
        }

        std::cout << "Received: " << std::string {response} << '\n';
        return EXIT_SUCCESS;
    }
    catch (const Network::Error& error) {
        std::cerr << error.what() << '\n';
        return EXIT_FAILURE;
    }
}
