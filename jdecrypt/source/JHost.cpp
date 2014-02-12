#include <sstream>

#include "JTypes.h"

JHost::JHost(const char *pName)
	: strHost(pName) { }

void JHost::pushAccount(JAccount Account)
{ 
	this->vAccounts.push_back(Account); 
}

std::string JHost::getSize()
{
	std::stringstream sstr;
	sstr << this->vAccounts.size();

	return sstr.str();
}

std::string JHost::getHost()
{
	return strHost;
}

std::string JHost::getAccounts()
{
	std::string strRet;

	for (size_t i = 0; i < this->vAccounts.size(); i++)
		strRet.append(this->vAccounts[i].getFormated());

	return strRet;
}