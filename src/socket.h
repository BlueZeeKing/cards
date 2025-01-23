#pragma once

#include <istream>
#include <memory>
#include <string>

class socketfd {
  public:
    std::shared_ptr<int> fd;

    socketfd(int);
    void force_close();
};

class tcp_streambuffer : public std::streambuf {
    char *read_buffer;
    int read_buffer_length;
    char *write_buffer;
    int write_buffer_length;

  public:
    socketfd fd;

    tcp_streambuffer(socketfd fd);

    tcp_streambuffer(int fd);

    virtual int underflow();

    virtual int overflow(int c);

    virtual int sync();

    virtual std::streambuf *setbuf(char *s, std::streamsize n);

    virtual ~tcp_streambuffer();
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
