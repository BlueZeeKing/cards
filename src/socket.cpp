#include <cerrno>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <streambuf>
#include <unistd.h>

#include "socket.h"

class tcp_streambuffer : public std::streambuf {
    int socketfd;
    char *read_buffer;
    int read_buffer_length;
    char *write_buffer;
    int write_buffer_length;

  public:
    tcp_streambuffer(int socketfd) {
        this->socketfd = socketfd;
        read_buffer_length = 2048;
        read_buffer = new char[read_buffer_length];
        write_buffer_length = 2048;
        write_buffer = new char[write_buffer_length];
        setg(read_buffer, read_buffer, read_buffer);
        setp(write_buffer, write_buffer + write_buffer_length);
    }

    virtual int underflow() {
        int amount_read = recv(socketfd, read_buffer, read_buffer_length, 0);

        if (amount_read < 0) {
            setg(read_buffer, read_buffer, read_buffer);
            throw strerror(errno);
        }

        if (amount_read == 0) {
            setg(read_buffer, read_buffer, read_buffer);
            return EOF;
        }

        setg(read_buffer, read_buffer, read_buffer + amount_read);
        return read_buffer[0];
    }

    virtual int overflow(int c) {
        sync();

        if (c != EOF) {
            sputc(c);
        }

        return c;
    }

    virtual int sync() {
        int amount_to_write = pptr() - pbase();
        int amount_written = send(socketfd, write_buffer, amount_to_write, 0);
        if (amount_written != amount_to_write) {
            throw strerror(errno);
        }

        setp(write_buffer, write_buffer + write_buffer_length);

        return 0;
    }

    virtual std::streambuf *setbuf(char *s, std::streamsize n) {
        sync();
        read_buffer_length = n;
        read_buffer = new char[read_buffer_length];
        write_buffer_length = n;
        write_buffer = new char[write_buffer_length];
        setg(read_buffer, read_buffer, read_buffer);
        setp(write_buffer, write_buffer + write_buffer_length);
        return this;
    }

    virtual ~tcp_streambuffer() {
        delete[] read_buffer;
        delete[] write_buffer;
        if (close(socketfd) == -1) {
            std::cerr << "Failed to close socket: " << strerror(errno)
                      << std::endl;
        }
    }
};

int open_client_socket(const std::string &url, const std::string &port) {
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
    : std::iostream(new tcp_streambuffer(socketfd)) {};
tcp_stream::tcp_stream(const std::string &url, const std::string &port)
    : std::iostream(new tcp_streambuffer(open_client_socket(url, port))) {};
tcp_stream::~tcp_stream() { delete this->rdbuf(); };
