#include "CardCollection.h"
#include "Deck.h"
#include "Hand.h"
#include "randomutils.h"
#include <iostream>
#include <istream>
#include <sstream>
#include <thread>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "messages.h"
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
    stringstream stream;
    Card parsed;
    for (auto card = deck.cards.begin(); card < deck.cards.end(); card++) {
        stream << *card;
        stream >> parsed;
        CHECK(parsed == *card);
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

    CHECK(deck == collection);
}

void test_message(Message message, iostream &stream) {
    stream << message;
    stream.sync();
    Message output(Draw{});
    stream >> output;
    CHECK(message == output);
}

void test_all_messages(iostream &stream) {
    Deck deck("deck");
    Hand hand("hand");
    deck.deal(hand, 10);
    vector<string> player_order = {"first", "second", "third"};

    test_message(StartGame{.hand = hand,
                           .player_order = player_order,
                           .discard = Card()},
                 stream);
    test_message(
        AddCard{
            .idx = 42,
        },
        stream);
    test_message(FinishTurn{.idx = 4242, .new_discard = Card(1, 1)}, stream);
    test_message(Draw{}, stream);
    test_message(DrawResult{.card = Card(2, 4)}, stream);
    test_message(Play{.card = Card(1, 10)}, stream);
    test_message(Join{.name = "hello, world"}, stream);
}

TEST_CASE("Test message serialization and deserialization") {
    stringstream stream;
    test_all_messages(stream);
}

TEST_CASE("Test message serialization and deserialization over network" *
          doctest::skip(true)) {
    tcp_stream stream("tcpbin.com", "4242");
    test_all_messages(stream);
}
