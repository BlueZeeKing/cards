#include <istream>

class tcp_stream : public std::iostream {
  public:
    tcp_stream(int socketfd);
    tcp_stream(const std::string &url, const std::string &port);
    ~tcp_stream();
};

class tcp_server {
    int socketfd;

  public:
    tcp_server(const std::string &url, const std::string &port);
    ~tcp_server();
    tcp_stream accept();
};
