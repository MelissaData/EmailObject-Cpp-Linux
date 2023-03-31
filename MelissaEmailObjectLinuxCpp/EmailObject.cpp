#include "EmailObject.h"

EmailObject::EmailObject(string license, string dataPath)
{
	// Set license string and set path to datafiles (.dat, etc)
	mdEmailObj->SetLicenseString(license.c_str());
	dataFilePath = dataPath;

	// If you see a different date than expected, check your license string and either download the new data files or use the Melissa Updater program to update your data files.  
	mdEmailObj->SetPathToEmailFiles(dataFilePath.c_str());
	mdEmail::ProgramStatus pStatus = mdEmailObj->InitializeDataFiles();

	if (pStatus != mdEmail::ProgramStatus::ErrorNone)
	{
		cout << "Failed to Initialize Object." << endl;
		cout << pStatus << endl;
		return;
	}

	cout << "                    DataBase Date: " + string(mdEmailObj->GetDatabaseDate()) << endl;
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
	// These are the configuarble pieces of the Email Object. We are setting what kind of information we want to be looked up
	mdEmailObj->SetCacheUse(1);
	mdEmailObj->SetCorrectSyntax(true);
	mdEmailObj->SetDatabaseLookup(true);
	mdEmailObj->SetFuzzyLookup(true);
	mdEmailObj->SetMXLookup(true);
	mdEmailObj->SetStandardizeCasing(true);
	mdEmailObj->SetWSLookup(false);

	mdEmailObj->VerifyEmail(data.Email);
	data.ResultCodes = mdEmailObj->GetResults();

	// ResultsCodes explain any issues Email Object has with the object.
	// List of result codes for Email Object
	// https://wiki.melissadata.com/?title=Result_Code_Details#Email_Object
}
