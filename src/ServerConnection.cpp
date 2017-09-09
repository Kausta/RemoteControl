// ServerConnection.cpp
// Created by Caner Korkmaz on 9/9/2017.
// Copyright 2017 Caner Korkmaz
//

#include <memory>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>

#include "ServerConnection.h"

using namespace network;
using namespace boost::asio;

Connection::pointer network::Connection::create(io_service &io_service) {
  return pointer(new Connection(io_service));
}

ip::tcp::socket &network::Connection::socket() {
  return socket_;
}

void network::Connection::start() {
  write_async("Connected to Server !\r\n");
  read_async();
}

void network::Connection::read_async() {
  buffer_.consume(buffer_.size());
  async_read_until(socket_, buffer_, '\n',
                   boost::bind(&Connection::handle_read,
                               shared_from_this(),
                               placeholders::error,
                               placeholders::bytes_transferred));
}

void network::Connection::write_async(const std::string &message) {
  auto message_ptr = std::make_shared<std::string>(message);
  async_write(socket_, buffer(*message_ptr),
              boost::bind(&Connection::handle_write, shared_from_this(),
                          message_ptr,
                          placeholders::error,
                          placeholders::bytes_transferred));
}

network::Connection::Connection(boost::asio::io_service &io_service)
    : socket_(io_service), reader_(CommandManager::instance(), {"exit", "-e", "--exit"}) {
}

void network::Connection::handle_read(const boost::system::error_code &error, size_t bytes_transferred) {
  bool is_running = true;
  if (!error && bytes_transferred > 0) {
    buffer_.commit(bytes_transferred);

    std::istream input(&buffer_);
    std::string message;
    // This way, no trim error occurs
    std::getline(input, message);
    // It won't have a new line, but trim anyways
    boost::algorithm::trim(message);
    std::cout << "Got: '" << message + "'!\n";

    try {
      auto executed = reader_.execute_line(message);
      if (!executed) {
        is_running = false;
      }
      message = "Executed command: " + message + "\n";
    }
    catch (const std::runtime_error &err) {
      message = "Error while executing command (" + message + ")= " + err.what() + "\n";
    }

    write_async(message);
  }
  if (is_running) {
    read_async();
  }
}

void network::Connection::handle_write(std::shared_ptr<std::string> sent_message, const boost::system::error_code &error, size_t) {
  if (!error) {
    std::cout << "Successfully sent '" << boost::algorithm::trim_copy(*sent_message) << "' !\n";
  }
}
