#include "ncursesw/ncurses.h"
#include <iostream>
#include <ostream>
#include <string>

#include "CardCollection.h"
#include "randomutils.h"

using namespace std;

CardCollection::CardCollection(string label) {
    this->label = label;
    // this->cards is automatically initialized to an empty vector
}

void CardCollection::add_card(const Card &card) { cards.push_back(card); }

Card CardCollection::pop_card() {
    // remove last card
    Card toRemove = cards.back();
    cards.pop_back();
    return toRemove;
}

Card CardCollection::pop_card(int i) {
    Card toRemove = cards[i];
    cards.erase(cards.begin() + i);
    return toRemove;
}

bool CardCollection::is_empty() const { return cards.empty(); }

int CardCollection::size() const { return cards.size(); }

Card &CardCollection::get_card(int i) { return cards[i]; }

Card &CardCollection::last_card() { return cards.back(); }

void CardCollection::swap_cards(int i, int j) {
    Card temp = cards[i];
    cards[i] = cards[j];
    cards[j] = temp;
}

void CardCollection::shuffle() {
    for (int i = cards.size() - 1; i > 0; i--) {
        int j = random_between(0, i + 1);
        swap_cards(i, j);
    }
}

void CardCollection::deal(CardCollection &that, int n) {
    for (int i = 0; i < n; i++) {
        Card card = pop_card();
        that.add_card(card);
    }
}

void CardCollection::deal_all(CardCollection &that) {
    int n = cards.size();
    deal(that, n);
}

void CardCollection::display(int row, int col, int limit, bool vertical) const {
    bool limited = false;

    if (limit < 0) {
        limit = cards.size();
    } else if (limit > cards.size()) {
        limit = cards.size();
    } else {
        limited = true;
    }

    for (int i = 0; i < limit; i++) {
        cards[i].display(row, col);
        if (vertical) {
            row += 5;
        } else {
            col += 5;
        }
    }

    if (limited && vertical) {
        color_set(COLOR_CARD_BLACK, nullptr);
        mvaddch(row - 1, col, '+');
    } else if (limited) {
        color_set(COLOR_CARD_BLACK, nullptr);
        mvaddch(row, col - 1, '+');
    }
}

istream &operator>>(istream &stream, CardCollection &collection) {
    int length;
    char byte;
    Card card;

    collection.cards.clear();

    while (stream.peek() != '\n') {
        stream >> card;
        collection.add_card(card);
    }

    stream.ignore();

    return stream;
}

ostream &operator<<(ostream &stream, const CardCollection &collection) {
    for (auto card = collection.cards.begin(); card < collection.cards.end();
         card++) {
        stream << *card;
    }
    stream << '\n';
    return stream;
}

bool CardCollection::operator==(const CardCollection &other) const {
    if (other.cards.size() != cards.size()) {
        return false;
    }

    for (int i = 0; i < cards.size(); i++) {
        if (other.cards[i] != cards[i]) {
            return false;
        }
    }

    return true;
}
bool CardCollection::operator!=(const CardCollection &other) const {
    return !(*this == other);
}
