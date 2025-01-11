#include "Eights.h"
#include "Hand.h"
#include "Card.h"
#include "Player.h"
#include "ncursesw/ncurses.h"
#include <sstream>
#include "Deck.h"
using namespace std;

Eights::Eights() : one(Player("One", true)), two(Player("Two", false)), draw_pile(Hand("Draw Pile")), discard_pile(Hand("Discard Pile"))
{
    Deck deck = Deck("Deck");
    deck.shuffle();

    // deal starting cards to each player
    deck.deal(one.hand, 5);
    deck.deal(two.hand, 5);

    // place top card in discard pile
    deck.deal(discard_pile, 1);

    // place remaining cards in draw pile
    deck.deal_all(draw_pile);
}

bool Eights::is_done()
{
    return one.hand.is_empty() || two.hand.is_empty();
}

void Eights::reshuffle()
{
    Card prev = discard_pile.pop_card();
    discard_pile.deal_all(draw_pile);
    discard_pile.add_card(prev);
    draw_pile.shuffle();
}

Card Eights::draw_card()
{
    if (draw_pile.is_empty()) {
        reshuffle();
    }
    return draw_pile.pop_card();
}

Player* Eights::next_player(const Player* current)
{
    if (current == (&one)) {
        return &two;
    } else {
        return &one;
    }
}

void Eights::display_state()
{
    clear();

    one.display();
    two.display();

    int center_row = getmaxy(stdscr) / 2;
    int center_col = getmaxx(stdscr) / 2;
    color_set(COLOR_CARD_BLACK, nullptr);
    stringstream message;
    message << "Draw pile: " << draw_pile.size() << " cards";
    mvaddstr(center_row, center_col - 15, message.str().c_str());
    discard_pile.last_card().display(center_row, center_col + 8);

    refresh();
}

void Eights::take_turn(Player* player)
{
    Card prev = discard_pile.last_card();
    Card next = (*player).play(*this, prev);
    discard_pile.add_card(next);
}

void Eights::play_game()
{
    Player* player = &one;
    // keep playing until there's a winner
    while (!is_done()) {
        display_state();
        take_turn(player);
        player = next_player(player);
    }
    // display the final score
    /* one.display(); */
    /* two.display(); */
}
