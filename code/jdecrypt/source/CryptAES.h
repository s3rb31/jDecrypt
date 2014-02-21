
class CryptAES 
{

private:

	HCRYPTPROV hProv;
	HCRYPTKEY  hKey;

public:

	struct AESKEY128 
	{
		BLOBHEADER Header;
		DWORD key_length;
		BYTE key_bytes[16];
	};

	~CryptAES();
	CryptAES(BYTE *pKey, BYTE *pIV);
	
	int Encrypt(BYTE *pData, DWORD *pdwDataSize, DWORD dwBufferSize, BOOL bFinal);
	int Decrypt(BYTE *pData, DWORD *pdwDataSize, BOOL bFinal);
};