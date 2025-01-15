#include "CardCollection.h"
#include "Deck.h"
#include "randomutils.h"
#include <bitset>
#include <iostream>
#include <sstream>
#include <thread>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "socket.h"
using namespace std;

const char *TEST_STRING =
    "Hello, world! This sends data to a tcp echo server and tests to make sure "
    "the buffer refills for sending and receiving properly.";

TEST_CASE("Test tcp socket" * doctest::skip(true)) {
    tcp_stream stream("tcpbin.com", "4242");
    stream.rdbuf()->pubsetbuf(nullptr, 10);
    stream << TEST_STRING << endl;
    string output;
    std::getline(stream, output);
    CHECK(output == TEST_STRING);
}

TEST_CASE("Test tcp socket and server" * doctest::skip(true)) {
    std::thread server_thread([]() {
        try {
            tcp_server server("127.0.0.1", "8678");
            tcp_stream stream = server.accept();

            string received;
            std::getline(stream, received);
            CHECK(received == "Client to server");

            stream << "Server to client" << endl;
        } catch (char *err) {
            cerr << "server error: " << err << endl;
            throw err;
        }
    });

    std::thread client_thread([]() {
        try {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            tcp_stream stream("127.0.0.1", "8678");

            stream << "Client to server" << endl;

            string received;
            std::getline(stream, received);
            CHECK(received == "Server to client");
        } catch (char *err) {
            cerr << "client error: " << err << endl;
            throw err;
        }
    });

    client_thread.join();
    server_thread.join();
}

TEST_CASE("Test card serialization and deserialization") {
    Deck deck("deck");
    for (auto card = deck.cards.begin(); card < deck.cards.end(); card++) {
        CHECK(Card(card->to_byte()) == *card);
    }
}

TEST_CASE("Test deck serialization and deserialization") {
    init_random_seed();
    Deck deck("deck");
    deck.shuffle();
    stringstream stream;
    stream << deck;
    CardCollection collection("output");
    stream >> collection;

    CHECK(deck.size() == collection.size());
    for (int i = 0; i < deck.size(); i++) {
        CHECK(deck.get_card(i) == collection.get_card(i));
    }
}
