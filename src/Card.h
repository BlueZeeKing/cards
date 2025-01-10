#pragma once
#include <string>
using namespace std;

#define COLOR_CARD_RED 1
#define COLOR_CARD_BLACK 2

struct Card
{
    int suit, rank;

    // constructors
    Card();
    Card(int s, int r);

    // member functions
    string to_string() const;
    bool equals(const Card& c2) const;
    bool is_greater(const Card& c2) const;
    void display(int row, int col);
};
