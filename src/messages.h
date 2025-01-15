#pragma once

#include <iostream>
#include <variant>

#include "Hand.h"

struct StartGame {
    Hand hand;
    std::vector<std::string> player_order;
    Card discard;

    bool operator==(const StartGame &) const;
};

struct AddCard {
    int idx;

    bool operator==(const AddCard &) const;
};

struct FinishTurn {
    int idx;
    Card new_discard;

    bool operator==(const FinishTurn &) const;
};

struct Draw {
    bool operator==(const Draw &) const;
};

struct DrawResult {
    Card card;

    bool operator==(const DrawResult &) const;
};

struct Play {
    Card card;

    bool operator==(const Play &) const;
};

struct Join {
    std::string name;

    bool operator==(const Join &) const;
};

typedef std::variant<StartGame, AddCard, FinishTurn, Draw, DrawResult, Play,
                     Join>
    Message;

std::istream &operator>>(std::istream &, Message &);
std::ostream &operator<<(std::ostream &, const Message &);
