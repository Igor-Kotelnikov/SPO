#include "Parser.h"

Parser::Parser(std::list<Token> token_list) {

	this->tokens = token_list;
}

bool Parser::CheckSyntax() {

	auto pos = tokens.begin();
	
	if (!func_define(pos) || !main_define(pos))
		return false;
	
	if (pos != tokens.end()) {

		std::cout << "Parser error" << std::endl;
		return false;
	}

	return true;
}

bool Parser::func_define(std::list<Token>::iterator& lex_pos) {

	while (1) {

		auto start = lex_pos;
		if (!func_init_expr(lex_pos))
			if (lex_pos != start)
				return false;
			else
				return true;
	}
}

bool Parser::main_define(std::list<Token>::iterator& lex_pos) {

	if (!MAIN_KW(lex_pos) || !L_BR(lex_pos) || !R_BR(lex_pos) || !L_FIG(lex_pos))
		return false;

	while (1) {

		auto start = lex_pos;
		if (!lang(lex_pos))
			if (lex_pos != start)
				return false;
			else
				break;
	}

	if (!R_FIG(lex_pos))
		return false;

	return true;
}

bool Parser::lang(std::list<Token>::iterator &lex_pos) {

	if (lex_pos == tokens.end())
		return false;

	auto start = lex_pos;
	
	if (!assign_expr(lex_pos)) {
		if (lex_pos != start)
			return false;
	}

	else
		return true;

	if (!if_expr(lex_pos)) {
		if (lex_pos != start)
			return false;
	}

	else
		return true;

	if (!while_expr(lex_pos)) {
		if (lex_pos != start)
			return false;
	}

	else
		return true;

	if (!return_expr(lex_pos)) {
		if (lex_pos != start)
			return false;
	}

	else
		return true;

	return false;
}

bool Parser::func_init_expr(std::list<Token>::iterator& lex_pos) {

	if (lex_pos == tokens.end())
		return false;

	auto start = lex_pos;

	if (!DEFINE_KW(lex_pos) || !DATA_TYPE(lex_pos) || !FUNCTION(lex_pos) || !L_BR(lex_pos) || !args_init_expr(lex_pos) || !R_BR(lex_pos) || !L_FIG(lex_pos))
		return false;

	while (1) {

		start = lex_pos;

		if (!lang(lex_pos))
			if (lex_pos != start)
				return false;
			else
				break;
	}

	if (!R_FIG(lex_pos))
		return false;

	return true;	
}

bool Parser::args_init_expr(std::list<Token>::iterator& lex_pos) {

	if (lex_pos == tokens.end())
		return false;

	auto start = lex_pos;
	CurrentFunctionArgumentsCounter = 0;

	if (VAR(lex_pos)) {

		CurrentFunctionArgumentsCounter++;

		while (COMMA(lex_pos)) {
			
			CurrentFunctionArgumentsCounter++;

			if (!VAR(lex_pos))
				return false;
		}
	}
	
	if (!FunctionsArgumentsNumber.insert({ CurrentFunctionName, CurrentFunctionArgumentsCounter }).second) {

		std::cout << "Unable to remember number of arguments for function" << std::endl;
		return false;
	}

	return true;
}

bool Parser::args_expr(std::list<Token>::iterator& lex_pos) {

	if (lex_pos == tokens.end())
		return false;

	auto start = lex_pos;
	CurrentFunctionArgumentsCounter = 0;

	if (value_expr(lex_pos)) {

		CurrentFunctionArgumentsCounter++;

		while (COMMA(lex_pos)) {
			
			CurrentFunctionArgumentsCounter++;

			if (!value_expr(lex_pos)) 
				return false;
			
		}
	}

	if (FunctionsArgumentsNumber[CurrentFunctionName] != CurrentFunctionArgumentsCounter)
		return false;

	return true;
}

bool Parser::assign_expr(std::list<Token>::iterator &lex_pos) {

	if (lex_pos == tokens.end())
		return false;

	auto start = lex_pos;

	if (!VAR(lex_pos) || !ASSIGN_OP(lex_pos))
		return false;	

	if (value_expr(lex_pos)) {

		while (value_expr(lex_pos));

		if (END_ST(lex_pos))
			return true;
	}
	
	return false;
}

bool Parser::func_expr(std::list<Token>::iterator& lex_pos) {

	auto start = lex_pos;

	if (lex_pos == tokens.end())
		return false;

	if (!FUNCTION(lex_pos) || !L_BR(lex_pos) || !args_expr(lex_pos) || !R_BR(lex_pos)) {

		lex_pos = start;
		return false;
	}

	return true;
}

