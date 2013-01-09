// ReRenamerC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace System;
using namespace System::IO;
using namespace System::Linq;
using namespace System::Collections::Generic;
using namespace System::Text::RegularExpressions;

ref class FilenamePredicate
{
	Regex^ m_matcher;

public:
	FilenamePredicate(String^ pattern)
	{
		m_matcher = gcnew Regex(pattern);
	}

	bool Check(String^ name)
	{
		return m_matcher->IsMatch(name);
	}
};

array<String^>^ Find(String^ path, String^ pattern)
{
	try
	{
		array<String^>^ files = Directory::GetFiles(path, "*.*", SearchOption::TopDirectoryOnly);
		IEnumerable<String^>^ filenames = Enumerable::Select<String^>(files, gcnew Func<String^, String^>(Path::GetFileName));

		FilenamePredicate^ predicate = gcnew FilenamePredicate(pattern);

		return Enumerable::ToArray(Enumerable::Where<String^>(filenames, gcnew Func<String^, bool>(predicate, &FilenamePredicate::Check)));
	}
	catch(DirectoryNotFoundException^)
	{
		Console::WriteLine("Invalid path '{0}'", path);
		return gcnew array<String^>(0);
	}
}

void RenameOne(String^ path, String^ fromName, String^ toName, bool whatif)
{
	Console::WriteLine("{0} -> {1}", fromName, toName);
	if (whatif) return;
	           
	String^ fullFromName = Path::Combine(path, fromName);
	String^ fullToName = Path::Combine(path, toName);

	try
	{
		File::Move(fullFromName, fullToName);
	}
	catch (Exception^ e)
	{
		Console::WriteLine("Unable to rename '{0}' because of '{1}'", fromName, e->Message);
	}     
}

void Rename(String^ path, String^ pattern, String^ replacement, bool whatif, array<String^>^ files)
{
	for each (auto file in files)
	{
		String^ newName = Regex::Replace(file, pattern, replacement);
		RenameOne(path, file, newName, whatif);
	}
}

int FindAndRename(String^ path, String^ pattern, String^ replacement, bool whatif)
{
	if (whatif) Console::WriteLine("Test mode");

	Console::WriteLine("Looking for '{0}' in '{1}' and replacing with '{2}'", pattern, path, replacement);

	array<String^>^ files = Find(path, pattern);
	Rename(path, pattern, replacement, whatif, files);
	
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

