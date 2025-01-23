#pragma once

#include <optional>
#include <string>

#include "Card.h"
#include "CardCollection.h"
#include "channel.h"
#include "messages.h"

class Eights;

struct Player {
    std::string name;
    channel<Message> sender;
    channel<Message> receiver;
    CardCollection hand;

    Player(std::string name, channel<Message> sender,
           channel<Message> receiver);

    std::optional<Card> play(Eights &);
    void start(const std::vector<std::string> &players, const Card &discard);
    void add_card();
    void player_finished(const Card &discard);
    void end();
};