bool Parser::value_expr(std::list<Token>::iterator &lex_pos) {

	auto start = lex_pos;

	if (lex_pos == tokens.end())
		return false;		

	if (!value(lex_pos) && (!L_BR(lex_pos) || !value_expr(lex_pos) || !R_BR(lex_pos))) {

		lex_pos = start;
		return false;
	}

	while (OP(lex_pos))
		if (!value_expr(lex_pos)) {

			lex_pos = start;
			return false;
		}

	return true;
}

bool Parser::if_expr(std::list<Token>::iterator &lex_pos) {

	if (lex_pos == tokens.end())
		return false;

	if (!IF_KW(lex_pos) || !L_BR(lex_pos) || !logic_expr(lex_pos) || !R_BR(lex_pos))
		return false;
	
	if (L_FIG(lex_pos)) {

		while (lang(lex_pos));

		if (!R_FIG(lex_pos))
			return false;


		if (!else_expr(lex_pos))
			return false;

		return true;
	}

	if (!lang(lex_pos))
		return false;

	if (!else_expr(lex_pos))
		return false;

	return true;
}

bool Parser::else_expr(std::list<Token>::iterator &lex_pos) {

	if (lex_pos == tokens.end())
		return true;

	if (ELSE_KW(lex_pos)) {

		if (L_FIG(lex_pos)) {

			while (lang(lex_pos));

			if (!R_FIG(lex_pos))
				return false;

			return true;
		}

		if (!lang(lex_pos))
			return false;
	}
		
	return true;
}

bool Parser::while_expr(std::list<Token>::iterator &lex_pos) {

	if (lex_pos == tokens.end())
		return false;

	if (!WHILE_KW(lex_pos) || !L_BR(lex_pos) || !logic_expr(lex_pos) || !R_BR(lex_pos))
		return false;

	if (L_FIG(lex_pos)) {

		while (lang(lex_pos));

		if (!R_FIG(lex_pos))
			return false;

		return true;
	}

	else if (!lang(lex_pos))
		return false;

	return true;
}

bool Parser::return_expr(std::list<Token>::iterator& lex_pos) {

	auto start = lex_pos;

	if (!RETURN_KW(lex_pos) || !value_expr(lex_pos) || !END_ST(lex_pos))
		return false;

	return true;
}

bool Parser::logic_expr(std::list<Token>::iterator &lex_pos) {

	auto start = lex_pos;

	if (lex_pos == tokens.end())
		return false;

	if (!comp_expr(lex_pos) && (!L_BR(lex_pos) || !logic_expr(lex_pos) || !R_BR(lex_pos))) {

		lex_pos = start;
		return false;
	}

	while (LOG_OP(lex_pos))
		if (!comp_expr(lex_pos)) {

			lex_pos = start;
			return false;
		}

	return true;
}

bool Parser::comp_expr(std::list<Token>::iterator &lex_pos) {

	auto start = lex_pos;

	if (lex_pos == tokens.end())
		return false;

	if (!value_expr(lex_pos)) {

		lex_pos = start;
		return false;
	}

	if (COMP_OP(lex_pos) && !value_expr(lex_pos)) {

		lex_pos = start;
		return false;
	}
		
	return true;
}

bool Parser::value(std::list<Token>::iterator &lex_pos) {

	if (lex_pos == tokens.end())
		return false;
	
	auto start = lex_pos;

	if (VAR(lex_pos))
		return true;
	
	if (DIGIT(lex_pos))
		return true;

	if (func_expr(lex_pos) && FunctionsType[start->GetValue()] == "int")
		return true;

	return false;
}

bool Parser::DEFINE_KW(std::list<Token>::iterator& lex_pos) {

	if (lex_pos == tokens.end())
		return false;

	if (lex_pos->GetType() == "DEFINE_KW") {
		++lex_pos;
		return true;
	}

	return false;
}

bool Parser::MAIN_KW(std::list<Token>::iterator& lex_pos) {

	if (lex_pos == tokens.end())
		return false;

	if (lex_pos->GetType() == "MAIN_KW") {
		++lex_pos;
		return true;
	}

	return false;

}

bool Parser::DATA_TYPE(std::list<Token>::iterator& lex_pos) {

	if (lex_pos == tokens.end())
		return false;

	if (lex_pos->GetType() == "DATA_TYPE") {
		++lex_pos;
		return true;
	}

	return false;
}

bool Parser::IF_KW(std::list<Token>::iterator &lex_pos) {

	if (lex_pos == tokens.end())
		return false;

	if (lex_pos->GetType() == "IF_KW") {
		++lex_pos;
		return true;
	}

	return false;
}

