#include <ctime>
#include <sstream>

#include "JDecrypt.h"
#include "HeaderImage.h"

// org.jdownloader.settings.AccountSettings
BYTE pAESKey[16] = { 1, 6, 4, 5, 2, 7, 4, 3, 12, 61, 14, 75, -2, -7, -44, 33 };	

template<typename T>
void ReturnError(int retcode, T err, char *func)
{
	std::cout << "Error " << err << " from \"" << func << "\"" << std::endl;
	exit(retcode);
}

std::basic_string<BYTE> ReadAndDecryptEJS(const char *pFile)
{
	/* Read data */

	FileIO input(pFile, GENERIC_READ, OPEN_EXISTING);

	if (!input.open())
		ReturnError(2, input.getError(), "input.open()");

	DWORD dwSize = input.getSize(), dwPos = 0;

	if (dwSize == INVALID_FILE_SIZE)
		ReturnError(3, input.getError(), "input.getSize()");

	BYTE *pCryptedData = new BYTE[dwSize]();

	if (!input.read(pCryptedData, dwSize, dwPos))
	{
		delete[] pCryptedData;
		ReturnError(4, input.getError(), "input.read()");
	}

	/* Decrypt data */

	int dwErr = 0;
	CryptAES AES(pAESKey);

	if ((dwErr = AES.Decrypt(pCryptedData, &dwSize, true)) != 0)
	{
		delete[] pCryptedData;
		ReturnError(5, dwErr, "CryptAES::Decrypt()");
	}

	std::basic_string<BYTE> strRet(pCryptedData);

	delete[] pCryptedData;
	return strRet;
}

std::vector<JHost> ParseEJS(const BYTE *pData, DWORD dwDataSize)
{
	std::string strErr;
	picojson::value rootValue;

	picojson::parse(rootValue, pData, pData + dwDataSize, &strErr);

	if (!strErr.empty())
		ReturnError(6, strErr.c_str(), "picojson::parse()");

	if (!rootValue.is<picojson::object>())
		ReturnError(7, "\"JSON not an object!\"", "rootValue.is<picojson::object>()");

	std::vector<JHost> vHosts;
	const picojson::object &rootObject = rootValue.get<picojson::object>();

	for (picojson::value::object::const_iterator rootIter = rootObject.begin(); rootIter != rootObject.end(); ++rootIter)
	{
		JHost TempHost(rootIter->first.c_str());

		const picojson::array &hostArray = rootIter->second.get<picojson::array>();

		for (picojson::value::array::const_iterator accountIter = hostArray.begin(); accountIter != hostArray.end(); ++accountIter)
		{
			JAccount TempAccount(accountIter->get<picojson::object>());

			if (TempAccount.isUsable()) TempHost.pushAccount(TempAccount);
		}

		if (TempHost.getSize() != "0") vHosts.push_back(TempHost);
	}

	return vHosts;
}

void OutputEJSAsHTML(std::vector<JHost> vHosts)
{
	/* Build HTML */

	std::string strFileName, strOutput =
		"<html>\r\n<head>\r\n<meta http-equiv=\"content-type\" content=\"text/html; charset=ISO-8859-1\">\r\n"
		"<title>jDecrytp - {ENTRIES} Host Entry/ies</title>\r\n<style type=\"text/css\">\r\n"
		"#logo { margin:5% auto; background: url(data:image/png;base64,{B64IMG}) no-repeat; background-size:944px 90px; width:944px; height:90px; }"
		".datagrid { width:85%; margin-left: auto; margin-right: auto; font: normal 12px/150% Arial, Helvetica, sans-serif; background: #fff; overflow: hidden; border: 3px solid #006699; -webkit-border-radius: 5px; -moz-border-radius: 5px; border-radius: 5px; }\r\n"
		".datagrid table { border-collapse: collapse; text-align: left; width: 100%; }\r\n"
		".datagrid table td, .datagrid table th { padding: 5px 15px; }\r\n"
		".datagrid table thead { background: -webkit-gradient( linear, left top, left bottom, color-stop(0.05,#006699), color-stop(1,#00557F)); background: -moz-linear-gradient( center top, #006699 5%, #00557F 100% ); filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#006699', endColorstr='#00557F'); background-color: #006699; }\r\n"
		".datagrid table thead th { background: -webkit-gradient( linear, left top, left bottom, color-stop(0.05,#006699), color-stop(1,#00557F)); background: -moz-linear-gradient( center top, #006699 5%, #00557F 100% ); filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#006699', endColorstr='#00557F'); background-color: #006699; }\r\n"
		".datagrid table thead td { background: #3EE6E6; padding:5; color:#185A5A; font-size:16px; font-weight:bold; text-align:center; }\r\n"
		".datagrid table thead td:first-child { -webkit-border-radius: 5px 0 0 0; -moz-border-radius: 5px 0 0 0; border-radius: 5px 0 0 0; }\r\n"
		".datagrid table thead td:last-child { -webkit-border-radius: 0 5px 0 0; -moz-border-radius: 0 5px 0 0; border-radius: 0 5px 0 0; }\r\n"
		".datagrid table tbody td { overlay: hidden; width: 33%; color: #00557F; font-size: 14px; font-weight: normal; text-align: center; }\r\n"
		".datagrid table tbody .alt td { cursor: pointer; border-top: 1px solid #009999; border-bottom: 1px solid #009999; background: #AEF5F5; font-weight:bold; }\r\n"
		".datagrid table tfoot td { padding: 0; background: #44FFEE; font-size: 10px; }\r\n"		
		"</style>\r\n</head>\r\n<body>\r\n<div id=\"logo\"></div>\r\n<div class=\"datagrid\"><table>"
		"<thead>\r\n<tr><th></th><th></th><th></th></tr>\r\n"
		"<tr><td>Hoster</td><td>Username</td><td>Password</td></tr>\r\n"
		"</thead><tfoot>\r\n<tr><td colspan=\"3\">&nbsp;</td></tr>\r\n</tfoot>\r\n";

	for (size_t i = 0; i < vHosts.size(); i++)
	{	
		strOutput.append("<tbody onclick=\"if(this.nextElementSibling.style.display==''){this.nextElementSibling.style.display='none';}else{this.nextElementSibling.style.display='';}\">")
				 .append("<tr class=\"alt\"><td>").append(vHosts[i].getHost()).append("</td><td></td><td>").append(vHosts[i].getSize()).append(" Account(s)</td></tr></tbody>\r\n")
				 .append("<tbody>\r\n").append(vHosts[i].getAccounts()).append("</tbody>\r\n");
	}

	strOutput.append("</table></div>\r\n</body>\r\n</html>");
	
	std::stringstream sstr; sstr << vHosts.size();

	strOutput.replace(strOutput.find("{ENTRIES}"), 9, sstr.str());
	strOutput.replace(strOutput.find("{B64IMG}"), 8, HeaderImage);

	/* Write to file */

	tm timeinfo;
	time_t rawtime;
	char pTimeBuf[80];

	time(&rawtime); localtime_s(&timeinfo, &rawtime);
	strftime(pTimeBuf, 80, "%d-%m-%Y_%H-%M-%S", &timeinfo);

	strFileName = std::string("jdecrypt_export_").append(pTimeBuf).append(".html");

	FileIO output(strFileName.c_str(), GENERIC_WRITE, CREATE_ALWAYS);

	if (!output.open())
		ReturnError(5, output.getError(), "input.open()");

	DWORD dwPos = 0;

	if (!output.write(reinterpret_cast<const BYTE*>(strOutput.c_str()), strOutput.size(), dwPos))
		ReturnError(5, output.getError(), "input.write()");
}