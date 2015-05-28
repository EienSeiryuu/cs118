#include "server.h"
#include "to_string_patched.h"

#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;
server::server(const string addr, int port): server_port(port), server_addr(addr)
{   
	string string_port = to_string_patched(port);
    struct addrinfo addri;
    addri.ai_family = AF_UNSPEC;
    addri.ai_socktype = SOCK_DGRAM;
    addri.ai_protocol = IPPROTO_UDP;
	
    getaddrinfo(addr.c_str(), string_port.c_str(), &addri, &server_addrinfo);
	
    if(server_addrinfo == NULL)
    {
		cerr<<"Failed to create server addrinfo for "<< addr << ":" << port<<"\n";
		cerr<<"Quit...\n";
		exit(1);
	}
	
    server_socket = socket(server_addrinfo->ai_family, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP);
    if(server_socket == -1)
    {
        freeaddrinfo(server_addrinfo);
		cerr<<"Failed to create server socket for "<< addr << ":" << port <<"\n";
		cerr<<"Quit...\n";
		exit(1);
	}
    if(bind(server_socket, server_addrinfo->ai_addr, server_addrinfo->ai_addrlen) != 0)
    {
        freeaddrinfo(server_addrinfo);
        close(server_socket);
		cerr<<"Failed to bind server socket at "<< addr << ":" << port <<"\n";;
		exit(1);
    }
}
server::~server()
{
    freeaddrinfo(server_addrinfo);
    close(server_socket);
}
int server::receive(char *msg)
{
	int x  = recv(server_socket, msg, MAX_MSG_SIZE, MSG_WAITALL);
	msg[x]='\0';
	return x;
}
