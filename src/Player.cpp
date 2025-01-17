#include "Player.h"
#include "CardCollection.h"
#include "Eights.h"
#include "messages.h"
#include <optional>

using namespace std;

Player::Player(string name, channel<Message> sender, channel<Message> receiver)
    : hand(CardCollection(name)), sender(sender), receiver(receiver),
      eights(nullptr), name(name) {}

optional<Card> Player::play() {
    Message message(Draw{});
    while (true) {
        message = receiver.recv();
        if (message.index() == 4) {
            Play inner = get<Play>(message);
            Card played = hand.pop_card(inner.hand_idx);

            if (!card_matches(played, eights->current_top_card())) {
                throw "Client played invalid move";
            }

            // NOTE: This is a lot of coupling to the eights class
            for (auto player = eights->players.begin();
                 player < eights->players.end(); player++) {
                player->player_finished();
            }

            return played;
        } else if (message.index() == 2) {
            sender.send(Message(DrawResult{.card = eights->draw_card()}));
            for (auto player = eights->players.begin();
                 player < eights->players.end(); player++) {
                player->add_card();
            }
        } else if (message.index() == 6) {
            return optional<Card>();
        } else {
            throw "Invalid message";
        }
    }
}

void Player::add_card() { sender.send(Message(Draw{})); }

void Player::player_finished() {
    sender.send(Message(FinishTurn{.new_discard = eights->current_top_card()}));
}

void Player::start() {
    vector<string> players;
    for (auto player = eights->players.begin(); player < eights->players.end();
         player++) {
        players.push_back(player->name);
    }

    sender.send(Message(StartGame{
        .hand = hand,
        .player_order = players,
        .discard = eights->current_top_card(),
    }));
}
