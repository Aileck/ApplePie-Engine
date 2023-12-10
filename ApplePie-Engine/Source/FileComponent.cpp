#include "FileComponent.h"
#include <cwchar>
#include<string>

using namespace std;
const char* FileComponent::CreateFilePath(const char* address, const char* fileName) {
	size_t fullPathSize = strlen(address) + strlen(fileName) + 2;

	char* fullPath = new char[fullPathSize];

	strcpy_s(fullPath, fullPathSize, address);
	strcat_s(fullPath, fullPathSize, fileName);

	return fullPath;
}

const wchar_t* FileComponent::CreateWideFilePath(const char* address, const char* fileName) {
	size_t addressLen = strlen(address) + 1;
	size_t fileNameLen = strlen(fileName) + 1;

	wchar_t* wideAddress = new wchar_t[addressLen];
	wchar_t* wideFileName = new wchar_t[fileNameLen];

	mbstowcs_s(nullptr, wideAddress, addressLen, address, _TRUNCATE);
	mbstowcs_s(nullptr, wideFileName, fileNameLen, fileName, _TRUNCATE);

	size_t fullPathSize = wcslen(wideAddress) + wcslen(wideFileName) + 2;

	wchar_t* fullPath = new wchar_t[fullPathSize];

	wcscpy_s(fullPath, fullPathSize, wideAddress);

	if (fullPath[wcslen(fullPath) - 1] != L'/') {
		wcscat_s(fullPath, fullPathSize, L"/");
	}

	wcscat_s(fullPath, fullPathSize, wideFileName);

	delete[] wideAddress;
	delete[] wideFileName;

	return fullPath;
}

const wchar_t* FileComponent::ConvertToWideFilePath(const char* fileName) {
	size_t fileNameLen = 0;
	mbstowcs_s(&fileNameLen, nullptr, 0, fileName, 0);

	wchar_t* wideFileName = new wchar_t[fileNameLen + 1];
	mbstowcs_s(nullptr, wideFileName, fileNameLen + 1, fileName, fileNameLen);

	return wideFileName;
}

void FileComponent::AnalyseFilePath(const char* fullpath, char* filePath, char* fileName, char* fileExtension) {
	string fullPathString(fullpath);

	// Last slash
	size_t lastSlashPos = fullPathString.find_last_of('/');
	size_t lastBackslashPos = fullPathString.find_last_of('\\');

	if (lastSlashPos == string::npos || (lastBackslashPos != string::npos && lastBackslashPos > lastSlashPos)) {
		lastSlashPos = lastBackslashPos;
	}
	// Path
	string path = fullPathString.substr(0, lastSlashPos + 1);
	strcpy_s(filePath, _TRUNCATE, path.c_str());

	// Name
	string fileNameWithExtension = fullPathString.substr(lastSlashPos + 1);

	// Last dot
	size_t lastDotPos = fileNameWithExtension.find_last_of('.');

	// File name 
	string fileOnly = fileNameWithExtension.substr(0, lastDotPos);
	strcpy_s(fileName, _TRUNCATE, fileOnly.c_str());

	// File extension 
	string extension = fileNameWithExtension.substr(lastDotPos + 1);
	strcpy_s(fileExtension, _TRUNCATE, extension.c_str());

}
