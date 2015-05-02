#include <string>
#include <windows.h>

using namespace std;

template<typename T> 
void ReturnError(int retcode, T err, char *func, bool bHex = false);

#include "FileIO.h"

void WriteOutputToFile(const string &strOutput, bool bHTML);

#include "CryptAES.h"

string ReadAndDecryptEJS(string strFile, int iKeyID, bool bNoIV);

#include "JTypes.h"

vector<JHost> ParseEJS(const string &strData);
string BuildHTMLExport(vector<JHost> vHosts);