#pragma once
#include "Token.h"
#include <list>
#include <iterator>
#include <unordered_map>

class Parser
{

private:

	std::list<Token> tokens;
	std::unordered_map<std::string, int> variables;

	bool lang(std::list<Token>::iterator &lex_pos);

	bool assign_expr(std::list<Token>::iterator &lex_pos);
	bool if_expr(std::list<Token>::iterator &lex_pos);
	bool else_expr(std::list<Token>::iterator &lex_pos);
	bool while_expr(std::list<Token>::iterator &lex_pos);

	bool value_expr(std::list<Token>::iterator &lex_pos);
	bool logic_expr(std::list<Token>::iterator &lex_pos);
	bool comp_expr(std::list<Token>::iterator &lex_pos);

	bool value(std::list<Token>::iterator &lex_pos);

	bool IF_KW(std::list<Token>::iterator &lex_pos);
	bool ELSE_KW(std::list<Token>::iterator &lex_pos);
	bool WHILE_KW(std::list<Token>::iterator &lex_pos);	

	bool VAR(std::list<Token>::iterator &lex_pos);
	bool DIGIT(std::list<Token>::iterator &lex_pos);

	bool ASSIGN_OP(std::list<Token>::iterator &lex_pos);
	bool OP(std::list<Token>::iterator &lex_pos);
	bool LOG_OP(std::list<Token>::iterator &lex_pos);
	bool COMP_OP(std::list<Token>::iterator &lex_pos);

	bool END_ST(std::list<Token>::iterator &lex_pos);

	bool L_BR(std::list<Token>::iterator &lex_pos);
	bool R_BR(std::list<Token>::iterator &lex_pos);

	bool L_FIG(std::list<Token>::iterator &lex_pos);
	bool R_FIG(std::list<Token>::iterator &lex_pos);

public:

	Parser(std::list<Token> token_list);
	bool CheckSyntax();
	std::unordered_map<std::string, int> GetVariableHash();
	~Parser();

};
