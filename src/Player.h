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
    Eights *eights;

    Player(std::string name, channel<Message> sender,
           channel<Message> receiver);

    std::optional<Card> play();
    void start();
    void add_card();
    void player_finished();
};
