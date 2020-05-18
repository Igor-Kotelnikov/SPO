#pragma once
#include <string>
#include <iostream>
class Token
{

private:
	 std::string TokenType;
	 std::string val;

public:
	Token();
	Token(std::string type, std::string value);
	~Token();

	std::string GetType();
	std::string GetValue();

	Token& operator = (const Token &tk);
	friend std::ostream& operator << (std::ostream& os, const Token& tk);
};

