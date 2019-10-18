// GenerateAst.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "GenerateAst.h"

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage: generate_ast <output directory>\n";
		return 1;
	}
	std::string outputDir = argv[1];
	std::vector<std::string> defs{
		"Binary:Expr left, Token operator, Expr right",
		"Grouping:Expr expression",
		"Literal:std::variant<std::string, double> value",
		"Unary:Token operator, Expr right"
	};
	defineAst(outputDir, "Expr", defs);
}

void defineAst(std::string outputDir, std::string baseName, std::vector<std::string> types)
{
	std::string path = outputDir + "/" + baseName + ".h";

	std::ofstream writer (path, std::ofstream::out);
	writer.write("#include \"Token.h\"\n", 20);
	writer.write("#include <vector>\n", 19);
	std::string classdef = "class " + baseName + "\n{\n}";
	writer.write(classdef.c_str(),classdef.length());

	for (auto type : types)
	{
		std::vector<std::string> classDef = splitString(type, ':');
		std::string className = classDef[0];
		std::string fields = classDef[1];

		defineType(writer, baseName, className, fields);
	}

}

void defineType(std::ofstream& writer, std::string& baseName, std::string& className, std::string& fieldList)
{
	std::string s("\nclass " + className + " : public baseName " + "\n{\n");
	writer.write(s.c_str(), s.length());
	s = "\t" + className + "(" + fieldList + ") {\n";
	writer.write(s.c_str(), s.length());

	std::vector<std::string> fields = splitString(fieldList, ',');
	for (auto field : fields)
	{
		s = "\t" + field + ";\n";
		writer.write(s.c_str(), s.length());
	}

	writer.write(" }", 3);
}


std::vector<std::string> splitString(const std::string& str, char delim)
{
	std::stringstream ss(str);
	std::string token;
	std::vector<std::string> returnVector;

	while (std::getline(ss, token, delim))
	{
		returnVector.push_back(token);
	}
	return returnVector;
}