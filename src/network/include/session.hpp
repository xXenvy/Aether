#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>

#include "../../parsing/include/parser.hpp"
#include "router.hpp"

namespace aether {
	class Session : public std::enable_shared_from_this<Session> {
	public:
		Session(boost::asio::ip::tcp::socket socket, Router& router);
		void run();
		void close();
	private:
		void readRequest();
		void sendRequest(const std::string_view& data);
		void handleRequest(const size_t size);

		boost::asio::ip::tcp::socket socket_;
		boost::asio::streambuf buffer_;

		Router router_;
		HttpParser parser_;
	};
};

#endif // SESSION_H