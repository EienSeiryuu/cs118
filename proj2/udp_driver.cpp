#include "client.h"
#include "server.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;
void test_server()
{
	char buffer[MAX_MSG_SIZE];
	server* s = new server("127.0.0.1",7777);
	while(1)
	{
		if(s->receive(buffer)!=0)
		{
		cout<<buffer<<"\n";
		cout<<"***********END OF MSG***********\n";
		}
	}
}
void test_client()
{
client* c = new client("127.0.0.1",7777);
	while(1)
	{
		string msg;
		cout<<"message?\n";
		getline(cin,msg);
		c->send(msg);
	}
}
void complain()
{
	cerr<<"Invalid arg\n";
	cerr<<"Quit...\n";
	exit(1);
}
int main(int argc, const char* argv[])
{
if(argc != 2) complain();

if(strcmp(argv[1], "server") == 0)
	test_server();
else if (strcmp(argv[1], "client") == 0)
	test_client();
else complain();
}
