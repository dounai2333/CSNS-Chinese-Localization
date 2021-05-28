#pragma once

class ArgParser
{
public:
	bool Open(int argc, const char** argv);
	void Clear();

	bool Exist(std::string arg);
	std::string GetValue(std::string arg);
private:
	int count = 1;
	std::string args[128] = {""};
};

extern ArgParser* Arg;