#include "PicoJSON.h"

class JAccount
{
private:

	picojson::object data;

public:

	JAccount(const picojson::object &AccountData);

	bool isUsable();

	std::string getUser();
	std::string getPass();
	std::string getFormated();
};


class JHost
{
private:

	std::string strHost;
	std::vector<JAccount> vAccounts;

public:

	JHost(const char *pName);

	size_t size();
	void pushAccount(JAccount Account);
		
	std::string getHost();	
	std::string getAccounts();
};