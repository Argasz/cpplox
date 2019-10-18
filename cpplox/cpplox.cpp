// cpplox.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include "cpplox.h"
#include "Scanner.h"

bool hadError = false;

int main(int argc, char* argv[])
{
	if (argc > 2)
	{
		std::cout << "Usage: cpplox [script]";
		return 64;
	}
	else if (argc == 2)
	{
		return runFile(argv[1]);
	}
	else 
	{
		return runPrompt();
	}
}

int runFile(const std::string& path)
{
	std::ifstream inFile;
	inFile.open(path, std::fstream::in);
	std::string content( (std::istreambuf_iterator<char>(inFile)), (std::istreambuf_iterator<char>()));

	run(content);

	if (hadError)
		return 65;
	
	return 0;
}

int runPrompt()
{
	for (;;)
	{
		std::cout << "> ";
		char input[255];
		std::cin.getline(input,255);
		run(input);
		hadError = false;
	}
	return 0;
}

int run(const std::string& source)
{
	Scanner scanner(source);
	std::vector<Token*> tokens = scanner.scanTokens();

	for (auto token : tokens)
	{
		std::cout << *token << '\n';
	}
	return 0;
}

void error(int line, const std::string& message)
{
	report(line,"",message);
}

void report(int line, const std::string& whence, const std::string& message)
{
	fprintf(stderr, "[line %d ] Error %s: %s",line, whence, message);
}