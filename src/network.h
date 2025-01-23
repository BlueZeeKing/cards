#include "Player.h"
#include "channel.h"
#include "socket.h"
#include <memory>

#define PORT "8080"

channel<Player> create_server();

std::pair<channel<Message>, channel<Message>>
create_channels(tcp_stream stream);
