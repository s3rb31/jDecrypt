#include <string>
#include "JTypes.h"

JAccount::JAccount(const picojson::object &account) : data(account) { }

bool JAccount::isUsable()
{
	std::string pNames[2] = { "user", "password" };

	for (int i = 0; i < (sizeof(pNames) / sizeof(pNames[0])); i++)
	{
		if (!this->data[pNames[i].c_str()].is<std::string>())
			return false;

		if (this->data[pNames[i].c_str()].to_str().size() == 0)
			return false;

		if (this->data[pNames[i].c_str()].to_str() == "??")
			return false;
	}

	if (this->data["properties"].is<picojson::object>() && this->data["properties"].contains("nopremium"))
	if (this->data["properties"].get<picojson::object>()["nopremium"].get<bool>())
		return false;

	return true;
}

std::string JAccount::getUser() 
{ 
	return this->data["user"].to_str(); 
}
std::string JAccount::getPass() 
{ 
	return this->data["password"].to_str(); 
}

std::string JAccount::getFormated()
{
	std::string strRet;

	strRet.append("<tr><td></td><td>").append(this->data["user"].to_str())
		.append("</td><td>").append(this->data["password"].to_str()).append("</td></tr>\r\n");

	return strRet;
}