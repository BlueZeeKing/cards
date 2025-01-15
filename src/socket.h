#include <istream>

class tcp_stream: public std::iostream {
    public:
        tcp_stream(int socketfd);
        tcp_stream(const std::string& url, const std::string& port);
        ~tcp_stream();
};
