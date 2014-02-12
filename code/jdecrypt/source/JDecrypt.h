#include <string>
#include <windows.h>

template<typename T>
void ReturnError(int retcode, T err, char *func);

#include "FileIO.h"
#include "CryptAES.h"

std::basic_string<BYTE> ReadAndDecryptEJS(const char *pFile);

#include "JTypes.h"

void OutputEJSAsHTML(std::vector<JHost> vHosts);
std::vector<JHost> ParseEJS(const BYTE *pData, DWORD dwDataSize);
