#include <boost/log/trivial.hpp>
#include <iostream>

#include "include/server.hpp"
#include "include/session.hpp"

using namespace boost::asio::ip;

namespace aether {
    HttpServer::HttpServer(
        const std::string_view& host,
        unsigned short port
    )
        : acceptor_(
            io_context_,
            tcp::endpoint(make_address(host), port)
        ) {};

    void HttpServer::run() {
        BOOST_LOG_TRIVIAL(info) << "Started listening on "
            << getHost() << ":" << getPort();
        listen();
        io_context_.run();
    }

    std::string HttpServer::getHost() const {
        return acceptor_.local_endpoint().address().to_string();
    }

    unsigned short HttpServer::getPort() const {
        return acceptor_.local_endpoint().port();
    }

    void HttpServer::listen() {
        acceptor_.async_accept(
            [this](boost::system::error_code error_code, tcp::socket socket) {
                if (!error_code) {
                    BOOST_LOG_TRIVIAL(info)
                        << "Creating session for: "
                        << socket.remote_endpoint().address().to_string()
                        << ":" << socket.remote_endpoint().port();

                    // Start a new session.
                    std::make_shared<Session>(std::move(socket), router)->run();
                }
                else {
                    BOOST_LOG_TRIVIAL(error)
                        << "Error during accepting connection: "
                        << error_code.message();
                }
                listen();
            }
        );
    }
};