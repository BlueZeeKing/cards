#include "messages.h"
#include <string>

istream &operator>>(istream &stream, Message &message) { return stream; }

ostream &operator<<(ostream &stream, Message &message) { return stream; }

bool StartGame::operator==(const StartGame &other) const {
    return other.discard == discard && other.hand == hand &&
           other.player_order == player_order;
}

bool AddCard::operator==(const AddCard &other) const {
    return other.idx == idx;
}

bool FinishTurn::operator==(const FinishTurn &other) const {
    return other.idx == idx && other.new_discard == new_discard;
}

bool Draw::operator==(const Draw &other) const { return true; }

bool DrawResult::operator==(const DrawResult &other) const {
    return other.card == card;
}

bool Play::operator==(const Play &other) const { return other.card == card; }

bool Join::operator==(const Join &other) const { return other.name == name; }
