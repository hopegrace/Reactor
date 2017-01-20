#ifndef REACTOR_REDIS_SERVER_H
#define REACTOR_REDIS_SERVER_H

#include "reactor/net/TcpServer.h"

namespace reactor {

class RedisServer {
public:
	RedisServer(EventLoop *loop, InetAddress addr):
		server_(loop, addr)
	{
	}

	void start() 
	{ server_.start(); }

private:
	net::TcpServer server_;	
};

}

#endif
