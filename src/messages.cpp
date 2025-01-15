#include "messages.h"
#include <iostream>
#include <string>

istream &operator>>(istream &stream, Message &message) {
    int index, length;
    char byte;
    string line;

    getline(stream, line);
    index = std::stoi(line);
    switch (index) {
    case 0: {
        Hand hand("Mine");
        stream >> hand;

        std::getline(stream, line);
        length = stoi(line);
        vector<string> players;
        for (int i = 0; i < length; i++) {
            std::getline(stream, line);
            players.push_back(line);
        }

        std::getline(stream, line);
        Card discard(line[0]);

        message = StartGame{
            .hand = hand,
            .player_order = players,
            .discard = discard,
        };
        break;
    }
    case 1: {
        std::getline(stream, line);
        int idx = stoi(line);
        message = AddCard{
            .idx = idx,
        };
        break;
    }
    case 2: {
        std::getline(stream, line);
        int idx = stoi(line);
        std::getline(stream, line);
        Card card(line[0]);
        message = FinishTurn{
            .idx = idx,
            .new_discard = card,
        };
        break;
    }
    case 3:
        message = Draw{};
        break;
    case 4: {
        std::getline(stream, line);
        Card card(line[0]);
        message = DrawResult{
            .card = card,
        };
        break;
    }
    case 5: {
        std::getline(stream, line);
        Card card(line[0]);
        message = Play{
            .card = card,
        };
        break;
    }
    case 6: {
        getline(stream, line);
        message = Join{
            .name = line,
        };
        break;
    }
    }

    return stream;
}

ostream &operator<<(ostream &stream, Message &message) {
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

        stream << inner.discard.to_byte() << '\n';
        break;
    }
    case 1: {
        AddCard inner = get<AddCard>(message);
        stream << inner.idx << '\n';
        break;
    }
    case 2: {
        FinishTurn inner = get<FinishTurn>(message);
        stream << inner.idx << '\n';
        stream << inner.new_discard.to_byte() << '\n';
        break;
    }
    case 3:
        break;
    case 4: {
        DrawResult inner = get<DrawResult>(message);
        stream << inner.card.to_byte() << '\n';
        break;
    }
    case 5: {
        Play inner = get<Play>(message);
        stream << inner.card.to_byte() << '\n';
        break;
    }
    case 6: {
        Join inner = get<Join>(message);
        stream << inner.name << '\n';
        break;
    }
    }

    return stream;
}

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
