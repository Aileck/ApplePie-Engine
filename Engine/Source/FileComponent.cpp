#include "FileComponent.h"

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

void AnalyseFilePath(const char* fullpath, char* filePath, char* fileName, char* fileExtension) {
	// Use C++ string class for convenient string manipulation
	string fullPathString(fullpath);

	// Find the position of the last slash
	size_t lastSlashPos = fullPathString.find_last_of('/');

	// Extract the path part
	string path = fullPathString.substr(0, lastSlashPos + 1);
	strcpy(filePath, path.c_str());

	// Extract the file name part
	string fileNameWithExtension = fullPathString.substr(lastSlashPos + 1);

	// Find the position of the last dot
	size_t lastDotPos = fileNameWithExtension.find_last_of('.');

	// Extract the file name part
	string fileOnly = fileNameWithExtension.substr(0, lastDotPos);
	strcpy(fileName, fileOnly.c_str());

	// Extract the file extension part
	string extension = fileNameWithExtension.substr(lastDotPos + 1);
	strcpy(fileExtension, extension.c_str());
}