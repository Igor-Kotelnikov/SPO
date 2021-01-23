#include "TriadOptimizer.h"

TriadOptimizer::TriadOptimizer(std::list<Token> reverse_list) {

	auto it = reverse_list.begin();
	std::stack<Token> operands;
	size_t count = 1;

	while (it != reverse_list.end()) {

		if (it->GetType() == "VAR" || it->GetType() == "DIGIT")
			operands.push(*it);

		else if (it->GetType() == "NO_OPT" || it->GetType() == "END_NO_OPT" || it->GetType() == "SCOPE" || it->GetType() == "ENTRY_POINT" || it->GetType() == "EXIT_POINT") {

			Triads.push_back(Triad(Token("DIGIT", "0"), Token("DIGIT", "0"), *it));
			count++;
		}

		else if (it->GetType() == "OP" || it->GetType() == "LOG_OP" || it->GetType() == "COMP_OP") {

			auto op2 = operands.top();
			operands.pop();

			auto op1 = operands.top();
			operands.pop();

			Triads.push_back(Triad(op1, op2, *it));
			operands.push(Token("TRIAD", std::to_string(count)));
			TriadPos.insert({count, --Triads.end()});
			count++;
		}

		else if (it->GetType() == "ASSIGN_OP") {

			auto op2 = operands.top();
			operands.pop();

			auto op1 = operands.top();
			operands.pop();

			Triads.push_back(Triad(op1, op2, *it));
			count++;
		}

		else if (it->GetType() == "UNARY_LOG_OP") {

			auto op1 = operands.top();
			operands.pop();

			Triads.push_back(Triad(op1, Token("DIGIT", "0"), *it));
			operands.push(Token("TRIAD", std::to_string(count)));
			TriadPos.insert({count, --Triads.end()});
			count++;
		}

		else if (it->GetType() == "UNARY_OP") {

			auto op1 = operands.top();
			operands.pop();

			Triads.push_back(Triad(Token("DIGIT", "0"), op1, Token("OP", it->GetValue())));
			operands.push(Token("TRIAD", std::to_string(count)));
			TriadPos.insert({ count, --Triads.end() });
			count++;
		}

		else if (it->GetValue() == "!F") {

			auto op2 = operands.top();
			operands.pop();

			auto op1 = operands.top();
			operands.pop();

			Triads.push_back(Triad(op1, op2, *it));
			count++;
		}

		else if (it->GetValue() == "!!") {

			auto op1 = operands.top();
			operands.pop();

			Triads.push_back(Triad(Token("DIGIT", "0"), op1, *it));
			count++;
		}

		++it;
	}
}

bool TriadOptimizer::Optimization() {

	bool check = true;
	size_t jmp_count = 0;
	size_t opt_count = 0;

	std::stack<std::list<Triad>::iterator> jump_triads;
	std::stack<size_t> jump_diffs;

	for (auto it = Triads.begin(); it != Triads.end() && check; ++it) {

		auto left = it->GetLeftOperand();
		auto right = it->GetRightOperand();
		auto op = it->GetOperation();

		if (left.GetType() == "TRIAD")
			Triads_Pointers.push_back(std::stoi(left.GetValue()));

		if (right.GetType() == "TRIAD")
			Triads_Pointers.push_back(std::stoi(right.GetValue()));

		if (op.GetType() == "NO_OPT") {

			jmp_count++;
			opt_count++;

			VarTable.clear();	
			continue;
		}

		if (op.GetType() == "END_NO_OPT") {

			jmp_count++;

			if (op.GetValue() == "WHILE_BODY") {

				jump_triads.top()->SetRightOperand(CalculateNewJumpPosition(jump_triads.top()->GetRightOperand(), jump_diffs.top()));
				jump_diffs.pop();
				jump_triads.pop();

				jump_triads.top()->SetRightOperand(CalculateNewJumpPosition(jump_triads.top()->GetRightOperand(), jmp_count));
				jump_triads.pop();
			}

			else if (op.GetValue() == "IF_BODY") {

				jump_triads.top()->SetRightOperand(CalculateNewJumpPosition(jump_triads.top()->GetRightOperand(), jmp_count));
				jump_triads.pop();
			}

			else if (op.GetValue() == "ELSE_BODY") {

				jump_triads.top()->SetRightOperand(CalculateNewJumpPosition(jump_triads.top()->GetRightOperand(), jmp_count));
				jump_triads.pop();

				jump_triads.top()->SetRightOperand(CalculateNewJumpPosition(jump_triads.top()->GetRightOperand(), jump_diffs.top()));
				jump_diffs.pop();
				jump_triads.pop();

			}

			opt_count--;

			continue;
		}

		if (op.GetType() == "FUNC") {

			jump_triads.push(it);

			if (op.GetValue() == "!F")
				jump_diffs.push(jmp_count);
		}
		

		if (left.GetType() == "VAR" &&
			!opt_count &&
			VarTable.find(left.GetValue()) != VarTable.end()) {

				it->SetLeftOperand(Token("DIGIT", VarTable.at(left.GetValue())));
				left = it->GetLeftOperand();
		}

		if (right.GetType() == "VAR" &&
			!opt_count &&
			VarTable.find(right.GetValue()) != VarTable.end()) {

				it->SetRightOperand(Token("DIGIT", VarTable.at(right.GetValue())));
				right = it->GetRightOperand();
		}

		if (left.GetType() == "TRIAD" &&
			TriadPos[std::stoi(left.GetValue())]->GetOperation().GetType() == "CONST") {

				it->SetLeftOperand(TriadPos[std::stoi(left.GetValue())]->GetLeftOperand());
				left = it->GetLeftOperand();
		}

		if (right.GetType() == "TRIAD" &&
			TriadPos[std::stoi(right.GetValue())]->GetOperation().GetType() == "CONST") {

				it->SetRightOperand(TriadPos[std::stoi(right.GetValue())]->GetLeftOperand());
				right = it->GetRightOperand();
		}

		if (left.GetType() == "DIGIT" &&
			right.GetType() == "DIGIT" &&
			op.GetType() != "FUNC" && op.GetType() != "NO_OPT" && op.GetType() != "END_NO_OPT" && op.GetType() != "SCOPE") {

				jmp_count += 2;
				
				it->SetLeftOperand(Compute(*it, check));
				it->SetOperation(Token("CONST", "C"));
				it->SetRightOperand(Token("DIGIT", "0"));
		}
			
		if (op.GetType() == "ASSIGN_OP" &&
			!opt_count &&
			right.GetType() == "DIGIT")

				VarTable[left.GetValue()] = right.GetValue();


	}

	return check;
}

