#ifndef ROUTING_H
#define ROUTING_H
#include <vector>
#include <iostream>
using namespace std;


class routing_entry
{
public:
	routing_entry(char d, int c, int o, int de)
		:destinantion(d), cost(c), out_port(o), dest_port(de)
	{}
	char destinantion;
	int cost;
		int out_port;
	int dest_port;
};
class routing_table
{
public:
	std::vector<routing_entry> table_entries;
	void insert(routing_entry x)
	{
		table_entries.push_back(x);
	}

	vector<routing_entry>::iterator get(char x)
	{
		vector<routing_entry>::iterator it;
		for (it = table_entries.begin(); it < table_entries.end(); it++)
		{
			if (it->destinantion == x)
				return it;
		}
		return it;
	}
	void print()
	{
		vector<routing_entry>::iterator it;
		cout << "destinantion,cost,outgoing_port,destinantion_port\n";
		for (it = table_entries.begin(); it < table_entries.end(); it++)
		{
			cout << it->destinantion << "," << it->cost << "," << it->out_port << "," << it->dest_port << "\n";
		}
	}
};


#endif
