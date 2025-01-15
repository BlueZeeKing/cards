#pragma once
#include "Card.h"
#include "Hand.h"
#include <string>
using namespace std;

class Eights;

struct Player {
    string name;
    Hand hand;
    int cursor_idx;
    bool is_top;

    Player(string name, bool is_top)
        : name(name), hand(Hand(name)), cursor_idx(-1), is_top(is_top) {};

    Card play(Eights &eights, const Card &prev);
    void draw_cursor(int new_idx);
    Card search_for_match(const Card &prev);
    Card draw_for_match(Eights &eights, const Card &prev);
    bool card_matches(const Card &card1, const Card &card2);
    void display();
};
