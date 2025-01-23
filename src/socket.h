#pragma once

#include <istream>
#include <memory>
#include <string>

class socketfd {
  public:
    std::shared_ptr<int> fd;

    socketfd(int);
};

class tcp_stream : public std::iostream {
  public:
    tcp_stream(int socketfd);
    tcp_stream(const std::string &url, const std::string &port);
    tcp_stream(const tcp_stream &);
    ~tcp_stream();
};

class tcp_server {
    socketfd fd;

  public:
    tcp_server(const std::string &url, const std::string &port);
    tcp_stream accept();
};
