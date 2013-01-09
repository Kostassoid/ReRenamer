// RenamerCow.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace System;
using namespace System::IO;

array<String^>^ Find(String^ path, String^ pattern)
{
	array<String^>^ files = Directory::GetFiles(path, "*.*", SearchOption::TopDirectoryOnly);

	return files;


                //.Select(Path.GetFileName)
                //.Where(f => Regex.IsMatch(f, pattern));
}

void Rename(String^ path, String^ pattern, String^ replacement, bool whatif, array<String^>^ files)
{
}

int FindAndRename(String^ path, String^ pattern, String^ replacement, bool whatif)
{
	if (whatif) Console::WriteLine("Test mode");

	Console::WriteLine("Looking for '{0}' in '{1}' and replacing with '{2}'", pattern, path, replacement);

	try
	{
		array<String^>^ files = Find(path, pattern);
		Rename(path, pattern, replacement, whatif, files);
	}
	catch(DirectoryNotFoundException^)
	{
		Console::WriteLine("Invalid path '{0}'", path);
	}
	finally
	{

	}
	
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 4 || argc > 5)
	{
		Console::WriteLine("Usage: ReRenamer <path> <pattern> <replacement> [-whatif]");
		return 1;
	}

	String^ path = gcnew String(argv[1]);
	String^ pattern = gcnew String(argv[2]);
    String^ replacement = gcnew String(argv[3]);
	bool whatif = argc > 4 && _tcscmp(argv[4], L"-whatif") == 0;

    return FindAndRename(path, pattern, replacement, whatif);
}

