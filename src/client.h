#include "CardCollection.h"
#include "channel.h"
#include "messages.h"
#include <optional>

struct client {
    CardCollection hand;
    std::vector<int> hand_size;
    std::vector<std::string> player_names;
    int current_player;
    int cursor_idx;
    int my_idx;
    std::string name;
    std::optional<Card> discard;
    channel<Message> sender;
    channel<Message> receiver;

    client(std::string name, channel<Message> sender, channel<Message> receiver,
           bool send_join);

    void play();
    void play_turn();
    void draw();
    void render();
    void render_cursor(int);
};
