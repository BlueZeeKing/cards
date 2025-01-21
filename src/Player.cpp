#include "Player.h"
#include "CardCollection.h"
#include "Eights.h"
#include "messages.h"
#include <optional>

using namespace std;

Player::Player(string name, channel<Message> sender, channel<Message> receiver)
    : hand(CardCollection(name)), sender(sender), receiver(receiver),
      name(name) {}

optional<Card> Player::play(Eights &eights) {
    Message message(Draw{});
    while (true) {
        message = receiver.recv();
        if (message.index() == 4) {
            Play inner = get<Play>(message);
            Card played = hand.pop_card(inner.hand_idx);

            if (!card_matches(played, eights.current_top_card())) {
                throw "Client played invalid move";
            }

            return played;
        } else if (message.index() == 2) {
            sender.send(Message(DrawResult{.card = eights.draw_card(*this)}));
        } else if (message.index() == 6) {
            return optional<Card>();
        } else {
            throw "Invalid message";
        }
    }
}

void Player::add_card() { sender.send(Message(Draw{})); }

void Player::player_finished(const Card &discard) {
    sender.send(Message(FinishTurn{.new_discard = discard}));
}

void Player::start(const vector<string> &players, const Card &discard) {
    sender.send(Message(StartGame{
        .hand = hand,
        .player_order = players,
        .discard = discard,
    }));
}
