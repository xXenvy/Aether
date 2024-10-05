#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include "router.hpp"

namespace aether {
    class HttpServer {
    public:
        HttpServer(
            const std::string_view& host,
            unsigned short port
        );
        void run();
        std::string getHost() const;
        unsigned short getPort() const;

        Router router;
    private:
        void listen();

        boost::asio::io_context io_context_;
        boost::asio::ip::tcp::acceptor acceptor_;
    };
};

#endif // SERVER_H
