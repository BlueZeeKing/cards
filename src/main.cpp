#include "channel.h"
#include "client.h"
#include "messages.h"
#include "ncursesw/ncurses.h"

#include "Card.h"
#include "Eights.h"
#include "network.h"
#include "randomutils.h"
#include "socket.h"
#include <iostream>
#include <memory>
#include <optional>
#include <thread>

using namespace std;

void render_start_option(bool is_starting_server) {
    if (is_starting_server) {
        attr_on(A_BOLD, nullptr);
    }
    mvaddstr(1, 1, "start server");
    if (is_starting_server) {
        attr_off(A_BOLD, nullptr);
    }

    if (!is_starting_server) {
        attr_on(A_BOLD, nullptr);
    }
    mvaddstr(3, 1, "join other server");
    if (!is_starting_server) {
        attr_off(A_BOLD, nullptr);
    }
}

string get_string(const string &label) {
    string value;
    int cursor = 0;

    clear();
    refresh();

    mvaddstr(1, 1, label.c_str());

    int currentLength;

    while (true) {
        mvaddstr(3, 1, value.c_str());
        mvchgat(3, 1 + cursor, 1, A_STANDOUT, 0, nullptr);

        refresh();

        int ch = getch();

        mvchgat(3, 1 + cursor, 1, A_NORMAL, 0, nullptr);
        mvaddstr(3, 1, string(value.size(), ' ').c_str());

        switch (ch) {
        case '\n':
        case KEY_ENTER:
            return value;
        case KEY_LEFT:
            cursor--;
            if (cursor < 0) {
                cursor = 0;
            }
            break;
        case KEY_RIGHT:
            cursor++;
            if (cursor > value.size()) {
                cursor = value.size();
            }
            break;
        case KEY_BACKSPACE:
            if (cursor == 0) {
                break;
            }
            value.erase(value.begin() + cursor - 1);
            cursor--;
            break;
        default:
            value.insert(value.begin() + cursor, ch);
            cursor++;
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    try {
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

        string name = get_string("Enter your username:");
        clear();

        bool is_starting_server = true;

        render_start_option(is_starting_server);
        refresh();

        while (true) {
            switch (getch()) {
            case KEY_UP:
                is_starting_server = true;
                break;
            case KEY_DOWN:
                is_starting_server = false;
                break;
            case '\n':
                goto next;
            }

            render_start_option(is_starting_server);
            refresh();
        }

    next:
        clear();
        if (is_starting_server) {
            channel<Message> sender;
            channel<Message> receiver;
            Player me(name, sender, receiver);

            channel<Player> player_ch = create_server();
            vector<Player> players = {me};
            timeout(50);

            attr_on(A_BOLD, nullptr);
            mvaddstr(1, 1, "Players: ");
            attr_off(A_BOLD, nullptr);
            refresh();

            int current_y = 3;
            mvaddstr(current_y++, 5, me.name.c_str());

            while (getch() != '\n') {
                optional<Player> player = player_ch.recv_with_timeout(50);
                if (player.has_value()) {
                    for (auto existing_player = players.begin();
                         existing_player < players.end(); existing_player++) {
                        if (existing_player->name == player.value().name) {
                            throw "Two players cannot have the same name";
                        }
                    }
                    mvaddstr(current_y++, 5, player.value().name.c_str());
                    refresh();
                    players.push_back(player.value());
                }
            }

            thread([=]() {
                Eights eights(players);
                eights.play_game();
            }).detach();

            client client(name, receiver, sender, false);
            client.play();
        } else {
            string ip = get_string("Enter server ip:");
            pair<channel<Message>, channel<Message>> chs =
                create_channels(tcp_stream(ip, PORT));
            client client(name, chs.first, chs.second, true);
            client.play();
        }

        endwin();
    } catch (const char *error) {
        endwin();
        cerr << error << endl;
    }
}
