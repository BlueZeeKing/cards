#include "Player.h"
#include "Hand.h"
#include "Eights.h"
#include "ncursesw/ncurses.h"
using namespace std;

Card Player::play(Eights& eights, const Card& prev)
{
    draw_cursor(0);

    while(true) {
        switch (getch()) {
        case KEY_RIGHT:
            if (cursor_idx < hand.size() - 1) {
                draw_cursor(cursor_idx + 1);
            }
            break;
        case KEY_LEFT:
            if (cursor_idx > 0) {
                draw_cursor(cursor_idx - 1);
            }
            break;
        case 'd':
            hand.add_card(eights.draw_card());

            clear();
            eights.display_state();

            draw_cursor(cursor_idx);

            break;
        case KEY_UP:
            if (!is_top && card_matches(hand.get_card(cursor_idx), prev)) {
                return hand.pop_card(cursor_idx);
            }
            break;
        case KEY_DOWN:
            if (is_top && card_matches(hand.get_card(cursor_idx), prev)) {
                return hand.pop_card(cursor_idx);
            }
            break;
        case ' ':
        case '\n':
            if (card_matches(hand.get_card(cursor_idx), prev)) {
                return hand.pop_card(cursor_idx);
            }
            break;
        }
        refresh();
    }
}

void Player::draw_cursor(int new_idx) {
    color_set(COLOR_CARD_BLACK, nullptr);
    int start_col = getmaxx(stdscr) / 2 - (hand.size() * 5) / 2;

    if (cursor_idx != -1) {
        mvaddstr(is_top ? 0 : getmaxy(stdscr) - 1, start_col + cursor_idx * 5, "  ");
    }

    cursor_idx = new_idx;

    if (cursor_idx != -1) {
        mvaddstr(is_top ? 0 : getmaxy(stdscr) - 1, start_col + cursor_idx * 5, "==");
    }
}

Card Player::search_for_match(const Card& prev)
{
    for (int i = 0; i < hand.size(); i++) {
        Card card = hand.get_card(i);
        if (card_matches(card, prev)) {
            return hand.pop_card(i);
        }
    }
    throw "No match found";
}

Card Player::draw_for_match(Eights& eights, const Card& prev)
{
    while (true) {
        Card card = eights.draw_card();
        if (card_matches(card, prev)) {
            return card;
        }
        hand.add_card(card);
    }
}

bool Player::card_matches(const Card& card1, const Card& card2)
{
    return card1.suit == card2.suit
        || card1.rank == card2.rank
        || card1.rank == 8;
}

void Player::display()
{
    
    hand.display(is_top ? 2 : getmaxy(stdscr) - 4, getmaxx(stdscr) / 2 - (hand.size() * 5) / 2);
}
