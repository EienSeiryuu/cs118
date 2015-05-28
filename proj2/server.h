#ifndef SERVER_H
#define SERVER_H

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>

#define MAX_MSG_SIZE 4096

class server
{
public:
	server(const std::string addr, int port);
	~server();

	int server_socket;
	int server_port;
	std::string server_addr;
	struct addrinfo* server_addrinfo;

	int receive(char *msg);

};

 
#endif
