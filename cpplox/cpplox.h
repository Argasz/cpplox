#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

extern bool hadError;
int runFile(const std::string & path);
int runPrompt();
int run(const std::string & source);
void error(int line, const std::string & message);
void report(int line, const std::string & whence, const std::string & message);

