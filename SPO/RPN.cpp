#include "RPN.h"

RPN::RPN(std::list<Token> correct_list) {
	
	std::stack<Token> operators;	

	std::stack<std::list<Token>::iterator> jump_pos;
	std::stack<int> int_pos;
	std::stack<bool> bodies;
	std::stack<Token> keywords;

	auto token_it = correct_list.begin();
	
	while (token_it != correct_list.end()) {

		auto cur = *token_it;
		
		if (cur.GetType() == "VAR" || cur.GetType() == "DIGIT")
			RPN_List.push_back(cur);

		else if (cur.GetType() == "L_BR")
			operators.push(cur);

		else if (cur.GetType() == "R_BR") {

			while (operators.top().GetValue() != "(") {

				RPN_List.push_back(operators.top());
				operators.pop();
			}

			operators.pop();

			if (operators.top().GetType() == "IF_KW" || operators.top().GetType() == "WHILE_KW") {							

				RPN_List.push_back(Token("FUNC", "!F"));
				operators.pop();
			}

			if (operators.top().GetType() == "NO_ARG_LIST_FUNC" || operators.top().GetType() == "ONE_ARG_LIST_FUNC" || operators.top().GetType() == "TWO_ARG_LIST_FUNC") {

				RPN_List.push_back(operators.top());
				operators.pop();
			}
		}

		else if (cur.GetType() == "COMMA") {

			while (operators.top().GetValue() != "(") {

				RPN_List.push_back(operators.top());
				operators.pop();
			}
		}

		else if (cur.GetType() == "R_FIG") {

			if (keywords.top().GetType() == "IF_KW") {

				if (++token_it != correct_list.end() && (*token_it).GetType() == "ELSE_KW") {

					*jump_pos.top() = Token("DIGIT", std::to_string(RPN_List.size() + 2));

					jump_pos.pop();
					int_pos.pop();
					bodies.pop();
					keywords.pop();

					RPN_List.push_back(Token("VAR", "p"));

					jump_pos.push(--RPN_List.end());
					bodies.push(false);
					keywords.push(Token("ELSE_KW", "else"));

					RPN_List.push_back(Token("FUNC", "!!"));
				}				

				else {

					--token_it;

					*jump_pos.top() = Token("DIGIT", std::to_string(RPN_List.size()));

					jump_pos.pop();
					int_pos.pop();
					bodies.pop();
					keywords.pop();
				}

			}

			else if (keywords.top().GetType() == "WHILE_KW") {

				*jump_pos.top() = Token("DIGIT", std::to_string(RPN_List.size()+2));
				
				RPN_List.push_back(Token("DIGIT", std::to_string(int_pos.top())));
				RPN_List.push_back(Token("FUNC", "!!"));

				jump_pos.pop();
				int_pos.pop();
				bodies.pop();
				keywords.pop();
			}

			else if (keywords.top().GetType() == "ELSE_KW") {

				*jump_pos.top() = Token("DIGIT", std::to_string(RPN_List.size()));

				jump_pos.pop();
				int_pos.pop();
				bodies.pop();
				keywords.pop();

			}
		}

		else if (cur.GetType() == "L_FIG") {

			bodies.top() = true;
		}

		else if (cur.GetType() == "END_ST") {

			while (!operators.empty()) {
				RPN_List.push_back(operators.top());
				operators.pop();
			}

			while(!bodies.empty() && !bodies.top()) {
				
				//Same as in R_FIG, might be optimized
				if (keywords.top().GetType() == "IF_KW") {

					if (++token_it != correct_list.end() && (*token_it).GetType() == "ELSE_KW") {

						*jump_pos.top() = Token("DIGIT", std::to_string(RPN_List.size() + 2));

						jump_pos.pop();
						int_pos.pop();
						bodies.pop();
						keywords.pop();

						RPN_List.push_back(Token("VAR", "p"));

						int_pos.push(RPN_List.size());
						jump_pos.push(--RPN_List.end());
						bodies.push(false);						
						keywords.push(Token("ELSE_KW", "else"));						

						RPN_List.push_back(Token("FUNC", "!!"));
					}

					else {

						--token_it;
						
						*jump_pos.top() = Token("DIGIT", std::to_string(RPN_List.size()));

						jump_pos.pop();
						int_pos.pop();
						bodies.pop();
						keywords.pop();
					}

				}

				else if (keywords.top().GetType() == "WHILE_KW") {
					
					*jump_pos.top() = Token("DIGIT", std::to_string(RPN_List.size() + 2));

					RPN_List.push_back(Token("DIGIT", std::to_string(int_pos.top())));
					RPN_List.push_back(Token("FUNC", "!!"));

					jump_pos.pop();
					int_pos.pop();
					bodies.pop();
					keywords.pop();
				}

				else if (keywords.top().GetType() == "ELSE_KW") {

					*jump_pos.top() = Token("DIGIT", std::to_string(RPN_List.size()));

					jump_pos.pop();
					int_pos.pop();
					bodies.pop();
					keywords.pop();
				}
			}
		}		

		else if (cur.GetType() != "POINT") {
		
		if (cur.GetType() == "IF_KW" || cur.GetType() == "WHILE_KW") {

			keywords.push(cur);
			bodies.push(false);
			int_pos.push(RPN_List.size());
			RPN_List.push_back(Token("VAR", "p"));
			jump_pos.push(--RPN_List.end());

			operators.push(cur);
		}

		else if (cur.GetType() == "NO_ARG_LIST_FUNC" || cur.GetType() == "ONE_ARG_LIST_FUNC" || cur.GetType() == "TWO_ARG_LIST_FUNC")
			operators.push(cur);

			else {

				while (!operators.empty() && Priority(cur) <= Priority(operators.top())) {

					RPN_List.push_back(operators.top());
					operators.pop();
				}

				operators.push(cur);
			}
		}

		++token_it;

	}

	std::cout << std::endl << std::endl;
	for (auto & tk : RPN_List)
		std::cout << tk << std::endl;

}

