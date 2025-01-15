#pragma once

#include <iostream>
#include <string>

#define COLOR_CARD_RED 1
#define COLOR_CARD_BLACK 2

struct Card {
    int suit, rank;

    // constructors
    Card();
    Card(int s, int r);

    // member functions
    std::string to_string() const;
    bool operator==(const Card &c2) const;
    bool operator!=(const Card &c2) const;
    bool is_greater(const Card &c2) const;
    void display(int row, int col) const;
};

std::istream &operator>>(std::istream &, Card &);
std::ostream &operator<<(std::ostream &, const Card &);
