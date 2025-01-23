#include "network.h"
#include "Player.h"
#include "channel.h"
#include "messages.h"
#include "socket.h"
#include <thread>
#include <utility>

using namespace std;

pair<channel<Message>, channel<Message>> create_channels(tcp_stream stream) {
    channel<Message> sender;
    channel<Message> receiver;

    std::thread([=]() mutable {
        Message msg = Draw{};
        while (true) {
            try {
                stream >> msg;
            } catch (int err) {
                receiver.send(Error{.num = err});
                break;
            }

            receiver.send(msg);

            if (msg.index() == 6) {
                break;
            }
        }
    }).detach();

    std::thread([=]() mutable {
        Message msg = Draw{};
        while (true) {
            msg = sender.recv();

            try {
                stream << msg;
                stream.sync();
            } catch (int err) {
                receiver.send(Error{.num = err});
                break;
            }

            if (msg.index() == 6) {
                break;
            }
        }
    }).detach();

    return make_pair(sender, receiver);
}

channel<Player> create_server() {
    channel<Player> ch;

    std::thread([=]() mutable {
        tcp_server server("0.0.0.0", PORT);
        while (true) {
            tcp_stream stream = server.accept();
            pair<channel<Message>, channel<Message>> channels =
                create_channels(stream);

            channel<Message> sender = channels.first;
            channel<Message> receiver = channels.second;

            Join msg = get<Join>(receiver.recv());
            Player player(msg.name, sender, receiver);

            ch.send(player);
        }
    }).detach(); // TODO: Graceful shutdown

    return ch;
}
