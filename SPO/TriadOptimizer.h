#pragma once
#include <list>
#include <unordered_map>
#include <string>
#include <stack>
#include <deque>

#include "Triad.h"

class TriadOptimizer
{

private:

	std::list<Triad> Triads;
	std::unordered_map<std::string, std::string> VarTable;

	std::unordered_map<int, std::list<Triad>::iterator> TriadPos;
	std::deque<size_t> Triads_Pointers;

public:

	TriadOptimizer(std::list<Token> reverse_list);
	bool Optimization();
	Token Compute(Triad tr, bool check);
	std::list<Token> ConvertToRPN();
	Token CalculateNewJumpPosition(Token tk, int diff);
	~TriadOptimizer();
};

