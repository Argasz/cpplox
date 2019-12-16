#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

class Token;
class LRunTimeError;
class Interpreter;

extern bool hadError;
extern bool hadRunTimeError;
extern Interpreter* interpreter;
int runFile(const std::string & path);
int runPrompt();
int run(const std::string & source);
void cpploxError(int line, const std::string & message);
void cpploxError(const Token& token, const std::string& message);
void runtimeError(const LRunTimeError& e);
void report(int line, const std::string & whence, const std::string & message);

