#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "socket.h"
using namespace std;

const char *TEST_STRING =
    "Hello, world! This sends data to a tcp echo server and tests to make sure "
    "the buffer refills for sending and receiving properly.";

TEST_CASE("Test tcp socket") {
    tcp_stream stream("tcpbin.com", "4242");
    stream.rdbuf()->pubsetbuf(nullptr, 10);
    stream << TEST_STRING << endl;
    string output;
    std::getline(stream, output);
    CHECK(output == TEST_STRING);
}
