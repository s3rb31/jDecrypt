#include "JDecrypt.h"

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		std::cout << "Usage: " << argv[0] << " [EJS-File]" << std::endl 
				  << "Coded bY S3RB31" << std::endl; 

		return 1;
	}

	std::basic_string<BYTE> strDecrypted = ReadAndDecryptEJS(argv[1]);
	std::vector<JHost> vHosts = ParseEJS(strDecrypted.c_str(), strDecrypted.size());

	OutputEJSAsHTML(vHosts);
	
	return 0;
}