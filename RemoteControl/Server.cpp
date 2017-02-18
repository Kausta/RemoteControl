/*
* File: Server.cpp
* Copyright: 2017 Caner Korkmaz (info@canerkorkmaz.com)
* Description:
*/

#include <vector>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>

#include "Server.h"

network::server::server(boost::asio::io_service& io_service, unsigned short port)
  : acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
	std::cout << "Server started at port " << port << ".\n";
	start_accept();
}

void network::server::start_accept()
{
	auto new_connection =
		connection::create(acceptor_.get_io_service());

	acceptor_.async_accept(new_connection->socket(),
	                       boost::bind(&server::handle_accept, this, new_connection,
	                                   boost::asio::placeholders::error));
}

void network::server::handle_accept(connection::pointer new_connection, const boost::system::error_code& error)
{
	if (!error)
	{
		new_connection->start();

		const auto endpoint = new_connection->socket().remote_endpoint();
		std::cout << "Connected to " << endpoint.address() << ":" << endpoint.port() << ".\n";
	}

	start_accept();
}
