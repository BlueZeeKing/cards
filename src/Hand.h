#pragma once

#include <string>

#include "CardCollection.h"

struct Hand : CardCollection {
    Hand(std::string label);
};
