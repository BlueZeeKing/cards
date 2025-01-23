#include <cerrno>
#include <cstring>
#include <iostream>
#include <memory>
#include <netdb.h>
#include <streambuf>
#include <sys/socket.h>
#include <unistd.h>

#include "socket.h"

using namespace std;

tcp_streambuffer::tcp_streambuffer(socketfd fd) : fd(fd) {
    read_buffer_length = 2048;
    read_buffer = new char[read_buffer_length];
    write_buffer_length = 2048;
    write_buffer = new char[write_buffer_length];
    setg(read_buffer, read_buffer, read_buffer);
    setp(write_buffer, write_buffer + write_buffer_length);
}

tcp_streambuffer::tcp_streambuffer(int fd) : tcp_streambuffer(socketfd(fd)) {}

int tcp_streambuffer::underflow() {
    int amount_read = recv(*fd.fd, read_buffer, read_buffer_length, 0);

    if (amount_read < 0) {
        setg(read_buffer, read_buffer, read_buffer);
        throw errno;
    }

    if (amount_read == 0) {
        setg(read_buffer, read_buffer, read_buffer);
        return EOF;
    }

    setg(read_buffer, read_buffer, read_buffer + amount_read);
    return read_buffer[0];
}

int tcp_streambuffer::overflow(int c) {
    sync();

    if (c != EOF) {
        sputc(c);
    }

    return c;
}

int tcp_streambuffer::sync() {
    int amount_to_write = pptr() - pbase();
    int amount_written = send(*fd.fd, write_buffer, amount_to_write, 0);
    if (amount_written != amount_to_write) {
        throw errno;
    }

    setp(write_buffer, write_buffer + write_buffer_length);

    return 0;
}

streambuf *tcp_streambuffer::setbuf(char *s, streamsize n) {
    sync();
    read_buffer_length = n;
    read_buffer = new char[read_buffer_length];
    write_buffer_length = n;
    write_buffer = new char[write_buffer_length];
    setg(read_buffer, read_buffer, read_buffer);
    setp(write_buffer, write_buffer + write_buffer_length);
    return this;
}

tcp_streambuffer::~tcp_streambuffer() {
    delete[] read_buffer;
    delete[] write_buffer;
}

int open_client_socket(const string &url, const string &port) {
    struct addrinfo hints, *result;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_NUMERICSERV;

    int err = getaddrinfo(url.c_str(), port.c_str(), &hints, &result);
    if (err != 0) {
        throw strerror(err);
    }

    int socketfd = socket(result->ai_family, result->ai_socktype, 0);
    if (socketfd == -1) {
        throw strerror(errno);
    }

    err = connect(socketfd, result->ai_addr, result->ai_addrlen);
    if (err == -1) {
        throw strerror(errno);
    }

    freeaddrinfo(result);

    return socketfd;
}

tcp_stream::tcp_stream(int socketfd)
    : iostream(new tcp_streambuffer(socketfd)) {};
tcp_stream::tcp_stream(const string &url, const string &port)
    : iostream(new tcp_streambuffer(open_client_socket(url, port))) {};
tcp_stream::~tcp_stream() { delete this->rdbuf(); };
tcp_stream::tcp_stream(const tcp_stream &other)
    : iostream(new tcp_streambuffer(
          dynamic_cast<tcp_streambuffer *>(other.rdbuf())->fd)) {};

int get_raw_fd(const string &url, const string &port) {
    struct addrinfo hints, *result;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_NUMERICSERV;

    int err = getaddrinfo(url.c_str(), port.c_str(), &hints, &result);
    if (err != 0) {
        throw strerror(err);
    }

    int raw_fd = socket(result->ai_family, result->ai_socktype, 0);
    if (raw_fd == -1) {
        throw strerror(errno);
    }

    err = bind(raw_fd, result->ai_addr, result->ai_addrlen);
    if (err == -1) {
        throw strerror(errno);
    }

    freeaddrinfo(result);

    listen(raw_fd, 5);

    return raw_fd;
}

tcp_server::tcp_server(const string &url, const string &port)
    : fd(get_raw_fd(url, port)) {}

tcp_stream tcp_server::accept() {
    int new_socketfd = ::accept(*fd.fd, nullptr, nullptr);
    if (new_socketfd == -1) {
        throw strerror(errno);
    }
    return tcp_stream(new_socketfd);
}

void socketfd::force_close() {
    if (close(*fd) == -1) {
        throw errno;
    }
}

socketfd::socketfd(int fd)
    : fd(new int(fd), [](int *fd) {
          if (close(*fd) == -1) {
              cerr << "Failed to close socket: " << strerror(errno) << endl;
          }
      }) {}
