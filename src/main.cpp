#include "Card.h"
#include "Eights.h"
#include <ncurses.h>
#include "randomutils.h"
using namespace std;

int main(int argc, char* argv[]) {
    init_random_seed();

    // Setup ncurses
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    start_color();
    timeout(-1);
    curs_set(0);
    keypad(stdscr, true);

    init_pair(COLOR_CARD_RED, COLOR_WHITE, COLOR_RED);
    init_pair(COLOR_CARD_BLACK, COLOR_WHITE, COLOR_BLACK);
    
    Eights game = Eights();
    game.play_game();

    endwin();
}
