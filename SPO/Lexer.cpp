#include "Lexer.h"

Lexer::Lexer(std::string Input) {

	this->RawInput = Input;
}

bool Lexer::GenLexems() {

	size_t pos = 0;

	SkipSpaces(pos);

	while (pos != RawInput.length() && ( VAR(pos) || DIGIT(pos) || COMP_OP(pos) || OP(pos) || LOG_OP(pos) || ASSIGN_OP(pos) || SYMBOLS(pos)))
		SkipSpaces(pos);

	if (pos != RawInput.length()) {
		std::cout << "Lexer error" << std::endl;
		return false;
	}

	return true;
}

std::list<Token> Lexer::GetTokens() { return this->tokens; }

void Lexer::PrintTokens() {

	for (auto const& p : tokens)
		std::cout << p << std::endl;
}

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
			if (tokens.back().GetType() == "DATA_TYPE") 

				if (Functions.find(RawInput.substr(start, pos - start)) == Functions.end()) {

					tokens.push_back(Token("FUNCTION", RawInput.substr(start, pos - start)));
					Functions.insert(RawInput.substr(start, pos - start));
				}

				else
					return false;								
			
			else 
				if (Functions.find(RawInput.substr(start, pos - start)) == Functions.end()) 
					tokens.push_back(Token("VAR", RawInput.substr(start, pos - start)));

				else
					tokens.push_back(Token("FUNCTION", RawInput.substr(start, pos - start)));	
						

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
bool Lexer::ASSIGN_OP(size_t &pos) {

	if (RawInput[pos] == '=') {
		pos++;
		tokens.push_back(Token("ASSIGN_OP", "="));
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
		if (tokens.size() == 0 || tokens.back().GetType() == "L_BR" || tokens.back().GetType() == "OP")
			tokens.push_back(Token("UNARY_OP", "+"));

		else
			tokens.push_back(Token("OP", "+"));
		break;

	case '-':
		if (tokens.size() == 0 || tokens.back().GetType() == "L_BR" || tokens.back().GetType() == "OP")
			tokens.push_back(Token("UNARY_OP", "-"));

		else
			tokens.push_back(Token("OP", "-"));
		break;

	default:
		return false;
	}

	pos++;
	return true;
}

//Logic operator token
bool Lexer::LOG_OP(size_t &pos) {

	if (RawInput[pos] == '!') {
		tokens.push_back(Token("UNARY_LOG_OP", "!"));
		pos++;
		return true;
	}

	else if (RawInput.substr(pos, 2) == "&&") {
		tokens.push_back(Token("LOG_OP", "&&"));
		pos += 2;
		return true;
	}

	else if (RawInput.substr(pos, 2) == "||") {
		tokens.push_back(Token("LOG_OP", "||"));
		pos += 2;
		return true;
	}

	return false;
}

bool Lexer::COMP_OP(size_t &pos) {

	if (RawInput.substr(pos, 2) == "==") {
		tokens.push_back(Token("COMP_OP", "=="));
		pos += 2;
		return true;
	}

	else if (RawInput.substr(pos, 2) == "<=") {
		tokens.push_back(Token("COMP_OP", "<="));
		pos += 2;
		return true;
	}

	else if (RawInput.substr(pos, 2) == ">=") {
		tokens.push_back(Token("COMP_OP", ">="));
		pos += 2;
		return true;
	}

	else if (RawInput.substr(pos, 2) == "!=") {
		tokens.push_back(Token("COMP_OP", "!="));
		pos += 2;
		return true;
	}
	
	else if (RawInput[pos] == '<') {
		tokens.push_back(Token("COMP_OP", "<"));
		pos++;
		return true;
	}

	else if (RawInput[pos] == '>') {
		tokens.push_back(Token("COMP_OP", ">"));
		pos++;
		return true;
	}		

	return false;
}

bool Lexer::KEYWORDS(std::string word) {

	if (word == "if") {
		tokens.push_back(Token("IF_KW", "if"));
		return true;
	}

	else if (word == "else") {
		tokens.push_back(Token("ELSE_KW", "else"));
		return true;
	}

	else if (word == "while") {
		tokens.push_back(Token("WHILE_KW", "while"));
		return true;
	}

	else if (word == "define") {
		tokens.push_back(Token("DEFINE_KW", "define"));
		return true;
	}

	else if (word == "main") {
		tokens.push_back(Token("MAIN_KW", "main"));
		return true;
	}

	else if (word == "return") {
		tokens.push_back(Token("RETURN_KW", "return"));
		return true;
	}

	//data types
	else if (word == "void") {
		tokens.push_back(Token("DATA_TYPE", "void"));
		return true;
	}

	else if (word == "int") {
		tokens.push_back(Token("DATA_TYPE", "int"));
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
	
	case '{':
		tokens.push_back(Token("L_FIG", "{"));
		break;

	case '}':
		tokens.push_back(Token("R_FIG", "}"));
		break;

	case ';':
		tokens.push_back(Token("END_ST", ";"));
		break;

	case ',':
		tokens.push_back(Token("COMMA", ","));
		break;

	default:
		return false;
	}

	pos++;
	return true;
}

void Lexer::SkipSpaces(size_t &pos) {

	while (RawInput[pos] == '\t' || RawInput[pos] == ' ' || RawInput[pos] == '\n')
		pos++;
}

Lexer::~Lexer() {}