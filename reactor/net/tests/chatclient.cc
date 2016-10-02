#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <libgen.h>  // basename
#include <unistd.h>
#include <fcntl.h>
#include <reactor/base/Timestamp.h>
#include <reactor/base/SimpleLogger.h>
#include <reactor/net/InetAddress.h>
#include <reactor/net/SelectPoller.h>
#include <reactor/net/TcpSocket.h>

using namespace sduzh::base;
using namespace sduzh::net;

int main(int argc, char *argv[])
{
	if (argc != 3) {
		printf("Usage: %s host, port\n", basename(argv[0]));
		return 1;
	}
	
	uint16_t port = static_cast<uint16_t>(atoi(argv[2]));

	TcpSocket conn;
	int ret = conn.connect(InetAddress(argv[1], port));
	if (ret < 0) {
		perror("connect");
		return 1;
	}

	SelectPoller poller;
	poller.update_event(STDIN_FILENO, EVENT_READ);
	poller.update_event(conn.fd(), EVENT_READ);

	for (;;) {
		std::vector<PollEvent> events;
		int nevent = poller.poll(&events);
		if (nevent <= 0) 
			break;

		char buffer[1024];
		PollEvent event = events[0];
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
			ssize_t nw = write(STDOUT_FILENO, buffer, nread);
			static_cast<void>(nw);
		}
	}

	conn.close();

	return 0;
}
