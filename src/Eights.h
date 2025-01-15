#pragma once
#include "Card.h"
#include "Hand.h"
#include "Player.h"

struct Eights {
    Player one;
    Player two;
    Hand draw_pile;
    Hand discard_pile;

    // make a new eights game with default settings
    Eights();

    bool is_done();
    void reshuffle();
    Card draw_card();
    Player *next_player(const Player *current);
    void display_state();
    void take_turn(Player *player);
    void play_game();
};
