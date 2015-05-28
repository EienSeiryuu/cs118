#include "client.h"
#include "to_string_patched.h"

#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


using namespace std;


client::client(const string addr, int port):client_port(port),client_addr(addr)
{
	string string_port = to_string_patched(port);
    struct addrinfo addri;
    addri.ai_family = AF_UNSPEC;
    addri.ai_socktype = SOCK_DGRAM;
    addri.ai_protocol = IPPROTO_UDP;
	
    getaddrinfo(addr.c_str(), string_port.c_str(), &addri, &client_addrinfo);
	
    if(client_addrinfo == NULL)
    {
		cerr<<"Failed to create socket for "<< addr << ":" << port<<"\n";
		cerr<<"Quit...\n";
		exit(1);
	}
	
    client_socket = socket(client_addrinfo->ai_family, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP);
    if(client_socket == -1)
    {
        freeaddrinfo(client_addrinfo);
		cerr<<"Failed to create socket for "<< addr << ":" << port <<"\n";
		cerr<<"Quit...\n";
		exit(1);
	}
}
client::~client()
{
    freeaddrinfo(client_addrinfo);
    close(client_socket);
}
int client::send(const string msg)
{
    return sendto(client_socket, msg.c_str(), msg.length(), 0, client_addrinfo->ai_addr, client_addrinfo->ai_addrlen);
}
