/*
* File: Server.h
* Copyright: 2017 Caner Korkmaz (info@canerkorkmaz.com)
* Description:
*/

#pragma once

#include <boost/asio.hpp>

#include "ServerConnection.h"

namespace network
{
	class server
	{
		boost::asio::ip::tcp::acceptor acceptor_;
	public:
		explicit server(boost::asio::io_service& io_service, unsigned short port);
	private:
		void start_accept();

		void handle_accept(connection::pointer new_connection,
		                   const boost::system::error_code& error);
	};
}