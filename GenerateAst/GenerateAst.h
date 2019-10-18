#pragma once

void defineAst(std::string outputDir, std::string baseName, std::vector<std::string> types);

void defineType(std::ofstream & writer, std::string & baseName, std::string & className, std::string & fieldList);

std::vector<std::string> splitString(const std::string & str, char delim);
