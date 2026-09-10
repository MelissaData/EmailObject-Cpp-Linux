#include<iostream>
#include <string>

#include "mdEmail.h"
#include "DataContainer.h"

using namespace std;

#ifndef EMAILOBJECT_H
#define EMAILOBJECT_H

/**
 * Wrapper that owns a single Melissa Email Object instance and encapsulates the two
 * things every Melissa object needs: one-time setup (license + data files) and the
 * per-record processing sequence. Reuse one instance across many emails; do NOT
 * re-initialize per email.
 */
class EmailObject
{
public:
	// Path to the Email Object data files.
	string dataFilePath;

	// The underlying Melissa Email Object instance.
	mdEmail* mdEmailObj = new mdEmail;

	/**
	 * Performs the mandatory one-time setup, in this required order:
	 *   1. SetLicenseString     - authorize the object.
	 *   2. SetPathToEmailFiles  - tell it where the data files live.
	 *   3. InitializeDataFiles  - load the data into memory.
	 *
	 * @param license  The Melissa license string used to authorize the object.
	 * @param dataPath Path to the folder containing the Email Object data files.
	 */
	EmailObject(string license, string dataPath);

	/**
	 * Runs the full Email Object processing sequence for one email and captures its
	 * result codes. This is the canonical per-record call pattern to copy into your
	 * own application:
	 *   configure lookup options -> VerifyEmail -> GetResults
	 *
	 * @param data The record to process. Its Email is read as input, and ResultCodes is
	 *             populated with this run's result codes.
	 */
	void ExecuteObjectAndResultCodes(DataContainer& data);
};

#endif
