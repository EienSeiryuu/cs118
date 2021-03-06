#define _GNU_SOURCE
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
#include <string.h>

#define BUFFERSIZE 8193

static char* skeleton = "HTTP/1.0 %d %s\r\ncontent-type: %s\r\n\r\n%s";    

void respond(int code,char* phrase,char* contype,char* body,int sock)
{
	char* msg;
	asprintf(&msg, skeleton,code,phrase,contype,body);
	write(sock,msg,strlen(msg));
	free(msg);
}

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
	
   char method[BUFFERSIZE];
   char url[BUFFERSIZE];
   char version[BUFFERSIZE];
   
   sscanf (buffer, "%s %s %s\r\n", method, url, version);
   //printf("%s\n",url);
   char* tmp;
   if(strcmp(version,"HTTP/1.0") && strcmp(version,"HTTP/1.1"))
   {
    asprintf(&tmp,"<html><body><h1>%s</h1></body></html>","505: HTTP Version not supported");
    respond(505,"HTTP Version not supported","text/html",tmp,sock);
	free(tmp);
   }
   else if(strcmp(method,"GET"))
   {
    asprintf(&tmp,"<html><body><h1>%s</h1></body></html>","501: Not Implemented");
	respond(501,"Not Implemented","text/html",tmp,sock);
	free(tmp);
   }
   else
   {
    char *contents;
	long input_file_size;
	FILE *input_file = fopen((url+1), "rb");
	if (input_file==NULL)
	{
		asprintf(&tmp,"<html><body><h1>%s</h1></body></html>","404: Not Found");
		respond(404,"Not Found","text/html",tmp,sock);
		free(tmp);
		return;
	}
	fseek(input_file, 0, SEEK_END);
	input_file_size = ftell(input_file);
	fseek(input_file, 0, SEEK_SET);
	contents = malloc(input_file_size+1);
	fread(contents, input_file_size,1, input_file);
	fclose(input_file);
	
	char* ext = strrchr (url, '.');
	char* content_type;
    if (ext==NULL)
	{
		content_type = "text/html";
		respond(200,"OK",content_type,contents,sock);
	}
    else
    {
		if (!strcasecmp (ext, ".jpg") || !strcasecmp (ext, ".jpeg"))
		{
			content_type = "image/jpeg";
			respond(200,"OK",content_type,"",sock);
			write(sock,contents,input_file_size);

		}
		else if (!strcasecmp (ext, ".gif"))
		{
			content_type = "image/gif";
			respond(200,"OK",content_type,"",sock);
			write(sock,contents,input_file_size);

		}
		else if (!strcasecmp (ext, ".html"))
		{
			content_type = "text/html";
			respond(200,"OK",content_type,contents,sock);

		}
		else
		{
		content_type = "text/html";
			respond(200,"OK",content_type,contents,sock);

		}
	}
//	printf("%ld\n",input_file_size);
	
		
	free(contents);
   }
   
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