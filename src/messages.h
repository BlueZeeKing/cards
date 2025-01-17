#pragma once

#include <iostream>
#include <variant>

#include "CardCollection.h"

struct StartGame {
    CardCollection hand;
    std::vector<std::string> player_order;
    Card discard;

    bool operator==(const StartGame &) const;
};

struct FinishTurn {
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
    int hand_idx;

    bool operator==(const Play &) const;
};

struct Join {
    std::string name;

    bool operator==(const Join &) const;
};

struct End {
    bool operator==(const End &) const;
};

typedef std::variant<StartGame, FinishTurn, Draw, DrawResult, Play, Join, End>
    Message;

std::istream &operator>>(std::istream &, Message &);
std::ostream &operator<<(std::ostream &, const Message &);
