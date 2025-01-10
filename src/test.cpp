#include "CardCollection.h"
#include "Deck.h"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "Card.h"

TEST_CASE("card binary") {
    CHECK(Card(Card(0, 1).to_binary()).equals(Card(0, 1)));
    CHECK(Card(Card(0, 2).to_binary()).equals(Card(0, 2)));
    CHECK(Card(Card(0, 3).to_binary()).equals(Card(0, 3)));
    CHECK(Card(Card(1, 4).to_binary()).equals(Card(1, 4)));
    CHECK(Card(Card(1, 5).to_binary()).equals(Card(1, 5)));
    CHECK(Card(Card(1, 6).to_binary()).equals(Card(1, 6)));
    CHECK(Card(Card(2, 7).to_binary()).equals(Card(2, 7)));
    CHECK(Card(Card(2, 8).to_binary()).equals(Card(2, 8)));
    CHECK(Card(Card(2, 9).to_binary()).equals(Card(2, 9)));
    CHECK(Card(Card(3, 10).to_binary()).equals(Card(3, 10)));
    CHECK(Card(Card(3, 11).to_binary()).equals(Card(3, 11)));
    CHECK(Card(Card(3, 12).to_binary()).equals(Card(3, 12)));
}

TEST_CASE("card collection binary") {
    Deck deck("Main");
    vector<unsigned char> data = deck.to_bytes();

    CardCollection decoded_deck("decoded");
    decoded_deck.from_bytes(data);

    CHECK(deck.size() == decoded_deck.size());

    for (int i = 0; i < deck.size(); i++) {
        CHECK(deck.get_card(i).equals(decoded_deck.get_card(i)));
    }
}
