#include <variant>

#include "Hand.h"

struct StartGame {
    Hand hand;
    vector<string> player_order;
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
    string name;

    bool operator==(const Join &) const;
};

typedef std::variant<StartGame, AddCard, FinishTurn, Draw, DrawResult, Play,
                     Join>
    Message;

istream &operator>>(istream &, Message &);
ostream &operator<<(ostream &, Message &);
