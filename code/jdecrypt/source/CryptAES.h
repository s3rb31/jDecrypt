
class CryptAES 
{

private:

	HCRYPTPROV hProv;
	HCRYPTKEY  hKey;

public:

	struct AESKEY128 
	{
		BLOBHEADER Header;
		DWORD dwKeyLen;
		BYTE pKey[16];

		AESKEY128()
		{
			this->Header.bType = PLAINTEXTKEYBLOB;
			this->Header.bVersion = CUR_BLOB_VERSION;
			this->Header.reserved = 0;
			this->Header.aiKeyAlg = CALG_AES_128;
			this->dwKeyLen = 16;
		}
	};

	~CryptAES();
	CryptAES(BYTE *pKey, BYTE *pIV);
	
	int Encrypt(BYTE *pData, DWORD *pdwDataSize, DWORD dwBufferSize, BOOL bFinal);
	int Decrypt(BYTE *pData, DWORD *pdwDataSize, BOOL bFinal);
};