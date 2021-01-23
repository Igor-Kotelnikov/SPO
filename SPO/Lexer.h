#pragma once
#include <string>
#include <set>
#include <list>
#include <iostream>

#include "Token.h"

class Lexer
{
private:

	std::string RawInput;

	//Reserved function names
	std::set<std::string> Functions;
	
	//Tokens
	std::list<Token> tokens;

	bool VAR(size_t &pos);
	bool DIGIT(size_t &pos);
	bool ASSIGN_OP(size_t &pos);
	bool OP(size_t &pos);
	bool LOG_OP(size_t &pos);
	bool COMP_OP(size_t &pos);
	bool KEYWORDS(std::string word);
	bool SYMBOLS(size_t &pos);

	void SkipSpaces(size_t &pos);

public:

	Lexer(std::string Input);
	~Lexer();

	std::list<Token> GetTokens();
	void PrintTokens();

	bool GenLexems();
};

