#include "ncursesw/ncurses.h"
#include <iostream>
#include <string>

#include "Card.h"

using namespace std;

Card::Card() {
    suit = 1;
    rank = 1;
}

Card::Card(int s, int r) {
    suit = s;
    rank = r;
}

string Card::to_string() const {
    string rank_strings[] = {"",  "Ace", "2", "3",  "4",    "5",     "6",
                             "7", "8",   "9", "10", "Jack", "Queen", "King"};
    string suit_strings[] = {"Clubs", "Diamonds", "Hearts", "Spades"};

    return rank_strings[rank] + " of " + suit_strings[suit];
}

bool Card::operator==(const Card &c2) const {
    return (suit == c2.suit && rank == c2.rank);
}

bool Card::operator!=(const Card &c2) const { return !(*this == c2); }

bool Card::is_greater(const Card &c2) const {
    // first check the suit_strings
    if (suit > c2.suit)
        return true;
    if (suit < c2.suit)
        return false;

    // if suit_strings are equal, check rank_strings
    if (rank > c2.rank)
        return true;
    if (rank < c2.rank)
        return false;

    // if rank_strings are equal too, 1st card is not greater than the 2nd
    return false;
}

void Card::display(int row, int col) const {
    switch (suit) {
    case 0:
        color_set(COLOR_CARD_BLACK, nullptr);
        mvaddstr(row + 1, col, "♣ ");
        break;
    case 1:
        color_set(COLOR_CARD_RED, nullptr);
        mvaddstr(row + 1, col, "♦ ");
        break;
    case 2:
        color_set(COLOR_CARD_RED, nullptr);
        mvaddstr(row + 1, col, "♥ ");
        break;
    case 3:
        color_set(COLOR_CARD_BLACK, nullptr);
        mvaddstr(row + 1, col, "♠ ");
        break;
    }

    mvvline(row, col - 1, 0, 2);
    mvvline(row, col + 2, 0, 2);
    mvhline(row - 1, col, 0, 2);
    mvhline(row + 2, col, 0, 2);
    mvaddch(row - 1, col - 1, ACS_ULCORNER);
    mvaddch(row - 1, col + 2, ACS_URCORNER);
    mvaddch(row + 2, col - 1, ACS_LLCORNER);
    mvaddch(row + 2, col + 2, ACS_LRCORNER);

    switch (rank) {
    case 1:
        mvaddstr(row, col, "A ");
        break;
    case 2:
        mvaddstr(row, col, "2 ");
        break;
    case 3:
        mvaddstr(row, col, "3 ");
        break;
    case 4:
        mvaddstr(row, col, "4 ");
        break;
    case 5:
        mvaddstr(row, col, "5 ");
        break;
    case 6:
        mvaddstr(row, col, "6 ");
        break;
    case 7:
        mvaddstr(row, col, "7 ");
        break;
    case 8:
        mvaddstr(row, col, "8 ");
        break;
    case 9:
        mvaddstr(row, col, "9 ");
        break;
    case 10:
        mvaddstr(row, col, "10");
        break;
    case 11:
        mvaddstr(row, col, "J ");
        break;
    case 12:
        mvaddstr(row, col, "Q ");
        break;
    case 13:
        mvaddstr(row, col, "K ");
        break;
    }
}

istream &operator>>(istream &stream, Card &card) {
    char suit = stream.get();
    char rank = stream.get();

    switch (suit) {
    case 'C':
        card.suit = 0;
        break;
    case 'D':
        card.suit = 1;
        break;
    case 'H':
        card.suit = 2;
        break;
    case 'S':
        card.suit = 3;
        break;
    }

    switch (rank) {
    case 'A':
        card.rank = 1;
        break;
    case '2':
        card.rank = 2;
        break;
    case '3':
        card.rank = 3;
        break;
    case '4':
        card.rank = 4;
        break;
    case '5':
        card.rank = 5;
        break;
    case '6':
        card.rank = 6;
        break;
    case '7':
        card.rank = 7;
        break;
    case '8':
        card.rank = 8;
        break;
    case '9':
        card.rank = 9;
        break;
    case 'T':
        card.rank = 10;
        break;
    case 'J':
        card.rank = 11;
        break;
    case 'Q':
        card.rank = 12;
        break;
    case 'K':
        card.rank = 13;
        break;
    }

    return stream;
}

ostream &operator<<(ostream &stream, const Card &card) {
    switch (card.suit) {
    case 0:
        stream << 'C';
        break;
    case 1:
        stream << 'D';
        break;
    case 2:
        stream << 'H';
        break;
    case 3:
        stream << 'S';
        break;
    }

    switch (card.rank) {
    case 1:
        stream << 'A';
        break;
    case 2:
        stream << '2';
        break;
    case 3:
        stream << '3';
        break;
    case 4:
        stream << '4';
        break;
    case 5:
        stream << '5';
        break;
    case 6:
        stream << '6';
        break;
    case 7:
        stream << '7';
        break;
    case 8:
        stream << '8';
        break;
    case 9:
        stream << '9';
        break;
    case 10:
        stream << 'T';
        break;
    case 11:
        stream << 'J';
        break;
    case 12:
        stream << 'Q';
        break;
    case 13:
        stream << 'K';
        break;
    }

    return stream;
}
