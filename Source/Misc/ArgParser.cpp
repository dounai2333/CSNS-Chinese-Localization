#include "..\main.h"

ArgParser* Arg = new ArgParser();

bool ArgParser::Open(int argc, const char** argv)
{
	Clear();

	if (argc == 1)
		return true;

	for (int i = 1; i < argc; i++)
		args[i] = argv[i];
	count = argc;

	return args[1] != "";
}

void ArgParser::Clear()
{
	Arg->count = NULL;
	for (int i = 0; i < CHAR_MAX; i++)
		args[i] = "";
}

bool ArgParser::Exist(std::string arg)
{
	for (int i = 1; i < count; i++)
		if (args[i] == arg)
			return true;

	return false;
}

std::string ArgParser::GetValue(std::string arg)
{
	for (int i = 1; i < count; i++)
		if (args[i] == arg)
			return args[i + 1];

	return "";
}