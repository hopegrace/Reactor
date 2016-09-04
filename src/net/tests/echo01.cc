#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <net/Socket.h>
#include <net/InetAddress.h>

using namespace sduzh::net;

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Usage; %s port", basename(argv[0]));
		return 1;
	}
	int port = atoi(argv[1]);
	Socket server;
	assert(server.fd() >= 0);

	server.bind(InetAddress("0.0.0.0", port));
	server.listen(5);
	InetAddress peer_addr;
	int fd = server.accept(&peer_addr);
	assert(fd > 0);
	printf("%s:%d connected\n", peer_addr.host().c_str(), peer_addr.port());
	Socket client(fd);
	while (1) {
		char buffer[ 1024 ];
		ssize_t nread = client.recv(buffer, sizeof buffer);
		if (nread == 0) {
			printf("connection closed");
			break;
		}
		else if (nread < 0) {
			printf("read error");
			break;
		}
		
		client.sendall(buffer, nread);
	}
	client.close();
	server.close();
	return 0;
}
