#include<iostream>

using namespace std;

#ifndef DATACONTAINER_H
#define DATACONTAINER_H

class DataContainer
{
public:
	char RecID[50] = "";
	char Email[50] = "";
	string ResultCodes;

	DataContainer() {};
};

#endif