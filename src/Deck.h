#pragma once

#include <string>

#include "CardCollection.h"

struct Deck : CardCollection {
    Deck(std::string label);
};
