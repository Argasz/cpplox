// cpplox.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma once
#include "cpplox.h"
#include "Scanner.h"
#include "AstPrinter.h"
#include "Parser.h"
#include "Interpreter.h"

bool hadError = false;
bool hadRuntimeError = false;

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
	std::string content((std::istreambuf_iterator<char>(inFile)), (std::istreambuf_iterator<char>()));

	run(content);

	if (hadError)
		return 65;
	if (hadRuntimeError)
		return 70;

	return 0;
}

int runPrompt()
{
	for (;;)
	{
		std::cout << "\n> ";
		char input[255];
		std::cin.getline(input, 255);
		run(input);
		hadError = false;
	}
	return 0;
}

int run(const std::string& source)
{
	Scanner scanner(source);
	std::vector<Token> tokens = scanner.scanTokens();
	auto parser = Parser(tokens);
	auto expression = parser.parse();
	if (hadError) return 1;

	const auto interpreter = Interpreter();
	interpreter.interpret(expression);

	return 0;
}

void cpploxError(int line, const std::string& message)
{
	report(line, "", message);
}

void report(int line, const std::string& whence, const std::string& message)
{
	fprintf(stderr, "[line %d ] Error %s: %s\n", line, whence.c_str(), message.c_str());
	hadError = true;
}

void cpploxError(const Token& token, const std::string& message)
{
	if (token.type == TokenType::EOFF)
	{
		report(token.line, " at end", message);
	}
	else
	{
		report(token.line, "at '" + token.lexeme + "'", message);
	}
}

void runtimeError(const LRunTimeError& e)
{
	std::cout << e.what();
	std::cout << "\n[line " << e.token.line << "]";
	hadRuntimeError = true;
}

void unpack_write_to_stream(std::ostream& stream, const Var_literal& var) {
	if (std::holds_alternative<std::monostate>(var)) { // Should prevent error due to missing << operator for std::monostate
		stream << " empty ";
	}
	else
	{
		auto visitorLambda = [&stream](auto&& _in) {
			return _in;
		};
		stream << " " << std::visit(visitorLambda, var) << " ";
	}
}