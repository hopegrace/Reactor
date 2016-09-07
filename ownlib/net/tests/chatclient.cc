#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <libgen.h>  // basename
#include <unistd.h>
#include <fcntl.h>
#include <ownlib/base/DateTime.h>
#include <ownlib/net/InetAddress.h>
#include <ownlib/net/Socket.h>
#include <ownlib/net/Select.h>

using namespace sduzh::base;
using namespace sduzh::net;

int main(int argc, char *argv[])
{
	if (argc != 3) {
		printf("Usage: %s host, port\n", basename(argv[0]));
		return 1;
	}
	
	uint16_t port = static_cast<uint16_t>(atoi(argv[2]));

	Socket conn;
	int ret = conn.connect(InetAddress(argv[1], port));
	if (ret < 0) {
		perror("connect");
		return 1;
	}

	Select poller;
	poller.add_readfd(STDIN_FILENO);
	poller.add_readfd(conn.fd());

	for (;;) {
		std::vector<SelectEvent> events;
		int nevent = poller.select(&events);
		if (nevent <= 0) 
			break;

		char buffer[1024];
		SelectEvent event = events[0];
		if (event.fd == STDIN_FILENO) {
			ssize_t nread = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
			if (nread <= 0) 
				break;
			buffer[nread] = '\0';
			size_t nblank = strspn(buffer, " \t\n");
			conn.sendall(buffer + nblank, nread - nblank);
		} else {
			ssize_t nread = conn.recv(buffer, sizeof buffer);
			if (nread <= 0) 
				break;
			write(STDOUT_FILENO, buffer, nread);
		}
	}

	conn.close();

	return 0;
}