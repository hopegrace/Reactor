#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <libgen.h> // basename
#include <ownlib/net/InetAddress.h>
#include <ownlib/net/Socket.h>
#include <ownlib/net/Select.h>

using namespace std;
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
	// server.set_reuse_port(true);
	server.bind(InetAddress("0.0.0.0", static_cast<uint16_t>(port)));
	server.listen(5);
	
	selector.add_readfd(server.fd());

	for (;;) {
		std::vector<SelectEvent> events;
		int nevents = selector.select(&events);

		if (nevents < 0) {
			perror("select");
			break;
		} 

		assert(nevents == static_cast<int>(events.size()));
		for (int i = 0; i < nevents; i++) {
			SelectEvent event = events[i];
			int fd = event.fd;
			assert(event.events & EVENT_READABLE);
			if (fd == server.fd()) {
				InetAddress client_addr;
				int client_fd  = server.accept(&client_addr);
				assert(client_fd >= 0);
				printf("%s:%d connected\n", client_addr.host().c_str(), client_addr.port());

				assert(clients.find(client_fd) == clients.end());
				clients[client_fd] = new Socket(client_fd);
				clients[client_fd]->set_blocking(false);

				selector.add_readfd(client_fd);
			} else {
				assert(clients.find(fd) != clients.end());
				Socket *client = clients[fd];
				assert(fd == client->fd());
				ssize_t nread = client->recv(buffer, sizeof buffer);
				if (nread <= 0) {
					InetAddress peer = client->getpeername();
					printf("%s:%d closed\n", peer.host().c_str(), peer.port()); 

					delete clients[fd];
					clients.erase(fd);
					selector.remove_readfd(fd);
				} else {
					for (auto it = clients.begin(); it != clients.end(); ++it) {
						it->second->sendall(buffer, nread);
					}
				}
			}
		}
	}

	return 0;
}
