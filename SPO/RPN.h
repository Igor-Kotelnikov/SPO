#pragma once
#include <list>
#include <iterator>
#include <stack>
#include <unordered_map>
#include <utility>
#include <limits>

#include "Token.h"
#include "TriadOptimizer.h"

class RPN
{
	
private:

	size_t scope = 0;

	std::list<Token> RPN_List;
	std::unordered_map<int, std::list<Token>::iterator> positions;
	std::unordered_map<int, std::unordered_map<std::string, int>> variables;
	std::unordered_map<std::string, std::list<Token>::iterator> functions_entries;

	int Priority(Token tk);
	int Value(Token tk, int allowed_min_scope, bool &abort_flag);
	std::unordered_map<std::string, int>::iterator FindVariable(std::string variable, int allowed_min_scope, bool &not_init_flag);
	std::list<Token>::iterator FindPosition(Token tk);
	bool InitializeScope();
	void RemoveScope();

public:

	RPN(std::list<Token> correct_list, std::unordered_map<std::string, int> FunctionArgumentsNumber);
	bool Optimization();
	void PrintReverseTokens();
	void PrintResults();
	bool Computation();	
	~RPN();
};

