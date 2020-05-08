#include "Token.h"

Token::Token(std::string type, std::string value) {

	this->TokenType = type;
	this->val = value;
}

std::string Token::GetType() { return this->TokenType; }

std::string Token::GetValue() { return this->val; }

std::ostream& operator << (std::ostream& os, const Token& tk) {

	os << "<" << tk.TokenType << ", " << tk.val << ">";
	return os;
}

Token::~Token(){}