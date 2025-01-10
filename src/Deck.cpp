#include "Deck.h"
#include "Card.h"
#include <string>
#include <iostream>
using namespace std;

Deck::Deck(string label) : CardCollection(label)
{
    for (int suit = 0; suit <= 3; suit++) {
        for (int rank = 1; rank <= 13; rank++) {
            add_card(Card(suit, rank));
        }
    }
}