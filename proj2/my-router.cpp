#include "routing.h"
#include <fstream>
#include <string>
#include <vector>
#include <string>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unordered_map>

using namespace std;

const string linkFile = "link-init.config";
const string portFile = "router-port.config";

void complain()
{
	cerr << "Invalid arg\n";
	cerr << "Quit...\n";
	exit(1);
}
void complain(string complaint)
{
	cerr << complaint;
	cerr << "Quit...\n";
	exit(1);
}
vector<string> split(string input, string delimiter)
{
	vector<string> output;
	char *pch;
	char *str = strdup(input.c_str());
	pch = strtok(str, delimiter.c_str());
	while (pch != NULL)
	{
		output.push_back(pch);

		pch = strtok(NULL, delimiter.c_str());
	}

	free(str);

	return output;
}

unordered_map<char, int>	ports;
routing_table* setup(char routerName)
{
	routing_table* ret = new routing_table();

	ifstream infile;
	infile.open(portFile);
	if (infile.fail())complain("cannot open " + portFile);
	string line;
	while (!infile.eof())
	{
		getline(infile, line);
		if (!line.empty())
		{
			int p = line.find("=", 0);
			if (p<0)complain("malformed " + portFile);
			ports.insert(pair<char, int>(line[0], atoi(line.substr(p + 1).c_str())));
		}
	}
	infile.close();

	infile.open(linkFile);
	if (infile.fail())complain("cannot open " + linkFile);
	while (!infile.eof())
	{
		getline(infile, line);
		if (!line.empty() && tolower(line[0]) == tolower(routerName))
		{
			vector<string> output = split(line, ",");
			int o, d;
			unordered_map<char, int>::iterator i;
			for (i = ports.begin(); i != ports.end(); i++)
			{
				if (tolower(i->first) == tolower(routerName))
				{
					o = i->second;
					break;
				}
			}
			if (i == ports.end())complain("bad configuration files");
			for (i = ports.begin(); i != ports.end(); i++)
			{
				if (tolower(i->first) == tolower(output[1][0]))
				{
					d = i->second;
					break;
				}
			}
			if (i == ports.end())complain("bad configuration files");

			ret->insert(routing_entry(output[1][0], atoi(output[2].c_str()), o, d));
		}
	}
	infile.close();
	return ret;
}
int main(int argc, const char* argv[])
{
	if (argc < 2)complain();
	routing_table* x = setup(argv[1][0]);
	x->print();
}