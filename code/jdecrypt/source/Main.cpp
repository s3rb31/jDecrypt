#include "JDecrypt.h"

void PrintKeys(); 
void PrintUsage(const char *pFileName);

int main(int argc, char **argv)
{
	if (argc == 1)
	{
		PrintUsage(argv[0]);
		return 1;
	}

	int iKeyID = 1;
	bool bNoIV = false;
	bool bHTML = false;	
	string strFile, strArg;

	for (int i = 1; i < argc; i++)
	{
		strArg = argv[i];

		if (strArg.at(0) == '-')
		{
			if (i == 1 && strArg == "-showkeys")
			{
				PrintKeys();
				return 1;
			}

			if (strArg == "-html")
			{
				bHTML = true;
				continue;
			}

			if (strArg == "-keyid")
			{
				i += 1;
				iKeyID = atoi(argv[i]);
				continue;
			}

			if (strArg == "-noiv")
			{
				bNoIV = true;
				continue;
			}
			
			continue;
		}		

		strFile = strArg;
		break;
	}

	if (iKeyID < 1 || iKeyID > 7)
	{
		cout << "Try one of those:" << endl;
		PrintKeys();
		return 1;
	}

	string strDecrypted = ReadAndDecryptEJS(strFile, iKeyID, bNoIV);
	
	if (bHTML)
	{
		vector<JHost> vHosts = ParseEJS(strDecrypted.c_str(), strDecrypted.size());
		strDecrypted = BuildHTMLExport(vHosts);
	}

	WriteOutputToFile(strDecrypted, bHTML);
	
	return 0;
}

void PrintUsage(const char *pFileName)
{
	cout
		<< "Coded bY S3RB31" << endl << endl
		<< "Usage: \t\t" << pFileName << ".exe [ARGS] [FILE]" << endl << endl
		<< "-html \t\t- Export to HTML (only AccountSettings.accounts.ejs)" << endl
		<< "-keyid [KeyID]\t- Set AES Key (1-7, 1 = default)" << endl
		<< "-noiv \t\t- Use zero as IV" << endl
		<< "-showkeys \t- Show implemented AES keys" << endl;
}

void PrintKeys()
{
	cout
		<< "Keys:" << endl
		<< "1: { 1, 6,  4, 5, 2,  7, 4, 3, 12, 61,  14, 75, -2, -7, -44,  33 } - AccountSettings.java" << endl
		<< "2: { 2, 4,  4, 5, 2,  7, 4, 3, 12, 61,  14, 75, -2, -7, -44,  33 } - AuthenticationControllerSettings.java" << endl
		<< "3: { 1, 3, 17, 1, 1, 84, 1, 1,  1,  2, -63,  1, 17,  1,  34, -12 } - ProxySelectorImpl.java" << endl
		<< "4: { 1, 3, 17, 1, 1, 84, 1, 1,  1,  1,  18,  1,  1,  1,  34,   1 } - CrawlerPluginController.java" << endl
		<< "5: { 1, 2, 17, 1, 1, 84, 1, 1,  1,  1,  18,  1,  1,  1,  34,   1 } - JSonStorage.java, SubConfiguration.java" << endl
		<< "6: { 0, 2, 17, 1, 1, 84, 2, 1,  1,  1,  18,  1,  1,  1,  18,   1 } - ExtFileChooseIdConfig.java" << endl
		<< "7: { 0, 1,  2, 3, 4,  5, 6, 7,  8,  9,  10, 11, 12, 13,  14,  15 } - DownloadLinkStorable.java" << endl;
}

