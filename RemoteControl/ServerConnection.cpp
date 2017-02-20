/*
* File: ServerConnection.cpp
* Copyright: 2017 Caner Korkmaz (info@canerkorkmaz.com)
* Description:
*/

#include <memory>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>

#include "ServerConnection.h"
#include "Program.h"

network::connection::pointer network::connection::create(boost::asio::io_service& io_service)
{
	return pointer(new connection(io_service));
}

boost::asio::ip::tcp::socket& network::connection::socket()
{
	return socket_;
}

void network::connection::start()
{
	write_async("Connected to server !\r\n");
	read_async();
}

void network::connection::read_async()
{
	boost::asio::async_read_until(socket_, buffer_, '\n',
	                              boost::bind(&connection::handle_read, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void network::connection::write_async(const std::string& message)
{
	auto message_ptr = std::make_shared<std::string>(message);
	boost::asio::async_write(socket_, boost::asio::buffer(*message_ptr),
	                         boost::bind(&connection::handle_write, shared_from_this(),
	                                     message_ptr,
	                                     boost::asio::placeholders::error,
	                                     boost::asio::placeholders::bytes_transferred));
}

network::connection::connection(boost::asio::io_service& io_service)
	: socket_(io_service)
	, reader_(program::instance().command_manager())
{
}

void network::connection::handle_read(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (!error && bytes_transferred > 0)
	{
		buffer_.commit(bytes_transferred);

		std::istream input(&buffer_);
		std::string message;
		// This way, no trim error occures
		std::getline(input, message);
		// It won't have a new line, but trim anyways
		boost::algorithm::trim(message);
		std::cout << "Got: '" << message + "'!\n";

		try
		{
			reader_.execute_line(message);
			message = "Executed command: " + message + "\n";
		}
		catch(const std::runtime_error& err)
		{
			message = "Error while executing command (" + message +  ")= " + err.what() + "\n";
		}
		write_async(message);
		
	}

	read_async();
}

void network::connection::handle_write(std::shared_ptr<std::string> sent_message, const boost::system::error_code& error, size_t)
{
	if (!error)
	{
		std::cout << "Successfully sent '" << boost::algorithm::trim_copy(*sent_message) << "' !\n";
	}
}

void network::connection::boost_trim(std::string& message)
{
	boost::algorithm::trim_if(message, boost::is_any_of(wspace_chars));
}

std::string network::connection::boost_trim_copy(const std::string& message)
{
	return boost::algorithm::trim_copy_if(message, boost::is_any_of(wspace_chars));
}
