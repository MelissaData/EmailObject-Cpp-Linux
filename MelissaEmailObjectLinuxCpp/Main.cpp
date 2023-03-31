#include <iostream>
#include <cstring>
#include <cstdlib>
#include <list>

#include "mdEmail.h"
#include "EmailObject.h"
#include "DataContainer.h"

using namespace std;

// function declarations
void ParseArguments(string& license, string& testEmail, string& dataPath, int argc, char** argv);
void RunAsConsole(string license, string testEmail, string dataPath);
list<string> SplitResultCodes(string s, string delimiter);

int main(int argc, char* argv[])
{
	// Variables
	string license = "";
	string testEmail = "";
	string dataPath = "";

	ParseArguments(license, testEmail, dataPath, argc, argv);
	RunAsConsole(license, testEmail, dataPath);

	return 0;
}

void ParseArguments(string& license, string& testEmail, string& dataPath, int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (string(argv[i]) == "--license" || string(argv[i]) == "-l")
		{
			if (argv[i + 1] != NULL)
			{
				license = argv[i + 1];
			}
		}
		if (string(argv[i]) == "--email" || string(argv[i]) == "-e")
		{
			if (argv[i + 1] != NULL)
			{
				testEmail = argv[i + 1];
			}
		}
		if (string(argv[i]) == "--dataPath" || string(argv[i]) == "-d")
		{
			if (argv[i + 1] != NULL)
			{
				dataPath = argv[i + 1];
			}
		}
	}
}

void RunAsConsole(string license, string testEmail, string dataPath)
{
	cout << "\n\n============ WELCOME TO MELISSA EMAIL OBJECT LINUX C++ =============\n" << endl;
	
	EmailObject* emailObject = new EmailObject(license, dataPath);

	bool shouldContinueRunning = true;

	while (shouldContinueRunning)
	{
		DataContainer dataContainer = DataContainer();

		if (testEmail.empty())
		{
			cout << "\nFill in each value to see the Email Object results" << endl;
			cout << "Email: ";

			string input;
			getline(cin, input);

			strcpy(dataContainer.Email, input.c_str());
		}
		else
		{
			strcpy(dataContainer.Email, testEmail.c_str());
		}

		// Print user input
		cout << "\n============================== INPUTS ==============================\n" << endl;
		cout << "\t                    Email: " + string(dataContainer.Email)              << endl;

		// Execute Email Object
		emailObject->ExecuteObjectAndResultCodes(dataContainer);

		// Print output
		cout << "\n============================== OUTPUT ==============================\n"                          << endl;
		cout << "\n\tEmail Object Information:"                                                                     << endl;
		cout << "\t                    Email: " + string(dataContainer.Email)                                       << endl;
		cout << "\t              MailBoxName: " + string(emailObject->mdEmailObj->GetMailBoxName())                 << endl;
		cout << "\t               DomainName: " + string(emailObject->mdEmailObj->GetDomainName())                  << endl;
		cout << "\t           TopLevelDomain: " + string(emailObject->mdEmailObj->GetTopLevelDomain())              << endl;
		cout << "\tTopLevelDomainDescription: " + string(emailObject->mdEmailObj->GetTopLevelDomainDescription())   << endl;
		cout << "\t             Result Codes: " + dataContainer.ResultCodes                                         << endl;

		list<string> rs = SplitResultCodes(dataContainer.ResultCodes, ",");
		list<string>::iterator it;

		for (it = rs.begin(); it != rs.end(); it++)
		{
			printf("        %s: %s", it->c_str(), emailObject->mdEmailObj->GetResultCodeDescription(it->c_str(), emailObject->mdEmailObj->ResultCodeDescriptionLong));
			cout << endl;
		}

		bool isValid = false;
		if (!testEmail.empty()) 
		{
			isValid = true;
			shouldContinueRunning = false;
		}
		while (!isValid)
		{
			string testAnotherResponse;

			cout << "\nTest another email? (Y/N)" << endl;
			getline(cin, testAnotherResponse);

			if (!testAnotherResponse.empty())
			{
				if (testAnotherResponse == "y" || testAnotherResponse == "Y")
				{
					isValid = true;
				}
				else if (testAnotherResponse == "n" || testAnotherResponse == "N")
				{
					isValid = true;
					shouldContinueRunning = false;
				}
				else
				{
					cout << "Invalid Response, please respond 'Y' or 'N'" << endl;
				}
			}
		}
	}
	cout << "\n=============== THANK YOU FOR USING MELISSA C++ OBJECT =============\n" << endl;
}

/// <summary>
/// This function splits the resultCodes based on a delimiter
/// </summary>
/// <param name="s">the resultCode string</param>
/// <param name="delimiter"the delimiter string></param>
/// <returns></returns>
list<string> SplitResultCodes(string s, string delimiter) {
	list<string> resultCodes;

	size_t pos = 0;
	string token;

	while ((pos = s.find(delimiter)) != string::npos) {
		token = s.substr(0, pos);
		resultCodes.push_back(token);
		s.erase(0, pos + delimiter.length());
	}

	// push back the last resultCode
	resultCodes.push_back(s);

	return resultCodes;
}
