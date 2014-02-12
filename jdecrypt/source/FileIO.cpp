#include <windows.h>
#include "FileIO.h"

FileIO::FileIO(const char *pName, DWORD dwAccess, DWORD dwMode, DWORD dwShareMode, DWORD dwFlags)
	: pName(pName), dwAccess(dwAccess), dwMode(dwMode), dwShareMode(dwShareMode), dwFlags(dwFlags) { }

FileIO::~FileIO()
{
	if (this->pName)
		this->pName = 0;

	if (this->hFile)
		CloseHandle(this->hFile);
}

bool FileIO::open()
{
	this->hFile = CreateFileA(this->pName, this->dwAccess, this->dwShareMode, 0, this->dwMode, this->dwFlags, 0);

	if (this->hFile == INVALID_HANDLE_VALUE)
	{
		this->dwError = GetLastError();
		return false;
	}

	return true;
}

bool FileIO::read(BYTE *pData, DWORD dwBufSize, DWORD &dwPos)
{
	if (pData == NULL)
		return false;

	if (this->dwAccess &= ~GENERIC_READ)
	{
		this->dwError = ERROR_ACCESS_DENIED;
		return false;
	}

	if (SetFilePointer(this->hFile, dwPos, 0, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		this->dwError = GetLastError();
		return false;
	}

	DWORD dwRead = 0;

	if (!ReadFile(hFile, pData, dwBufSize, &dwRead, 0))
	{
		this->dwError = GetLastError();
		return false;
	}

	dwPos += dwRead;

	if (dwRead < dwBufSize)
	{
		this->dwError = GetLastError();
		return false;
	}

	return true;
}

bool FileIO::write(const BYTE *pData, DWORD dwBufSize, DWORD &dwPos)
{
	if (pData == NULL)
		return false;

	if (this->dwAccess &= ~GENERIC_WRITE)
	{
		this->dwError = ERROR_ACCESS_DENIED;
		return false;
	}

	if (SetFilePointer(this->hFile, dwPos, 0, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		this->dwError = GetLastError();
		return false;
	}

	DWORD dwWritten = 0;

	if (!WriteFile(hFile, pData, dwBufSize, &dwWritten, 0))
	{
		this->dwError = GetLastError();
		return false;
	}

	dwPos += dwWritten;

	if (dwWritten != dwBufSize)
	{
		this->dwError = GetLastError();
		return false;
	}

	return true;
}

size_t FileIO::getSize()
{
	return GetFileSize(this->hFile, 0);
}

DWORD FileIO::getError()
{
	return this->dwError;
}