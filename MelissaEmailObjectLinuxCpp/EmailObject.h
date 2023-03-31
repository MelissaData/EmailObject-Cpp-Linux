#include<iostream>
#include <string>

#include "mdEmail.h"
#include "DataContainer.h"

using namespace std;

#ifndef EMAILOBJECT_H
#define EMAILOBJECT_H

class EmailObject
{
public:
	// Path to Email Object data files (.dat, etc)
	string dataFilePath;

	// Create instance of Melissa Email Object
	mdEmail* mdEmailObj = new mdEmail;

	EmailObject(string license, string dataPath);

	// This will call the lookup function to process the input email as well as generate the result codes
	void ExecuteObjectAndResultCodes(DataContainer& data);
};

#endif
