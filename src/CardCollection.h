#pragma once
#include <string>
#include <vector>
#include "Card.h"
using namespace std;

struct CardCollection {
    string label;
    vector<Card> cards;

    CardCollection(string label);

    void add_card(const Card& card);
    Card pop_card();
    Card pop_card(int i);
    bool is_empty();
    int size();
    Card& get_card(int i);
    Card& last_card();
    void swap_cards(int i, int j);
    void shuffle();
    void deal(CardCollection& that, int n);
    void deal_all(CardCollection& that);
    void display(int row, int col, int limit = -1, bool vertical = false);
};
