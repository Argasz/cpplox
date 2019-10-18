#include "Scanner.h"
#include "cpplox.h"
#include <exception>

bool Scanner::isAtEnd()
{
	return current >= source.length();
}

char Scanner::advance()
{
	return source.at(current++);
}

void Scanner::addToken(TokenType type, std::variant<double, std::string> literal)
{
	std::string text = source.substr(start, current-start);
	tokens.push_back(new Token(type, text, literal, line));
}

void Scanner::scanToken()
{
	char c = advance();
	switch (c)
	{
	case '(': addToken(TokenType::LEFT_PAREN); break;
	case ')': addToken(TokenType::RIGHT_PAREN); break;
	case '{': addToken(TokenType::LEFT_BRACE); break;
	case '}': addToken(TokenType::RIGHT_BRACE); break;
	case ',': addToken(TokenType::COMMA); break;
	case '.': addToken(TokenType::DOT); break;
	case '-': addToken(TokenType::MINUS); break;
	case '+': addToken(TokenType::PLUS); break;
	case ';': addToken(TokenType::SEMICOLON); break;
	case '*': addToken(TokenType::STAR); break;
	case '!': addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
	case '=': addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
	case '<': addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
	case '>': addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;
	case '/':
		if (match('/'))
		{
			while (peek() != '\n' && !isAtEnd())
				advance();
		}
		else
		{
			addToken(TokenType::SLASH);
		}
		break;
	case ' ':
	case '\r':
	case '\t':
		// Ignore whitespace.
		break;
	case '\n':
		++line;
		break;
	case '"': 
		string(); 
		break;
	default: 
		if (isDigit(c))
		{
			number();
		}
		else if (isAlpha(c))
		{
			identifier();
		}
		else
		{
			error(line, "Unexpected character.");
		}
		break;
	}
}

bool Scanner::match(char expected)
{
	if (isAtEnd())
		return false;
	if (source.at(current) != expected)
		return false;

	++current;
	return true;
}

char Scanner::peek()
{
	if (isAtEnd())
		return '\0';

	return source.at(current);
}

void Scanner::string()
{
	while (peek() != '"' && !isAtEnd())
	{
		if (peek() == '\n')
			++line;

		advance();
	}

	if (isAtEnd())
	{
		error(line, "Unterminated string.");
		return;
	}

	advance();

	char test = peek();
	
	std::string value = source.substr(start + 1, current-start-2);
	addToken(TokenType::STRING, value);
}

bool Scanner::isDigit(char c)
{
	return c >= '0' && c <= '9';
}

void Scanner::number()
{
	while (isDigit(peek()))
		advance();

	// Look for a fractional part.
	if (peek() == '.' && isDigit(peekNext()))
	{
		// Consume the "."
		advance();
		while (isDigit(peek())) advance();
	}

	addToken(TokenType::NUMBER, std::stod(source.substr(start, current-start)));
}

char Scanner::peekNext()
{
	if (current + 1 >= source.length())
		return '\0';

	return source.at(current + 1);
}

void Scanner::identifier()
{
	while (isAlphaNumeric(peek()))
		advance();
	
	std::string text = source.substr(start, current-start);
	TokenType type;
	try{
		type = keywords.at(text);
		addToken(type);
	}
	catch(...)
	{
		type = TokenType::IDENTIFIER;
		addToken(type);
	}

	
}

bool Scanner::isAlpha(char c)
{
	return	(c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') ||
			c == '_';
}

bool Scanner::isAlphaNumeric(char c)
{
	return isAlpha(c) || isDigit(c);
}

std::vector<Token*> Scanner::scanTokens()
{
	while (!isAtEnd())
	{
		start = current;
		scanToken();
	}
	tokens.push_back(new Token(TokenType::EOFF, "", "", line));
	return tokens;
}

Scanner::Scanner(std::string source)
{
	this->source = source;
	keywords.emplace("and",	TokenType::AND);
	keywords.emplace("class", TokenType::CLASS);
	keywords.emplace("else", TokenType::ELSE);
	keywords.emplace("false", TokenType::FALSE);
	keywords.emplace("for", TokenType::FOR);
	keywords.emplace("fun", TokenType::FUN);
	keywords.emplace("if", TokenType::IF);
	keywords.emplace("nil", TokenType::NIL);
	keywords.emplace("or", TokenType::OR);
	keywords.emplace("print", TokenType::PRINT);
	keywords.emplace("return", TokenType::RETURN);
	keywords.emplace("super", TokenType::SUPER);
	keywords.emplace("this", TokenType::THIS);
	keywords.emplace("true", TokenType::TRUE);
	keywords.emplace("var", TokenType::VAR);
	keywords.emplace("while", TokenType::WHILE);

}
