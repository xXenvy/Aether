#include <boost/log/trivial.hpp>
#include <string>
#include <iostream>

#include "include/session.hpp"
#include "../errors.hpp"

namespace aether {
    Session::Session(boost::asio::ip::tcp::socket socket, Router& router)
        : socket_(std::move(socket)), router_(router) {};

    void Session::run() {
        readRequest();
    }

    void Session::close() {
        BOOST_LOG_TRIVIAL(info)
            << "Closing session for client: "
            << socket_.remote_endpoint();
        socket_.close();
    }

    void Session::readRequest() {
        // Ensure session stays alive during async read.
        auto self(shared_from_this());

        async_read_until(
            socket_,
            buffer_,
            "\r\n\r\n",
            [this, self](boost::system::error_code error_code, std::size_t size) {
                if (error_code) {
                    BOOST_LOG_TRIVIAL(error)
                        << "Error during reading socket: "
                        << error_code.message();
                }
                else handleRequest(size);
                close();
            }
        );
    }

    void Session::sendRequest(const std::string_view& data) {
        // Ensure session stays alive during async write.
        auto self(shared_from_this());

        // Normally, data is being destroyed after this method is finished.
        // But, since this is async write, it can be used later, so we need keep this data longer.
        std::string* data_ptr = new std::string(data);

        async_write(
            socket_,
            boost::asio::buffer(*data_ptr),
            [self, data_ptr](boost::system::error_code error_code, std::size_t) {
                if (!error_code) {
                    BOOST_LOG_TRIVIAL(debug) << "Message sent: " << *data_ptr;
                }
                else {
                    BOOST_LOG_TRIVIAL(error) << "Error during sending: " << error_code.message();
                }
                delete data_ptr;  // Safe to delete after async_write completes.
            }
        );
    }

    void Session::handleRequest(const size_t size) {
        const std::string raw_data{
                    std::istreambuf_iterator<char>(&buffer_),
                    std::istreambuf_iterator<char>()
        };
        BOOST_LOG_TRIVIAL(debug) << "Received data: " << raw_data;
        buffer_.consume(size);

        try {
            HttpRequest request = parser_.parse(raw_data);
            Callback func = router_.getCallback(request.path);

            HttpResponse response = func(request);
            sendRequest(parser_.parse(response));
        }
        catch (AetherError& exception) {
            BOOST_LOG_TRIVIAL(error) << exception.what();
        }
    }
};