bool Parser::ELSE_KW(std::list<Token>::iterator &lex_pos) {

	if (lex_pos == tokens.end())
		return false;

	if (lex_pos->GetType() == "ELSE_KW") {
		++lex_pos;
		return true;
	}

	return false;
}

bool Parser::WHILE_KW(std::list<Token>::iterator &lex_pos) {

	if (lex_pos == tokens.end())
		return false;

	if (lex_pos->GetType() == "WHILE_KW") {
		++lex_pos;
		return true;
	}

	return false;
}

bool Parser::RETURN_KW(std::list<Token>::iterator& lex_pos) {

	if (lex_pos == tokens.end())
		return false;

	if (lex_pos->GetType() == "RETURN_KW") {
		++lex_pos;
		return true;
	}

	return false;
}

bool Parser::FUNCTION(std::list<Token>::iterator& lex_pos) {

	if (lex_pos == tokens.end())
		return false;

	if (lex_pos->GetType() == "FUNCTION") {

		if (FunctionsType.find(lex_pos->GetValue()) == FunctionsType.end())
			if (!FunctionsType.insert({ lex_pos->GetValue(), std::prev(lex_pos, 1)->GetValue() }).second) {

				std::cout << "Unable to store information about function" << std::endl;
				return false;
			}

		CurrentFunctionName = lex_pos->GetValue();
		++lex_pos;
		return true;
	}

	return false;
}

bool Parser::VAR(std::list<Token>::iterator &lex_pos) {

	if (lex_pos == tokens.end())
		return false;

	if (lex_pos->GetType() == "VAR") {		

		++lex_pos;
		return true;
	}
		
	return false;
}

bool Parser::DIGIT(std::list<Token>::iterator &lex_pos) {

	if (lex_pos == tokens.end())
		return false;

	if (lex_pos->GetType() == "DIGIT") {
		++lex_pos;
		return true;
	}

	return false;
}

bool Parser::ASSIGN_OP(std::list<Token>::iterator &lex_pos) {

	if (lex_pos == tokens.end())
		return false;

	if (lex_pos->GetType() == "ASSIGN_OP") {
		++lex_pos;
		return true;
	}

	return false;
}

bool Parser::OP(std::list<Token>::iterator &lex_pos) {

	if (lex_pos == tokens.end())
		return false;

	if (lex_pos->GetType() == "OP") {
		++lex_pos;
		return true;
	}

	return false;
}

bool Parser::LOG_OP(std::list<Token>::iterator &lex_pos) {

	if (lex_pos == tokens.end())
		return false;

	if (lex_pos->GetType() == "LOG_OP") {
		++lex_pos;
		return true;
	}

	return false;
}

bool Parser::COMP_OP(std::list<Token>::iterator &lex_pos) {

	if (lex_pos == tokens.end())
		return false;

	if (lex_pos->GetType() == "COMP_OP") {
		++lex_pos;
		return true;
	}

	return false;
}

bool Parser::END_ST(std::list<Token>::iterator &lex_pos) {

	if (lex_pos == tokens.end())
		return false;

	if (lex_pos->GetType() == "END_ST") {
		++lex_pos;
		return true;
	}

	return false;
}

bool Parser::L_BR(std::list<Token>::iterator &lex_pos) {

	if (lex_pos == tokens.end())
		return false;

	if (lex_pos->GetType() == "L_BR") {
		++lex_pos;
		return true;
	}

	return false;
}

bool Parser::R_BR(std::list<Token>::iterator &lex_pos) {

	if (lex_pos == tokens.end())
		return false;

	if (lex_pos->GetType() == "R_BR") {
		++lex_pos;
		return true;
	}

	return false;
}

bool Parser::L_FIG(std::list<Token>::iterator &lex_pos) {

	if (lex_pos == tokens.end())
		return false;

	if (lex_pos->GetType() == "L_FIG") {
		++lex_pos;
		return true;
	}

	return false;
}

bool Parser::R_FIG(std::list<Token>::iterator &lex_pos) {

	if (lex_pos == tokens.end())
		return false;

	if (lex_pos->GetType() == "R_FIG") {
		++lex_pos;
		return true;
	}

	return false;
}

bool Parser::COMMA(std::list<Token>::iterator &lex_pos) {

	if (lex_pos == tokens.end())
		return false;

	if (lex_pos->GetType() == "COMMA") {
		++lex_pos;
		return true;
	}

	return false;
}

std::unordered_map<std::string, int> Parser::GetFunctionsArgumentsNumber() { return FunctionsArgumentsNumber; }

bool Parser::WordCheck(std::list<Token>::iterator &lex_pos, std::string word) {

	if (lex_pos->GetValue() == word) {
		++lex_pos;
		return true;
	}

	return false;
}

Parser::~Parser() {}