#include "client.h"
#include "Eights.h"
#include "messages.h"
#include <ncursesw/ncurses.h>
#include <sstream>

using namespace std;

client::client(string name, channel<Message> sender, channel<Message> receiver,
               bool send_join)
    : hand("Hand"), name(name), sender(sender), receiver(receiver) {
    if (send_join) {
        sender.send(Join{.name = name});
    }
}

void client::play() {
    Message msg = Draw{};
    while (true) {
        msg = receiver.recv();

        switch (msg.index()) {
        case 0: {
            StartGame inner = get<StartGame>(msg);
            hand = inner.hand;
            hand_size = vector<int>(inner.player_order.size(), hand.size());
            player_names = inner.player_order;

            for (int i = 0; i < player_names.size(); i++) {
                if (player_names[i] == name) {
                    my_idx = i;
                    break;
                }
            }

            current_player = 0;
            discard = inner.discard;
            clear();
            render();
            refresh();

            if (current_player == my_idx) {
                play_turn();
            }

            break;
        }
        case 1: {
            FinishTurn inner = get<FinishTurn>(msg);
            hand_size[current_player]--;
            discard = inner.new_discard;
            current_player += 1;
            current_player %= player_names.size();
            clear();
            render();
            refresh();

            if (current_player == my_idx) {
                play_turn();
            }
            break;
        }
        case 2:
            hand_size[current_player]++;
            clear();
            render();
            refresh();
            break;
        }
    }
}

void client::render() {
    if (!discard.has_value()) {
        mvaddstr(getmaxy(stdscr) / 2, getmaxx(stdscr) / 2 - 28 / 2,
                 "Waiting for game to start...");
        return;
    }

    color_set(COLOR_CARD_BLACK, nullptr);
    discard.value().display(getmaxy(stdscr) / 2, getmaxx(stdscr) / 2);

    hand.display(getmaxy(stdscr) - 4,
                 getmaxx(stdscr) / 2 - (hand.size() * 5) / 2);

    color_set(COLOR_CARD_BLACK, nullptr);

    int upper_width = -1;
    vector<string> size_msgs;
    for (int i = 0; i < player_names.size(); i++) {
        if (i == my_idx) {
            size_msgs.push_back("");
            continue;
        }
        stringstream stream;
        stream << hand_size[i] << " card";
        if (hand_size[i] > 1) {
            stream << "s";
        }
        string size_msg = stream.str();
        size_msgs.push_back(size_msg);
        if (size_msg.size() > player_names[i].size()) {
            upper_width += size_msg.size();
        } else {
            upper_width += player_names[i].size();
        }
        upper_width += 1;
    }

    int col = getmaxx(stdscr) / 2 - upper_width / 2;
    int width;
    for (int i = 0; i < player_names.size(); i++) {
        int i_offset = (i + my_idx) % player_names.size();
        if (i_offset == my_idx) {
            continue;
        }
        attr_on(A_BOLD, nullptr);
        mvaddstr(1, col, player_names[i_offset].c_str());
        attr_off(A_BOLD, nullptr);
        mvaddstr(2, col, size_msgs[i_offset].c_str());

        if (size_msgs[i_offset].size() > player_names[i_offset].size()) {
            width = size_msgs[i_offset].size();
        } else {
            width = player_names[i_offset].size();
        }

        if (i_offset == current_player) {
            mvaddstr(3, col + width / 2 - 1, "==");
        }

        col += width;
        col += 1;
    }
}

void client::render_cursor(int new_idx) {
    color_set(COLOR_CARD_BLACK, nullptr);
    int start_col = getmaxx(stdscr) / 2 - (hand.size() * 5) / 2;

    if (cursor_idx != -1) {
        mvaddstr(getmaxy(stdscr) - 1, start_col + cursor_idx * 5, "  ");
    }

    cursor_idx = new_idx;

    if (cursor_idx != -1) {
        mvaddstr(getmaxy(stdscr) - 1, start_col + cursor_idx * 5, "==");
    }
}

void client::draw() {
    sender.send(Draw{});
    DrawResult msg = get<DrawResult>(receiver.recv());
    hand.add_card(msg.card);
}

void client::play_turn() {
    clear();
    render();
    cursor_idx = 0;
    render_cursor(0);
    refresh();

    while (true) {
        switch (getch()) {
        case KEY_RIGHT:
            if (cursor_idx < hand.size() - 1) {
                render_cursor(cursor_idx + 1);
            }
            break;
        case KEY_LEFT:
            if (cursor_idx > 0) {
                render_cursor(cursor_idx - 1);
            }
            break;
        case 'd':
            draw();

            clear();
            render();

            render_cursor(cursor_idx);

            break;
        case KEY_UP:
        case ' ':
        case '\n':
            if (card_matches(hand.get_card(cursor_idx), discard.value())) {
                sender.send(Play{
                    .hand_idx = cursor_idx,
                });
                hand.pop_card(cursor_idx);
                clear();
                render();
                refresh();
                return;
            }
            break;
        }
        refresh();
    }
}
