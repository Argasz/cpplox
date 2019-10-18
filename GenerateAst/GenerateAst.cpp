// GenerateAst.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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
		"Binary:Expr left,Token op,Expr right",
		"Grouping:Expr expression",
		"Literal:VARIANT value",
		"Unary:Token op,Expr right"
	};
	defineAst(outputDir, "Expr", defs);
}

void defineAst(std::string outputDir, std::string baseName, std::vector<std::string> types)
{
	std::string hPath = outputDir + baseName + ".h";
	std::string cppPath = outputDir + baseName + ".cpp";

	std::ofstream writerHead (hPath, std::ofstream::out);
	writerHead.write("#include \"Token.h\"\n", 19);
	writerHead.write("#include <vector>\n", 18);
	writerHead.write("#include <variant>\n", 19);
	writerHead.write("#define VARIANT std::variant<std::string, double>\n", 50);
	std::string classdef = "class " + baseName + "\n{\n};";
	writerHead.write(classdef.c_str(),classdef.length());
	genVisitor(writerHead, baseName, types);

	std::ofstream writerCpp(cppPath, std::ofstream::out);
	std::string temp = "#include \"" + baseName + ".h\"\n";
	writerCpp.write(temp.c_str(), temp.length());

	for (auto type : types)
	{
		std::vector<std::string> classDef = splitString(type, ':');
		std::string className = classDef[0];
		std::string fields = classDef[1];

		genHeader(writerHead, baseName, className, fields);
		genCpp(writerCpp, baseName, className, fields);
	}
	writerHead.close();
	writerCpp.close();

}

void genHeader(std::ofstream& writerHead, std::string& baseName, std::string& className, std::string& fieldList)
{
	std::string s("\nclass " + className + " : public " + baseName + "\n{\n");
	writerHead.write(s.c_str(), s.length());
	s = "\t" + className + "(" + fieldList + ");\n";
	writerHead.write(s.c_str(), s.length());

	std::vector<std::string> fields = splitString(fieldList, ',');
	for (auto field : fields)
	{
		s = "\t" + field + ";\n";
		writerHead.write(s.c_str(), s.length());
	}

	writerHead.write(" };\n", 3);
	
	s = "\tvoid accept(ExprVisitor& visitor) { return std::visit(visitor, this) };\n";
	writerHead.write(s, s.length());
}

void genCpp(std::ofstream& writerCpp, std::string & baseName, std::string & className, std::string& fieldList)
{
	std::string s = className + "::" + className + "(" + fieldList + ") : ";
	writerCpp.write(s.c_str(),s.length());
	std::vector<std::string> fields = splitString(fieldList, ',');

	for (int i = 0; i < fields.size(); i++ )
	{
		int space = fields[i].find_first_of(" ");
		std::string field = fields[i].substr(space + 1, fields[i].length() - space);
		s = field + "(" + field + ")";
		if (i != fields.size() - 1) {
			s += ", ";
		}
		writerCpp.write(s.c_str(), s.length());
	}

	writerCpp.write("\n{};\n", 5);
}

void genVisitor(std::ofstream & visitWriter, std::string baseName, std::vector<std::string> types)
{
	std::string s = "\nstruct " + baseName + "Visitor\n{\n";
	visitWriter.write(s.c_str(), s.length());

	for (auto type : types)
	{
		std::vector<std::string> typeNames = splitString(type, ':');
		std::string visitorFunc = "void operator()(" + typeNames[0] + "&);\n";
		visitWriter.write(visitorFunc.c_str(), visitorFunc.length());
	}

	visitWriter.write("\n};\n", 4);
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
