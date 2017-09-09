// Server.h
// Created by Caner Korkmaz on 9/9/2017.
// Copyright 2017 Caner Korkmaz
//

#pragma once

#include <boost/asio.hpp>

#include "ServerConnection.h"

namespace network {
class Server {
  boost::asio::ip::tcp::acceptor acceptor_;
 public:
  explicit Server(boost::asio::io_service &io_service, unsigned short port);
 private:
  void start_accept();

  void handle_accept(Connection::pointer new_connection,
                     const boost::system::error_code &error);
};
}