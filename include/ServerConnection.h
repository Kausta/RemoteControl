// ServerConnection.h
// Created by Caner Korkmaz on 9/9/2017.
// Copyright 2017 Caner Korkmaz
//

#pragma once

#include <memory>
#include <boost/asio.hpp>
#include "CommandReader.h"

namespace network {
class Connection : public std::enable_shared_from_this<Connection> {
 public:
  using pointer = std::shared_ptr<Connection>;
  static pointer create(boost::asio::io_service &io_service);

  boost::asio::ip::tcp::socket &socket();

  void start();

  void read_async();

  void write_async(const std::string &message);
 private:
  explicit Connection(boost::asio::io_service &io_service);

  void handle_read(const boost::system::error_code &error,
                   size_t bytes_transferred);

  void handle_write(std::shared_ptr<std::string> sent_message,
                    const boost::system::error_code &error,
                    size_t /*bytes_transferred*/);

  boost::asio::ip::tcp::socket socket_;
  boost::asio::streambuf buffer_;

  CommandReader reader_;
};
}
