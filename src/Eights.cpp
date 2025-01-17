#include <optional>
#include <vector>

#include "Card.h"
#include "Deck.h"
#include "Eights.h"
#include "Player.h"

using namespace std;

Eights::Eights(vector<Player> players)
    : discard_pile("Discard pile"), draw_pile("Draw pile") {
    Deck deck = Deck("Deck");
    deck.shuffle();

    // deal starting cards to each player
    for (auto player = players.begin(); player < players.end(); player++) {
        deck.deal(player->hand, 5);
        player->eights = this;
    }

    // place top card in discard pile
    deck.deal(discard_pile, 1);

    // place remaining cards in draw pile
    deck.deal_all(draw_pile);

    current_player = 0;
    this->players = players;
}

optional<Player> Eights::get_winner() {
    for (auto player = players.begin(); player < players.end(); player++) {
        if (player->hand.is_empty()) {
            return optional<Player>(*player);
        }
    }

    return optional<Player>();
}

void Eights::refill_draw_pile() {
    Card prev = discard_pile.pop_card();
    discard_pile.deal_all(draw_pile);
    discard_pile.add_card(prev);
    draw_pile.shuffle();
}

Card Eights::draw_card() {
    if (draw_pile.is_empty()) {
        refill_draw_pile();
    }
    return draw_pile.pop_card();
}

Player &Eights::next_player() {
    Player &next_player = players[current_player];

    current_player += 1;
    current_player %= players.size();

    return next_player;
}

Card Eights::current_top_card() { return discard_pile.last_card(); }

void Eights::play_game() {
    for (auto player = players.begin(); player < players.end(); player++) {
        player->start();
    }
    while (!get_winner().has_value()) {
        optional<Card> next_card = next_player().play();
        if (next_card.has_value()) {
            discard_pile.add_card(next_card.value());
        } else {
            break;
        }
    }
}

bool card_matches(const Card &card1, const Card &card2) {
    return card1.suit == card2.suit || card1.rank == card2.rank ||
           card1.rank == 8;
}
