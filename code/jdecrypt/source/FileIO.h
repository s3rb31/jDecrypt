
class FileIO
{

private:

	HANDLE hFile;
	const char *pName;

	DWORD dwError;
	DWORD dwAccess, dwMode;
	DWORD dwShareMode, dwFlags;

public:

	~FileIO();
	FileIO(const char *pName, DWORD dwAccess, DWORD dwMode, DWORD dwShareMode = 0, DWORD dwFlags = 0);

	bool open();
	bool read(BYTE *pData, DWORD dwBufSize, DWORD &dwPos);
	bool write(const BYTE *pData, DWORD dwBufSize, DWORD &dwPos);

	size_t getSize();
	DWORD getError();
};
