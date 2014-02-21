#include <windows.h>
#include <wincrypt.h>

#include "CryptAES.h"

#pragma comment(lib, "advapi32.lib")

CryptAES::CryptAES(BYTE *pKey, BYTE *pIV)
{
	this->hProv = NULL;
	this->hKey = NULL;

	if (CryptAcquireContext(&this->hProv, NULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, 0))
	{
		AESKEY128 AESBlob;
		AESBlob.Header.bType = PLAINTEXTKEYBLOB;
		AESBlob.Header.bVersion = CUR_BLOB_VERSION;
		AESBlob.Header.reserved = 0;
		AESBlob.Header.aiKeyAlg = CALG_AES_128;
		AESBlob.key_length = 16;
		memcpy_s(AESBlob.key_bytes, 16, pKey, 16);

		CryptImportKey(this->hProv, reinterpret_cast<BYTE*>(&AESBlob), sizeof(AESBlob), NULL, 0, &this->hKey);

		if (this->hKey)
		{
			DWORD dwMode = CRYPT_MODE_CBC,
				dwPadding = PKCS5_PADDING;

			CryptSetKeyParam(this->hKey, KP_IV, pIV, 0);
			CryptSetKeyParam(this->hKey, KP_MODE, reinterpret_cast<BYTE*>(&dwMode), 0);
			CryptSetKeyParam(this->hKey, KP_PADDING, reinterpret_cast<BYTE*>(&dwPadding), 0);
		}
	}
}

CryptAES::~CryptAES()
{
	if (this->hKey != NULL) 
		CryptDestroyKey(this->hKey);

	if (this->hProv != NULL) 
		CryptReleaseContext(this->hProv, 0);
}

int CryptAES::Encrypt(BYTE *pData, DWORD *pdwDataSize, DWORD dwBufferSize, BOOL bFinal)
{
	if (this->hKey == NULL)
		return -1;

	CryptEncrypt(this->hKey, 0, bFinal, 0, pData, pdwDataSize, dwBufferSize);

	return GetLastError();
}

int CryptAES::Decrypt(BYTE *pData, DWORD *pdwDataSize, BOOL bFinal)
{
	if (this->hKey == NULL)
		return -1;

	CryptDecrypt(this->hKey, 0, bFinal, 0, pData, pdwDataSize);

	return GetLastError();
}