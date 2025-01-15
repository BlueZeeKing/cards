#pragma once
#include "Card.h"
#include <istream>
#include <string>
#include <vector>

struct CardCollection {
    std::string label;
    std::vector<Card> cards;

    CardCollection(std::string label);

    void add_card(const Card &card);
    Card pop_card();
    Card pop_card(int i);
    bool is_empty() const;
    int size() const;
    Card &get_card(int i);
    Card &last_card();
    void swap_cards(int i, int j);
    void shuffle();
    void deal(CardCollection &that, int n);
    void deal_all(CardCollection &that);
    void display(int row, int col, int limit = -1, bool vertical = false) const;
    bool operator==(const CardCollection &) const;
    bool operator!=(const CardCollection &) const;
};

std::istream &operator>>(std::istream &, CardCollection &);
std::ostream &operator<<(std::ostream &, const CardCollection &);
