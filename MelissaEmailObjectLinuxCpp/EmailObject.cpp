#include "EmailObject.h"

EmailObject::EmailObject(string license, string dataPath)
{
	// Set license string and set path to data files
	mdEmailObj->SetLicenseString(license.c_str());
	dataFilePath = dataPath;

	// Load the data files. The returned ProgramStatus reports whether initialization succeeded.
	// If you see a different date than expected, check your license string and either download the new data files
	// or use the Melissa Updater program to update your data files.
	mdEmailObj->SetPathToEmailFiles(dataFilePath.c_str());
	mdEmail::ProgramStatus pStatus = mdEmailObj->InitializeDataFiles();

	// If an issue occurred, please investigate the common causes.
	// Common causes: an invalid/expired license, or missing/wrong-path data files.
	if (pStatus != mdEmail::ProgramStatus::ErrorNone)
	{
		cout << "Failed to Initialize Object." << endl;
		cout << pStatus << endl;
		return;
	}

	// Diagnostic information, handy for confirming the object loaded the data you expect:

	// Build date of the data files
	cout << "                    DataBase Date: " + string(mdEmailObj->GetDatabaseDate()) << endl;

	// When the license stops working
	cout << "                  Expiration Date: " + string(mdEmailObj->GetLicenseStringExpirationDate()) << endl;

	/**
	 * This number should match with the file properties of the Melissa Object binary file.
	 * If TEST appears with the build number, there may be a license key issue.
	 */
	cout << "                   Object Version: " + string(mdEmailObj->GetBuildNumber()) << endl;
}

// This will call the lookup function to process the input email as well as generate the result codes
void EmailObject::ExecuteObjectAndResultCodes(DataContainer& data)
{
	// These are the configurable pieces of the Email Object - they control which checks
	// VerifyEmail performs (syntax correction, database & MX lookups, fuzzy matching, ...).
	mdEmailObj->SetCacheUse(1);
	mdEmailObj->SetCorrectSyntax(true);
	mdEmailObj->SetDatabaseLookup(true);
	mdEmailObj->SetFuzzyLookup(true);
	mdEmailObj->SetMXLookup(true);
	mdEmailObj->SetStandardizeCasing(true);
	mdEmailObj->SetWSLookup(false);

	// Validate and correct the email per the options above
	mdEmailObj->VerifyEmail(data.Email);

	// Collect the result codes for this run
	// ResultsCodes explain any issues Email Object has with the object.
	// List of result codes for Email Object
	// https://docs.melissa.com/on-premise-api/email-object/result-codes.html
	data.ResultCodes = mdEmailObj->GetResults();
}
