#pragma once
#include <unordered_map>
#include <string>
#include <list>
#include <iostream>

#include "Token.h"

class Lexer
{
private:

	std::string RawInput;

	//variable table(int only)
	std::unordered_map<std::string, int> ITable;
	
	std::list<std::string> keywords = {"if", "else", "while"};

	//Tokens
	std::list<Token> tokens;

	bool VAR(size_t &pos);
	bool DIGIT(size_t &pos);
	bool ASSIGN(size_t &pos);
	bool OP(size_t &pos);
	bool KEYWORDS(std::string word);
	bool SYMBOLS(size_t &pos);

	void SkipSpaces(size_t &pos);

public:

	Lexer(std::string Input);
	~Lexer();

	std::list<Token> GetTokens();

	bool GenLexems();
};

