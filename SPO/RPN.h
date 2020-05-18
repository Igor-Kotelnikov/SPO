#pragma once
#include <list>
#include <iterator>
#include <stack>
#include <unordered_map>
#include <utility>
#include <limits>

#include "Token.h"
class RPN
{
	
private:


	std::list<Token> RPN_List;
	std::unordered_map<int, std::list<Token>::iterator> positions;
	std::unordered_map<std::string, int> variables;


	int Priority(Token tk);
	int Value(Token tk);

public:

	RPN(std::list<Token> correct_list, std::unordered_map<std::string, int> vars);
	bool Computation();
	~RPN();
};

