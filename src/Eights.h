#pragma once

#include <optional>

#include "Card.h"
#include "CardCollection.h"
#include "Player.h"

struct Eights {
    std::vector<Player> players;
    CardCollection draw_pile;
    CardCollection discard_pile;
    int current_player;

    // make a new eights game with default settings
    Eights(std::vector<Player>);

    std::optional<Player> get_winner();
    void refill_draw_pile();
    Player &next_player();
    Card draw_card();
    Card current_top_card();
    void play_game();
};

bool card_matches(const Card &card1, const Card &card2);
