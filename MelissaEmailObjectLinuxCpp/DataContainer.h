#include<iostream>

using namespace std;

#ifndef DATACONTAINER_H
#define DATACONTAINER_H

/**
 * Data holder for a single record: carries the input email in and the result codes out.
 */
class DataContainer
{
public:
	// Record identifier. Never set or read by this sample.
	char RecID[50] = "";

	// Input: the email address to process.
	char Email[50] = "";

	// Output: comma-separated result codes from GetResults().
	string ResultCodes;

	DataContainer() {};
};

#endif