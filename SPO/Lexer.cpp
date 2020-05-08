#include "Lexer.h"

Lexer::Lexer(std::string Input) {

	this->RawInput = Input;
}

bool Lexer::GenLexems() {

	size_t pos = 0;

	SkipSpaces(pos);

	while (pos != RawInput.length() && ( VAR(pos) || DIGIT(pos) || ASSIGN(pos) || OP(pos) ))
		SkipSpaces(pos);

	if (pos != RawInput.length()) {
		std::cout << "Lexer error" << std::endl;
		return false;
	}

	return true;
}

std::list<Token> Lexer::GetTokens() { return this->tokens; }

//Variable token
bool Lexer::VAR(size_t &pos) {

	size_t start = pos;

	if (RawInput[pos] == '_' ||
		RawInput[pos] >= 'a' && RawInput[pos] <= 'z' ||
		RawInput[pos] >= 'A' && RawInput[pos] <= 'Z') {

		pos++;
		while  (RawInput[pos] == '_' ||
				RawInput[pos] >= 'a' && RawInput[pos] <= 'z' ||
				RawInput[pos] >= 'A' && RawInput[pos] <= 'Z' ||
				RawInput[pos] >= '0' && RawInput[pos] <= '9')

			pos++;

		if (!KEYWORDS(RawInput.substr(start, pos - start)))
			tokens.push_back(Token("VAR", RawInput.substr(start, pos - start)));
		return true;
	}

	return false;
}

//Digit token(int only)
bool Lexer::DIGIT(size_t &pos) {

	size_t start = pos;

	if (RawInput[pos] >= '0' && RawInput[pos] <= '9') {

		pos++;
		while (RawInput[pos] >= '0' && RawInput[pos] <= '9')
			pos++;

		if (!KEYWORDS(RawInput.substr(start, pos - start)))	
			tokens.push_back(Token("DIGIT", RawInput.substr(start, pos - start)));

		return true;
	}

	return false;
}

//Assign operator token( '=' only)
bool Lexer::ASSIGN(size_t &pos) {

	if (RawInput[pos] == '=') {
		pos++;
		tokens.push_back(Token("ASSIGN", "="));
		return true;
	}

	return false;

}

//Math operator token ('*', '/', '+', '-' only)
bool Lexer::OP(size_t &pos) {

	switch (RawInput[pos])
	{
	case '*':
		tokens.push_back(Token("OP", "*"));
		break;

	case '/':
		tokens.push_back(Token("OP", "/"));
		break;

	case '+':
		tokens.push_back(Token("OP", "+"));
		break;

	case '-':
		tokens.push_back(Token("OP", "-"));
		break;

	default:
		return false;
	}

	pos++;
	return true;
}

bool Lexer::KEYWORDS(std::string word) {

	if (word == "if") {
		tokens.push_back(Token("KEYWORD", "if"));
		return true;
	}

	else if (word == "else") {
		tokens.push_back(Token("KEYWORD", "else"));
		return true;
	}

	else if (word == "while") {
		tokens.push_back(Token("KEYWORD", "while"));
		return true;
	}

	return false;
}

bool Lexer::SYMBOLS(size_t &pos) {

	switch (RawInput[pos])
	{
	case '(':
		tokens.push_back(Token("L_BR", "("));
		break;

	case ')':
		tokens.push_back(Token("R_BR", ")"));
		break;

	case ';':
		tokens.push_back(Token("END_ST", ";"));
		break;

	default:
		return false;
	}

	pos++;
	return true;
}

void Lexer::SkipSpaces(size_t &pos) {

	while (RawInput[pos] == '/t' || RawInput[pos] == ' ' || RawInput[pos] == '/n')
		pos++;
}

Lexer::~Lexer() {}