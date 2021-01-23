#include "Token.h"

Token::Token() {

	this->TokenType = "NO_DEF";
	this->val = "null";
}

Token::Token(std::string type, std::string value) {

	this->TokenType = type;
	this->val = value;
}

std::string Token::GetType() { return this->TokenType; }

std::string Token::GetValue() { return this->val; }

Token& Token::operator= (const Token &tk) {

	this->TokenType = tk.TokenType;
	this->val = tk.val;

	return *this;
}

std::ostream& operator << (std::ostream& os, const Token& tk) {

	os << "<" << tk.TokenType << ", " << tk.val << ">";
	return os;
}

bool operator!=(const Token& l, const Token& r) {
	
	return l.TokenType != r.TokenType || l.val != r.val;
}

Token::~Token(){}