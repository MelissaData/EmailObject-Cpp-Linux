/**
 * @file Main.cpp
 * @brief Email Object allows your websites and custom applications to update email addresses
 * in your database files while verifying and correcting misspelled domain names.
 *
 * High-level flow of this sample:
 *   1. SETUP     - create an mdEmail instance, hand it the license string and the
 *                  path to the data files, then InitializeDataFiles() (one time).
 *   2. INPUT     - feed an email address in.
 *   3. PROCESS   - configure the lookup options, then VerifyEmail() checks and
 *                  corrects the address.
 *   4. READ      - pull the parsed pieces back out with the Get* getters
 *                  (GetMailBoxName, GetDomainName, GetTopLevelDomain, ...).
 *   5. INTERPRET - GetResults() returns comma-separated result codes describing
 *                  what the object did/found; each code has a human description.
 *
 * The pieces of this sample map onto that flow:
 *   - main / ParseArguments / RunAsConsole : console harness (argument parsing + the interactive loop).
 *   - EmailObject     : thin wrapper around mdEmail that owns setup + the call sequence.
 *   - DataContainer   : plain holder for one record's input and output.
 *
 * Where mdEmail comes from:
 *   There is no generated wrapper source for C++. mdEmail.h and
 *   mdEnums.h declare the API, and libmdEmail.so carries the implementation. The
 *   accompanying MelissaEmailObjectLinuxCpp.sh script downloads all three on every run.
 *
 * Reference:
 *   Quickstart    : https://docs.melissa.com/on-premise-api/email-object/email-object-quickstart.html
 *   Release notes : https://releasenotes.melissa.com/on-premise-api/email-object/
 *   Result codes  : https://docs.melissa.com/on-premise-api/email-object/result-codes.html
 */

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

/**
 * Entry point. Reads the optional command-line arguments, then hands control to
 * RunAsConsole, which performs the actual Email Object setup and processing.
 *
 * @param argc The count of command-line arguments.
 * @param argv The raw command-line arguments.
 */
int main(int argc, char* argv[])
{
	// Populated by ParseArguments below.
	string license = "";
	string testEmail = "";
	string dataPath = "";

	ParseArguments(license, testEmail, dataPath, argc, argv);
	RunAsConsole(license, testEmail, dataPath);

	return 0;
}

/**
 * Reads the supported command-line options into the reference parameters.
 *
 * Recognized flags (each followed by its value, e.g. "--email name@example.com"):
 *   --license / -l   : the Melissa license string
 *   --email / -e     : an email address to test in one-shot mode
 *   --dataPath / -d  : path to the Email Object data files
 *
 * @param license   Receives the Melissa license string.
 * @param testEmail Receives the email address to test in one-shot mode.
 * @param dataPath  Receives the path to the Email Object data files.
 * @param argc      The count of command-line arguments to parse.
 * @param argv      The raw command-line arguments to parse.
 */
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

/**
 * Sets up the Email Object once, then drives the input -> process -> output cycle.
 *
 * In interactive mode (no --email) it loops, asking for a new email each pass until
 * the user answers "N". In one-shot mode (--email supplied) it runs a single pass
 * and exits.
 *
 * @param license   The Melissa license string used to initialize the object.
 * @param testEmail An email address to process in one-shot mode; if empty, the program prompts interactively.
 * @param dataPath  Path to the Email Object data files.
 */
void RunAsConsole(string license, string testEmail, string dataPath)
{
	cout << "\n\n============ WELCOME TO MELISSA EMAIL OBJECT LINUX C++ =============\n" << endl;
	
	// Construct the wrapper. This is where the object is licensed, pointed at the
	// data files, and initialized (see the EmailObject constructor below).
	EmailObject* emailObject = new EmailObject(license, dataPath);

	bool shouldContinueRunning = true;

	while (shouldContinueRunning)
	{
		// Holder for this pass's input and result codes.
		DataContainer dataContainer = DataContainer();

		if (testEmail.empty())
		{
			// Interactive mode: prompt the user for an email address.
			cout << "\nFill in each value to see the Email Object results" << endl;
			cout << "Email: ";

			string input;
			getline(cin, input);

			strcpy(dataContainer.Email, input.c_str());
		}
		else
		{
			// One-shot mode: use the email passed on the command line.
			strcpy(dataContainer.Email, testEmail.c_str());
		}

		// Print user input
		cout << "\n============================== INPUTS ==============================\n" << endl;
		cout << "\t                    Email: " + string(dataContainer.Email)              << endl;

		// Execute Email Object
		// Runs the configure + verify sequence and stores the result codes on dataContainer
		emailObject->ExecuteObjectAndResultCodes(dataContainer);

		// Print output
		// Each Get* getter below returns one component the object produced for the most
		// recently processed email. These read directly from the mdEmail instance, which
		// still holds the results from the Execute call above.
		cout << "\n============================== OUTPUT ==============================\n"                          << endl;
		cout << "\n\tEmail Object Information:"                                                                     << endl;
		cout << "\t                    Email: " + string(dataContainer.Email)                                       << endl;
		cout << "\t              MailBoxName: " + string(emailObject->mdEmailObj->GetMailBoxName())                 << endl;
		cout << "\t               DomainName: " + string(emailObject->mdEmailObj->GetDomainName())                  << endl;
		cout << "\t           TopLevelDomain: " + string(emailObject->mdEmailObj->GetTopLevelDomain())              << endl;
		cout << "\tTopLevelDomainDescription: " + string(emailObject->mdEmailObj->GetTopLevelDomainDescription())   << endl;
		cout << "\t             Result Codes: " + dataContainer.ResultCodes                                         << endl;

		// Result codes come back as a single comma-separated string (e.g. "ES01,ES21").
		// Split it and ask the object for a readable description of each code.
		// ResultCodeDescriptionLong requests the long-form text; a short form is also
		// available via ResultCodeDescriptionShort.
		list<string> rs = SplitResultCodes(dataContainer.ResultCodes, ",");
		list<string>::iterator it;

		for (it = rs.begin(); it != rs.end(); it++)
		{
			printf("        %s: %s", it->c_str(), emailObject->mdEmailObj->GetResultCodeDescription(it->c_str(), emailObject->mdEmailObj->ResultCodeDescriptionLong));
			cout << endl;
		}

		bool isValid = false;

		// In one-shot mode there is nothing more to do after a single pass: mark the
		// input handled and stop the outer loop.
		if (!testEmail.empty()) 
		{
			isValid = true;
			shouldContinueRunning = false;
		}

		// Interactive mode: ask whether to process another email. Keep prompting until
		// we get a valid Y/N. "N" ends the program; "Y" falls through to another pass.
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

/**
 * Splits the comma-separated result-code string into individual codes.
 *
 * @param s         The result-code string (e.g. "ES01,ES21").
 * @param delimiter The delimiter string to split on.
 * @return A list holding each individual result code.
 */
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
