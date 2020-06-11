#pragma once
#include <list>
#include <iterator>
#include <stack>
#include <unordered_map>
#include <utility>
#include <limits>

#include "Token.h"
#include "LinkedList.h"
class RPN
{
	
private:


	std::list<Token> RPN_List;
	std::unordered_map<int, std::list<Token>::iterator> positions;
	std::unordered_map<std::string, int> variables;
	std::unordered_map<std::string, LinkedList> list_vars;

	int Priority(Token tk);
	int Value(Token tk, bool &abort_flag);

public:

	RPN(std::list<Token> correct_list);
	bool Computation();
	~RPN();
};

