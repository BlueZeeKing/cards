#include <cassert>
#include <iostream>
#include <string>

#include "CardCollection.h"
#include "messages.h"

using namespace std;

istream &operator>>(istream &stream, Message &message) {
    int index, length;
    char byte;
    string line;

    getline(stream, line);
    index = stoi(line);
    switch (index) {
    case 0: {
        CardCollection hand("Hand");
        stream >> hand;

        getline(stream, line);
        length = stoi(line);
        vector<string> players;
        for (int i = 0; i < length; i++) {
            getline(stream, line);
            players.push_back(line);
        }

        Card discard;
        stream >> discard;
        assert(stream.get() == '\n');

        message = StartGame{
            .hand = hand,
            .player_order = players,
            .discard = discard,
        };
        break;
    }
    case 1: {
        Card card;
        stream >> card;
        assert(stream.get() == '\n');
        message = FinishTurn{
            .new_discard = card,
        };
        break;
    }
    case 2:
        message = Draw{};
        break;
    case 3: {
        Card card;
        stream >> card;
        assert(stream.get() == '\n');
        message = DrawResult{
            .card = card,
        };
        break;
    }
    case 4: {
        getline(stream, line);
        int idx = stoi(line);
        message = Play{
            .hand_idx = idx,
        };
        break;
    }
    case 5: {
        getline(stream, line);
        message = Join{
            .name = line,
        };
        break;
    }
    case 6:
        message = End{};
        break;
    }

    return stream;
}

ostream &operator<<(ostream &stream, const Message &message) {
    stream << (int)message.index() << '\n';
    switch (message.index()) {
    case 0: {
        StartGame inner = get<StartGame>(message);
        stream << inner.hand;
        stream << (int)inner.player_order.size() << '\n';

        vector<string> players;
        for (auto player = inner.player_order.begin();
             player < inner.player_order.end(); player++) {
            stream << *player << '\n';
        }

        stream << inner.discard << '\n';
        break;
    }
    case 1: {
        FinishTurn inner = get<FinishTurn>(message);
        stream << inner.new_discard << '\n';
        break;
    }
    case 2:
        break;
    case 3: {
        DrawResult inner = get<DrawResult>(message);
        stream << inner.card << '\n';
        break;
    }
    case 4: {
        Play inner = get<Play>(message);
        stream << inner.hand_idx << '\n';
        break;
    }
    case 5: {
        Join inner = get<Join>(message);
        stream << inner.name << '\n';
        break;
    }
    case 6:
        break;
    }

    return stream;
}

bool StartGame::operator==(const StartGame &other) const {
    return other.discard == discard && other.hand == hand &&
           other.player_order == player_order;
}

bool FinishTurn::operator==(const FinishTurn &other) const {
    return other.new_discard == new_discard;
}

bool Draw::operator==(const Draw &other) const { return true; }

bool DrawResult::operator==(const DrawResult &other) const {
    return other.card == card;
}

bool Play::operator==(const Play &other) const {
    return other.hand_idx == hand_idx;
}

bool Join::operator==(const Join &other) const { return other.name == name; }

bool End::operator==(const End &other) const { return true; }
