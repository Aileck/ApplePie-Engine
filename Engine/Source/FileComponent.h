#pragma once
class FileComponent
{
public:
	static const char* CreateFilePath(const char* address, const char* fileName);
	static const wchar_t* CreateWideFilePath(const char* address, const char* fileName);

	static const char* AnalyseFilePath(const char* fullpath, char* filePath, char* fileName, char* fileExtension);
};

