#include "Eights.h"
#include "channel.h"
#include <chrono>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include <iostream>
#include <sstream>
#include <thread>

#include "CardCollection.h"
#include "Deck.h"
#include "messages.h"
#include "randomutils.h"
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
    getline(stream, output);
    CHECK(output == TEST_STRING);
}

TEST_CASE("Test tcp socket and server" * doctest::skip(true)) {
    thread server_thread([]() {
        try {
            tcp_server server("127.0.0.1", "8678");
            tcp_stream stream = server.accept();

            string received;
            getline(stream, received);
            CHECK(received == "Client to server");

            stream << "Server to client" << endl;
        } catch (char *err) {
            cerr << "server error: " << err << endl;
            throw err;
        }
    });

    thread client_thread([]() {
        try {
            this_thread::sleep_for(chrono::seconds(1));
            tcp_stream stream("127.0.0.1", "8678");

            stream << "Client to server" << endl;

            string received;
            getline(stream, received);
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
    CardCollection cards("hand");
    deck.deal(cards, 10);
    vector<string> player_order = {"first", "second", "third"};

    test_message(StartGame{.hand = cards,
                           .player_order = player_order,
                           .discard = Card()},
                 stream);
    test_message(FinishTurn{.new_discard = Card(1, 1)}, stream);
    test_message(Draw{}, stream);
    test_message(DrawResult{.card = Card(2, 4)}, stream);
    test_message(Play{.hand_idx = 2}, stream);
    test_message(Join{.name = "hello, world"}, stream);
    test_message(End{}, stream);
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

TEST_CASE("Test server") {
    channel<Message> one_sender;
    channel<Message> one_receiver;
    channel<Message> two_sender;
    channel<Message> two_receiver;

    thread player_one_thread([one_sender, one_receiver]() mutable {
        Message msg = one_sender.recv();
        CHECK(msg.index() == 0);
        StartGame msg_inner = get<StartGame>(msg);
        CHECK(msg_inner.player_order[0] == "one");
        CHECK(msg_inner.player_order[1] == "two");
        CHECK(msg_inner.hand.size() == 5);
        one_receiver.send(End{});
    });

    thread player_two_thread([two_sender, two_receiver]() mutable {
        Message msg = two_sender.recv();
        CHECK(msg.index() == 0);
        StartGame msg_inner = get<StartGame>(msg);
        CHECK(msg_inner.player_order[0] == "one");
        CHECK(msg_inner.player_order[1] == "two");
        CHECK(msg_inner.hand.size() == 5);
    });

    vector<Player> players;
    players.push_back(Player("one", one_sender, one_receiver));
    players.push_back(Player("two", two_sender, two_receiver));

    Eights eights(players);

    eights.play_game();

    player_one_thread.join();
    player_two_thread.join();
}

TEST_CASE("channel") {
    channel<int> sender;
    channel<int> receiver(sender);

    thread receiver_thread(
        [receiver]() mutable { CHECK(receiver.recv() == 42); });

    thread sender_thread([sender]() mutable {
        this_thread::sleep_for(chrono::milliseconds(50));
        sender.send(42);
    });

    sender_thread.join();
    receiver_thread.join();
}