int RPN::Priority(Token tk) {

	std::string TokenValue = tk.GetValue();

	if (TokenValue == "*" || TokenValue == "/")
		return 6;

	if (TokenValue == "+" || TokenValue == "-")
		return 5;

	if (TokenValue == "<=" || TokenValue == "<" || TokenValue == ">=" || TokenValue == ">")
		return 4;

	if (TokenValue == "!=" || TokenValue == "==")
		return 3;

	if (TokenValue == "&&")
		return 2;

	if (TokenValue == "||")
		return 1;

	if (TokenValue == "=")
		return 0;


	return -1;
}

bool RPN::Computation() {

	std::stack<Token> operands;
	
	int p1, p2;
	bool correct_access = true;

	auto pos = RPN_List.begin();

	while ( pos != RPN_List.end()) {

		auto cur = *pos;

		if (cur.GetType() == "VAR" || cur.GetType() == "DIGIT") {

			operands.push(cur);
			++pos;
		}

		else if (cur.GetType() == "FUNC") {

			if (cur.GetValue() == "!!") {

				if (positions.find(std::stoi(operands.top().GetValue())) != positions.end())
					pos = positions.at(std::stoi(operands.top().GetValue()));

				else {

					pos = RPN_List.begin();
					std::advance(pos, std::stoi(operands.top().GetValue()));
					positions.insert({ std::stoi(operands.top().GetValue()), pos });
				}

				operands.pop();
			}

			if (cur.GetValue() == "!F") {
				if (!std::stoi(operands.top().GetValue())) {

					operands.pop();

					if (positions.find(std::stoi(operands.top().GetValue())) != positions.end())
						pos = positions.at(std::stoi(operands.top().GetValue()));

					else {

						pos = RPN_List.begin();
						std::advance(pos, std::stoi(operands.top().GetValue()));
						positions.insert({ std::stoi(operands.top().GetValue()), pos });
					}

					operands.pop();
				}

				else {

					operands.pop();
					operands.pop();
					++pos;
				}
			}
				
		}

		else if (cur.GetType() == "NO_ARG_LIST_FUNC"){

			if (cur.GetValue() == "create_list") {

				if (list_vars.find(operands.top().GetValue()) == list_vars.end())
					if (!list_vars.insert({ operands.top().GetValue(), LinkedList() }).second || variables.find(operands.top().GetValue()) != variables.end()) {

						std::cout << "List variable initialization error";
						return false;
					}
			}

			if (cur.GetValue() == "pop_front") {

				if (list_vars.find(operands.top().GetValue()) == list_vars.end()) {

					std::cout << "List variable access error";
					return false;
				}

				if (!list_vars.at(operands.top().GetValue()).getSize()) {

					std::cout << "List function access error";
					return false;
				}

				list_vars.at(operands.top().GetValue()).pop_front();	
				operands.pop();
			}

			if (cur.GetValue() == "pop_back") {

				if (list_vars.find(operands.top().GetValue()) == list_vars.end()) {

					std::cout << "List variable access error";
					return false;
				}

				if (!list_vars.at(operands.top().GetValue()).getSize()) {

					std::cout << "List function access error";
					return false;
				}

				list_vars.at(operands.top().GetValue()).pop_back();
				operands.pop();
			}

			if (cur.GetValue() == "get_size") {

				if (list_vars.find(operands.top().GetValue()) == list_vars.end()) {

					std::cout << "List variable access error";
					return false;
				}

				p1 = list_vars.at(operands.top().GetValue()).getSize();
				operands.pop();
				operands.push(Token("DIGIT", std::to_string(p1)));
			}

			++pos;
		}

		else if (cur.GetType() == "ONE_ARG_LIST_FUNC") {

			if (cur.GetValue() == "push_back") {

				p1 = Value(operands.top(), correct_access);
				operands.pop();

				if (!correct_access) {
					std::cout << "variable access error";
					return false;
				}

				if (list_vars.find(operands.top().GetValue()) == list_vars.end()) {

					std::cout << "List variable access error";
					return false;
				}

				list_vars.at(operands.top().GetValue()).push_back(p1);
				operands.pop();
			}

			if (cur.GetValue() == "push_front") {

				p1 = Value(operands.top(), correct_access);
				operands.pop();

				if (!correct_access) {
					std::cout << "variable access error";
					return false;
				}

				if (list_vars.find(operands.top().GetValue()) == list_vars.end()) {

					std::cout << "List variable access error";
					return false;
				}

				list_vars.at(operands.top().GetValue()).push_front(p1);
				operands.pop();
			}

			if (cur.GetValue() == "get") {

				p1 = Value(operands.top(), correct_access);
				operands.pop();

				if (!correct_access) {
					std::cout << "variable access error";
					return false;
				}

				if (list_vars.find(operands.top().GetValue()) == list_vars.end()) {

					std::cout << "List variable access error";
					return false;
				}


				if (p1 < 0 || p1 >= list_vars.at(operands.top().GetValue()).getSize()) {

					std::cout << "List function access error";
					return false;
				}

				p1 = list_vars.at(operands.top().GetValue()).get(p1);
				operands.pop();
				operands.push(Token("DIGIT", std::to_string(p1)));
			}

			if (cur.GetValue() == "remove") {

				p1 = Value(operands.top(), correct_access);
				operands.pop();

				if (!correct_access) {
					std::cout << "variable access error";
					return false;
				}
				
				if (list_vars.find(operands.top().GetValue()) == list_vars.end()) {

					std::cout << "List variable access error";
					return false;
				}


				if (p1 < 0 || p1 >= list_vars.at(operands.top().GetValue()).getSize()) {

					std::cout << "List function access error";
					return false;
				}

				list_vars.at(operands.top().GetValue()).remove(p1);
				operands.pop();

			}

			++pos;
		}

		else if (cur.GetType() == "TWO_ARG_LIST_FUNC") {

			if (cur.GetValue() == "insert") {

				p1 = Value(operands.top(), correct_access);
				operands.pop();

				p2 = Value(operands.top(), correct_access);
				operands.pop();

				if (!correct_access) {
					std::cout << "variable access error";
					return false;
				}

				if (list_vars.find(operands.top().GetValue()) == list_vars.end()) {

					std::cout << "List variable access error";
					return false;
				}

				if (p2 < 0 || p2 >= list_vars.at(operands.top().GetValue()).getSize()) {

					std::cout << "List function access error";
					return false;
				}

				list_vars.at(operands.top().GetValue()).insert(p2, p1);
				operands.pop();
			}

			++pos;
		}

		else {

			//Math binary ops

			if (cur.GetValue() == "*") {
				
				p2 = Value(operands.top(), correct_access);
				operands.pop();			

				p1 = Value(operands.top(), correct_access);
				operands.pop();

				if (!correct_access) {
					std::cout << "variable access error";
					return false;
				}

				if (p1 > std::numeric_limits<int>::max() / p2) {
					std::cout << "Computation error";
					return false;
				}

				operands.push(Token("DIGIT", std::to_string(p1*p2)));
			}

			if (cur.GetValue() == "/") {

				p2 = Value(operands.top(), correct_access);
				operands.pop();

				p1 = Value(operands.top(), correct_access);
				operands.pop();

				if (!correct_access) {
					std::cout << "variable access error";
					return false;
				}

				if (!p2) {
					std::cout << "Computation error";
					return false;
				}

				operands.push(Token("DIGIT", std::to_string(p1/p2)));
			}

			if (cur.GetValue() == "+" && cur.GetType() == "OP") {

				p2 = Value(operands.top(), correct_access);
				operands.pop();

				p1 = Value(operands.top(), correct_access);
				operands.pop();

				if (!correct_access) {
					std::cout << "variable access error";
					return false;
				}

				if ((p1 > 0 && p2 > 0 && p1 > std::numeric_limits<int>::max()-p2) || (p1 < 0 && p2 < 0 && p1 < std::numeric_limits<int>::min() - p2)) {
					std::cout << "Computation error";
					return false;
				}

				operands.push(Token("DIGIT", std::to_string(p1+p2)));
			}

			if (cur.GetValue() == "-" && cur.GetType() == "OP") {

				p2 = Value(operands.top(), correct_access);
				operands.pop();

				p1 = Value(operands.top(), correct_access);
				operands.pop();

				if (!correct_access) {
					std::cout << "variable access error";
					return false;
				}

				if ((p1 > 0 && p2 < 0 && p1 > std::numeric_limits<int>::max() + p2) || (p1 < 0 && p2 > 0 && p1 < std::numeric_limits<int>::min() + p2)) {
					std::cout << "Computation error";
					return false;
				}

				operands.push(Token("DIGIT", std::to_string(p1 - p2)));
			}

			//Math unary ops

			if (cur.GetValue() == "+" && cur.GetType() == "UNARY_OP") {

				p2 = Value(operands.top(), correct_access);
				operands.pop();			

				if (!correct_access) {
					std::cout << "variable access error";
					return false;
				}

				operands.push(Token("DIGIT", std::to_string(+p2)));
			}

			if (cur.GetValue() == "-" && cur.GetType() == "UNARY_OP") {

				p2 = Value(operands.top(), correct_access);
				operands.pop();

				if (!correct_access) {
					std::cout << "variable access error";
					return false;
				}

				operands.push(Token("DIGIT", std::to_string(-p2)));
			}

			//Compare ops

			if (cur.GetValue() == "<") {

				p2 = Value(operands.top(), correct_access);
				operands.pop();

				p1 = Value(operands.top(), correct_access);
				operands.pop();

				if (!correct_access) {
					std::cout << "variable access error";
					return false;
				}

				if (p1 < p2)
					operands.push(Token("DIGIT", "1"));
				else
					operands.push(Token("DIGIT", "0"));
			}

			if (cur.GetValue() == "<=") {

				p2 = Value(operands.top(), correct_access);
				operands.pop();

				p1 = Value(operands.top(), correct_access);
				operands.pop();

				if (!correct_access) {
					std::cout << "variable access error";
					return false;
				}

				if (p1 <= p2)
					operands.push(Token("DIGIT", "1"));
				else
					operands.push(Token("DIGIT", "0"));
			}

			if (cur.GetValue() == ">") {

				p2 = Value(operands.top(), correct_access);
				operands.pop();

				p1 = Value(operands.top(), correct_access);
				operands.pop();

				if (!correct_access) {
					std::cout << "variable access error";
					return false;
				}

				if (p1 > p2)
					operands.push(Token("DIGIT", "1"));
				else
					operands.push(Token("DIGIT", "0"));
			}

			if (cur.GetValue() == ">=") {

				p2 = Value(operands.top(), correct_access);
				operands.pop();

				p1 = Value(operands.top(), correct_access);
				operands.pop();

				if (!correct_access) {
					std::cout << "variable access error";
					return false;
				}

				if (p1 >= p2)
					operands.push(Token("DIGIT", "1"));
				else
					operands.push(Token("DIGIT", "0"));
			}

			if (cur.GetValue() == "==") {

				p2 = Value(operands.top(), correct_access);
				operands.pop();

				p1 = Value(operands.top(), correct_access);
				operands.pop();

				if (!correct_access) {
					std::cout << "variable access error";
					return false;
				}

				if (p1 == p2)
					operands.push(Token("DIGIT", "1"));
				else
					operands.push(Token("DIGIT", "0"));
			}

			if (cur.GetValue() == "!=") {

				p2 = Value(operands.top(), correct_access);
				operands.pop();

				p1 = Value(operands.top(), correct_access);
				operands.pop();

				if (!correct_access) {
					std::cout << "variable access error";
					return false;
				}

				if (p1 != p2)
					operands.push(Token("DIGIT", "1"));
				else
					operands.push(Token("DIGIT", "0"));
			}
			//Logic ops

			if (cur.GetValue() == "&&") {

				p2 = Value(operands.top(), correct_access);
				operands.pop();

				p1 = Value(operands.top(), correct_access);
				operands.pop();

				if (!correct_access) {
					std::cout << "variable access error";
					return false;
				}

				if (p1 && p2)
					operands.push(Token("DIGIT", "1"));
				else
					operands.push(Token("DIGIT", "0"));
			}

			if (cur.GetValue() == "||") {

				p2 = Value(operands.top(), correct_access);
				operands.pop();

				p1 = Value(operands.top(), correct_access);
				operands.pop();

				if (!correct_access) {
					std::cout << "variable access error";
					return false;
				}

				if (p1 || p2)
					operands.push(Token("DIGIT", "1"));
				else
					operands.push(Token("DIGIT", "0"));
			}

			//Unary logic op

			if (cur.GetValue() == "!") {

				p2 = Value(operands.top(), correct_access);
				operands.pop();

				if (!correct_access) {
					std::cout << "variable access error";
					return false;
				}

				operands.push(Token("DIGIT", std::to_string(!p2)));
			}

			//Assign op
			if (cur.GetValue() == "=") {

				p2 = Value(operands.top(), correct_access);
				operands.pop();

				if (!correct_access) {
					std::cout << "variable access error";
					return false;
				}

				if (variables.find(operands.top().GetValue()) == variables.end()) {
					if (!variables.insert({ operands.top().GetValue(), p2 }).second || list_vars.find(operands.top().GetValue()) != list_vars.end()) {

						std::cout << "Variable initialization error" << std::endl;
						return false;
					}
				}

				else
					variables.find(operands.top().GetValue())->second = p2;


				operands.pop();
			}

			++pos;
		}

	}

	std::cout << std::endl;

	for (auto &var : variables)
		std::cout << var.first << " = " << var.second << std::endl;

	for (auto &list_var : list_vars) {
		std::cout << "List " << list_var.first << " contains:" << std::endl;
		for (int i = 0; i < list_var.second.getSize(); i++) {
			std::cout << list_var.second.get(i) << std::endl;
		}
	}

	return true;
}

int RPN::Value(Token tk, bool &abort_flag) {

	if (tk.GetType() == "DIGIT")
		return std::stoi(tk.GetValue());

	else
		if (variables.find(tk.GetValue()) != variables.end())
			return variables.at(tk.GetValue());
		else {
			abort_flag = false;
			return 0;
		}

}

RPN::~RPN() {}