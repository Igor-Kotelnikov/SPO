#include "RPN.h"

RPN::RPN(std::list<Token> correct_list, std::unordered_map<std::string, int> vars) {

	this->variables = vars;

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

		else {
		
			if (cur.GetType() == "IF_KW" || cur.GetType() == "WHILE_KW") {

				keywords.push(cur);
				bodies.push(false);
				int_pos.push(RPN_List.size());
				RPN_List.push_back(Token("VAR", "p"));
				jump_pos.push(--RPN_List.end());

				operators.push(cur);
			}
			
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
		std::cout << tk << " ";

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

		else {

			//Math binary ops

			if (cur.GetValue() == "*") {
				
				p2 = Value(operands.top());
				operands.pop();

				p1 = Value(operands.top());
				operands.pop();

				if (p1 > std::numeric_limits<int>::max() / p2) {
					std::cout << "Computation error";
					return false;
				}

				operands.push(Token("DIGIT", std::to_string(p1*p2)));
			}

			if (cur.GetValue() == "/") {

				p2 = Value(operands.top());
				operands.pop();

				p1 = Value(operands.top());
				operands.pop();

				if (!p2) {
					std::cout << "Computation error";
					return false;
				}

				operands.push(Token("DIGIT", std::to_string(p1/p2)));
			}

			if (cur.GetValue() == "+" && cur.GetType() == "OP") {

				p2 = Value(operands.top());
				operands.pop();

				p1 = Value(operands.top());
				operands.pop();

				if ((p1 > 0 && p2 > 0 && p1 > std::numeric_limits<int>::max()-p2) || (p1 < 0 && p2 < 0 && p1 < std::numeric_limits<int>::min() - p2)) {
					std::cout << "Computation error";
					return false;
				}

				operands.push(Token("DIGIT", std::to_string(p1+p2)));
			}

			if (cur.GetValue() == "-" && cur.GetType() == "OP") {

				p2 = Value(operands.top());
				operands.pop();

				p1 = Value(operands.top());
				operands.pop();

				if ((p1 > 0 && p2 < 0 && p1 > std::numeric_limits<int>::max() + p2) || (p1 < 0 && p2 > 0 && p1 < std::numeric_limits<int>::min() + p2)) {
					std::cout << "Computation error";
					return false;
				}

				operands.push(Token("DIGIT", std::to_string(p1 - p2)));
			}

			//Math unary ops

			if (cur.GetValue() == "+" && cur.GetType() == "UNARY_OP") {

				p2 = Value(operands.top());
				operands.pop();			

				operands.push(Token("DIGIT", std::to_string(+p2)));
			}

			if (cur.GetValue() == "-") {

				p2 = Value(operands.top());
				operands.pop();

				operands.push(Token("DIGIT", std::to_string(-p2)));
			}

			//Compare ops

			if (cur.GetValue() == "<") {

				p2 = Value(operands.top());
				operands.pop();

				p1 = Value(operands.top());
				operands.pop();

				if (p1 < p2)
					operands.push(Token("DIGIT", "1"));
				else
					operands.push(Token("DIGIT", "0"));
			}

			if (cur.GetValue() == "<=") {

				p2 = Value(operands.top());
				operands.pop();

				p1 = Value(operands.top());
				operands.pop();

				if (p1 <= p2)
					operands.push(Token("DIGIT", "1"));
				else
					operands.push(Token("DIGIT", "0"));
			}

			if (cur.GetValue() == ">") {

				p2 = Value(operands.top());
				operands.pop();

				p1 = Value(operands.top());
				operands.pop();

				if (p1 > p2)
					operands.push(Token("DIGIT", "1"));
				else
					operands.push(Token("DIGIT", "0"));
			}

			if (cur.GetValue() == ">=") {

				p2 = Value(operands.top());
				operands.pop();

				p1 = Value(operands.top());
				operands.pop();

				if (p1 >= p2)
					operands.push(Token("DIGIT", "1"));
				else
					operands.push(Token("DIGIT", "0"));
			}

			if (cur.GetValue() == "==") {

				p2 = Value(operands.top());
				operands.pop();

				p1 = Value(operands.top());
				operands.pop();

				if (p1 == p2)
					operands.push(Token("DIGIT", "1"));
				else
					operands.push(Token("DIGIT", "0"));
			}

			if (cur.GetValue() == "!=") {

				p2 = Value(operands.top());
				operands.pop();

				p1 = Value(operands.top());
				operands.pop();

				if (p1 != p2)
					operands.push(Token("DIGIT", "1"));
				else
					operands.push(Token("DIGIT", "0"));
			}
			//Logic ops

			if (cur.GetValue() == "&&") {

				p2 = Value(operands.top());
				operands.pop();

				p1 = Value(operands.top());
				operands.pop();

				if (p1 && p2)
					operands.push(Token("DIGIT", "1"));
				else
					operands.push(Token("DIGIT", "0"));
			}

			if (cur.GetValue() == "||") {

				p2 = Value(operands.top());
				operands.pop();

				p1 = Value(operands.top());
				operands.pop();

				if (p1 || p2)
					operands.push(Token("DIGIT", "1"));
				else
					operands.push(Token("DIGIT", "0"));
			}

			//Unary logic op

			if (cur.GetValue() == "!") {

				p2 = Value(operands.top());
				operands.pop();


				operands.push(Token("DIGIT", std::to_string(!p2)));
			}

			//Assign op
			if (cur.GetValue() == "=") {

				p2 = Value(operands.top());
				operands.pop();

				variables.find(operands.top().GetValue())->second = p2;
				operands.pop();
			}

			++pos;
		}

	}

	return true;
}

int RPN::Value(Token tk) {

	if (tk.GetType() == "DIGIT")
		return std::stoi(tk.GetValue());

	else
		return variables.at(tk.GetValue());
}

RPN::~RPN() {}