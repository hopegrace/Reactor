#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <libgen.h> // basename
#include <ownlib/base/DateTime.h>
#include <ownlib/net/InetAddress.h>
#include <ownlib/net/Socket.h>
#include <ownlib/net/Select.h>

using namespace std;
using namespace sduzh::base;
using namespace sduzh::net;

map<int, Socket*> clients;
char buffer[1024];

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Usage: %s listen port\n", basename(argv[0]));
		return 1;
	}

	Select selector;

	int port = atoi(argv[1]);
	Socket server;
	server.set_blocking(false);
	server.set_reuse_addr(true);
	server.bind(InetAddress("0.0.0.0", static_cast<uint16_t>(port)));
	server.listen(5);
	
	selector.add_readfd(server.fd());

	for (;;) {
		std::vector<SelectEvent> events;
		int nevents = selector.select(&events);

		if (nevents < 0) {
			perror("select");
			continue;
		} 

		assert(nevents == static_cast<int>(events.size()));
		for (int i = 0; i < nevents; i++) {
			SelectEvent event = events[i];
			int fd = event.fd;
			assert(event.events & EVENT_READABLE);
			if (fd == server.fd()) {
				InetAddress client_addr;
				int client_fd  = server.accept(&client_addr);
				DateTime now = DateTime::current();
				printf("[%s]%s:%d connected\n", now.to_string().c_str(), client_addr.host().c_str(), client_addr.port());

				assert(clients.find(client_fd) == clients.end());
				clients[client_fd] = new Socket(client_fd);
				clients[client_fd]->set_blocking(false);
				clients[client_fd]->sendall("Welcome!\n");

				selector.add_readfd(client_fd);
			} else {
				assert(clients.find(fd) != clients.end());
				Socket *client = clients[fd];
				DateTime now = DateTime::current();
				InetAddress peer = client->getpeername();
				int head = snprintf(buffer, sizeof buffer, "[%s]%s:%d:\n", now.to_string().c_str(),
								peer.host().c_str(), peer.port());
				ssize_t nread = client->recv(buffer + head, sizeof(buffer) - head);
				if (nread <= 0) {
					printf("[%s]%s:%d disconnected\n", now.to_string().c_str(), peer.host().c_str(), peer.port());
					delete clients[fd];
					clients.erase(fd);
					selector.remove_readfd(fd);
				} else {
					for (auto it = clients.begin(); it != clients.end(); ++it) {
						if (it->second == client)
							continue;
						it->second->sendall(buffer, head + nread);
					}
				}
			}
		}
	}

	return 0;
}
