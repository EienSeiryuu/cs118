#ifndef CLIENT_H
#define CLIENT_H

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>

class client
{
public:
	client(const std::string addr, int port);
	~client();

	int client_socket;
	int client_port;
	std::string client_addr;
	struct addrinfo* client_addrinfo;
	
	int send(const std::string msg);

};

#endif
