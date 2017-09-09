// Server.cpp
// Created by Caner Korkmaz on 9/9/2017.
// Copyright 2017 Caner Korkmaz
//

#include <vector>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>

#include "Server.h"

using namespace network;
using namespace boost::asio;

network::Server::Server(io_service &io_service, unsigned short port)
    : acceptor_(io_service, ip::tcp::endpoint(ip::tcp::v4(), port)) {
  std::cout << "Server started at port " << port << ".\n";
  start_accept();
}

void network::Server::start_accept() {
  auto new_connection = Connection::create(acceptor_.get_io_service());

  acceptor_.async_accept(new_connection->socket(),
                         boost::bind(&Server::handle_accept, this, new_connection, placeholders::error));
}

void network::Server::handle_accept(Connection::pointer new_connection, const boost::system::error_code &error) {
  if (!error) {
    new_connection->start();

    const auto endpoint = new_connection->socket().remote_endpoint();
    std::cout << "Connected to " << endpoint.address() << ":" << endpoint.port() << ".\n";
  }

  start_accept();
}
