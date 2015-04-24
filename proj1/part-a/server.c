#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

#include <unistd.h>

#define BUFFERSIZE 8193
void error(char *msg)
{
    perror(msg);
    exit(1);
}
void process_client (int sock)
{
   int n;
   char buffer[BUFFERSIZE];
      
   bzero(buffer,BUFFERSIZE);
   n = read(sock,buffer,BUFFERSIZE-1);
   if (n < 0) error("ERROR reading from socket");
   printf("%s\n",buffer);
}

int main(int argc, char *argv[])
{
	int port_num,pid,clilen;
	int server_socket,new_socket;
	struct sockaddr_in serv_addr, cli_addr;
    if (argc < 2) 
	{
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
    }
	//prepare socket and port number
	port_num = atoi(argv[1]);
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
	{
         fprintf(stderr,"ERROR, cannot create socket\n");
         exit(1);
    }
	//init sock addr struct
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port_num);

	//bind
	if ( bind(server_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 ) 
	{
		fprintf(stderr, "ERROR, binding failed\n");
		exit(1);
    }
	//listen max 127 conn @bound server socket
	listen(server_socket,127);
	clilen = sizeof(cli_addr);

	while(1)
	{
		new_socket = accept(server_socket, (struct sockaddr*) &cli_addr, (unsigned int *) &clilen); 
		if (new_socket < 0)
		{
			error("ERROR on accept");
		}
		// fork for each conn
		pid = fork();
		if (pid < 0)
		{
			error("ERROR on fork");
		}
		else if (pid == 0) 
		{
			close(server_socket);
			process_client(new_socket);
			if (close(new_socket) < 0)
				error("ERROR closing connection");
			exit(0);
		}
		else 
		{
			if (close(new_socket) < 0)
				error("ERROR closing connection (new_socket)");
			waitpid(-1, NULL, WNOHANG);
		}
	}
	return 0;
}