std::list<Token> TriadOptimizer::ConvertToRPN() {

	std::vector<std::list<Token>> Triads_Lists;
	std::list<Token> tmp;

	Triads_Lists.push_back(std::list<Token>());

	for (auto it = Triads.begin(); it != Triads.end(); ++it) {

		if (it->GetOperation().GetType() == "SCOPE")
			tmp.push_back(it->GetOperation());

		else {

			if (it->GetOperation().GetValue() != "!!")
				tmp.push_back(it->GetLeftOperand());

			tmp.push_back(it->GetRightOperand());
			tmp.push_back(it->GetOperation());

		}

		Triads_Lists.push_back(tmp);
		tmp.clear();
	}

	std::list<Token> reverse_list;

	for (int i = 1; i < Triads_Lists.size(); i++) {

		auto op = Triads_Lists[i].back().GetType();

		if (op == "CONST" || op == "NO_OPT" || op == "END_NO_OPT")
			continue;

		else if (op == "SCOPE")
			reverse_list.push_back(Triads_Lists[i].back());

		else if (Triads_Pointers.front() != i) {

			auto it = Triads_Lists[i].begin();

			while (it != Triads_Lists[i].end())
				if (it->GetType() != "TRIAD") {

					reverse_list.push_back(*it);
					++it;
				}

				else {

					Triads_Pointers.pop_front();

					int pos = std::stoi(it->GetValue());
					auto tmp = it;
					tmp++;

					for (auto it2 = Triads_Lists[pos].begin(); it2 != Triads_Lists[pos].end(); ++it2) {

						Triads_Lists[i].insert(tmp, *it2);
					}
						
					it = Triads_Lists[i].erase(it);

				}					
		}

		else {

			auto it = Triads_Lists[i].begin();

			while (it != Triads_Lists[i].end())

				if (it->GetType() == "TRIAD") {

					auto tmp = it;
					int pos = std::stoi(it->GetValue());

					for (auto it2 = Triads_Lists[pos].begin(); it2 != Triads_Lists[pos].end(); ++it2) {

						++tmp;
						Triads_Lists[i].insert(tmp, *it2);
					}
				}

				else
					++it;
		}
	}

	return reverse_list;
}

Token TriadOptimizer::Compute(Triad tr, bool check) {

	int p1 = std::stoi(tr.GetLeftOperand().GetValue()),
		p2 = std::stoi(tr.GetRightOperand().GetValue());

	std::string oper = tr.GetOperation().GetValue();

	//Math ops
	if (oper == "+") {

		if ((p1 > 0 && p2 > 0 && p1 > std::numeric_limits<int>::max() - p2) || (p1 < 0 && p2 < 0 && p1 < std::numeric_limits<int>::min() - p2)) {
			std::cout << "Computation error";
			check = false;
		}

		return Token("DIGIT", std::to_string(p1 + p2));
	}

	else if (oper == "-") {

		if ((p1 > 0 && p2 < 0 && p1 > std::numeric_limits<int>::max() + p2) || (p1 < 0 && p2 > 0 && p1 < std::numeric_limits<int>::min() + p2)) {
			std::cout << "Computation error";
			check = false;
		}

		return Token("DIGIT", std::to_string(p1 - p2));
	}

	else if (oper == "*") {

		if (p1 > std::numeric_limits<int>::max() / p2) {
			std::cout << "Computation error";
			check = false;
		}

		return Token("DIGIT", std::to_string(p1 * p2));
	}

	else if (oper == "/") {

		if (!p2 || (p1 == std::numeric_limits<int>::min() && p2 == -1)) {
			std::cout << "Computation error";
			check = false;
		}

		return Token("DIGIT", std::to_string(p1 / p2));
	}

	//Logic ops
	else if (oper == "&&") {

		return Token("DIGIT", std::to_string(p1 && p2));
	}

	else if (oper == "||") {

		return Token("DIGIT", std::to_string(p1 || p2));
	}

	//Logic unary op
	else if (oper == "!") {

		return Token("DIGIT", std::to_string(!p1));
	}

	//Compare ops
	else if (oper == ">") {

		return Token("DIGIT", std::to_string(p1 > p2));
	}

	else if (oper == "<") {

		return Token("DIGIT", std::to_string(p1 < p2));
	}

	else if (oper == "==") {

		return Token("DIGIT", std::to_string(p1 == p2));
	}

	else if (oper == ">=") {

		return Token("DIGIT", std::to_string(p1 >= p2));
	}

	else if (oper == "<=") {

		return Token("DIGIT", std::to_string(p1 <= p2));
	}

	else if (oper == "!=") {

		return Token("DIGIT", std::to_string(p1 != p2));
	}
}

Token TriadOptimizer::CalculateNewJumpPosition(Token tk, int diff) {

	return Token("DIGIT", std::to_string(std::stoi(tk.GetValue()) - diff));
}

TriadOptimizer::~TriadOptimizer() {}