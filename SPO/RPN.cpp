#include "RPN.h"

RPN::RPN(std::list<Token> correct_list, std::unordered_map<std::string, int> FunctionArgumentsNumber) {
	
	std::stack<Token> operators;	

	std::stack<std::list<Token>::iterator> jump_pos;
	std::stack<int> int_pos;
	std::stack<bool> bodies;
	std::stack<Token> keywords;

	std::stack<bool> func_definition;
	bool is_definition;
	std::string current_function;

	auto token_it = correct_list.begin();
	
	while (token_it != correct_list.end()) {

		auto cur = *token_it;
		
		if (cur.GetType() == "VAR" || cur.GetType() == "DIGIT")
			RPN_List.push_back(cur);

		else if (cur.GetType() == "DEFINE_KW")
			is_definition = true;
		
		else if (cur.GetType() == "MAIN_KW") {

			current_function = "main";
			RPN_List.push_back(Token("ENTRY_POINT", current_function));

			keywords.push(cur);
			bodies.push(false);

			operators.push(cur);
		}

		else if (cur.GetType() == "L_BR")
			operators.push(cur);

		else if (cur.GetType() == "R_BR") {

			while (operators.top().GetValue() != "(") {

				RPN_List.push_back(operators.top());
				operators.pop();
			}

			operators.pop();

			if (!func_definition.empty() && FunctionArgumentsNumber[operators.top().GetValue()]) {
				if (func_definition.top())
					RPN_List.push_back(Token("REVERSE_ASSIGN_OP", "="));
				else
					RPN_List.push_back(Token("VALUE_READ_OP", "REFERENCE"));
			}

			if (operators.top().GetType() == "IF_KW" || operators.top().GetType() == "WHILE_KW") {

				RPN_List.push_back(Token("VAR", "p"));
				jump_pos.push(--RPN_List.end());

				RPN_List.push_back(Token("FUNC", "!F"));
				operators.pop();
			}

			else if (operators.top().GetType() == "FUNCTION") {

				if (!func_definition.top()) 
					RPN_List.push_back(operators.top());				

				func_definition.pop();
				operators.pop();
			}

			else if (operators.top().GetType() == "MAIN_KW") {

				operators.pop();
			}
		}

		else if (cur.GetType() == "FUNCTION") {
			
			current_function = cur.GetValue();

			if (is_definition) {

				RPN_List.push_back(Token("ENTRY_POINT", current_function));

				keywords.push(cur);
				bodies.push(false);
			}

			func_definition.push(is_definition);
			is_definition = false;

			operators.push(cur);			
		}

		else if (cur.GetType() == "COMMA") {

			while (operators.top().GetValue() != "(") {

				RPN_List.push_back(operators.top());
				operators.pop();
			}

			if (func_definition.top())
				RPN_List.push_back(Token("REVERSE_ASSIGN_OP", "="));

			else
				RPN_List.push_back(Token("VALUE_READ_OP", "REFERENCE"));
		}

		else if (cur.GetType() == "R_FIG") {

			if (keywords.top().GetType() == "IF_KW") {

				if (++token_it != correct_list.end() && (*token_it).GetType() == "ELSE_KW")
					*jump_pos.top() = Token("DIGIT", std::to_string(RPN_List.size() + 2));

				else {

					RPN_List.push_back(Token("END_NO_OPT", "IF_BODY"));
					RPN_List.push_back(Token("SCOPE", "DEC"));
					*jump_pos.top() = Token("DIGIT", std::to_string(RPN_List.size()));
				}

				--token_it;

				jump_pos.pop();
				int_pos.pop();
				bodies.pop();
				keywords.pop();

			}

			else if (keywords.top().GetType() == "WHILE_KW") {
												
				RPN_List.push_back(Token("DIGIT", std::to_string(int_pos.top())));
				RPN_List.push_back(Token("FUNC", "!!"));

				RPN_List.push_back(Token("END_NO_OPT", "WHILE_BODY"));

				*jump_pos.top() = Token("DIGIT", std::to_string(RPN_List.size()));
				
				jump_pos.pop();
				int_pos.pop();
				bodies.pop();
				keywords.pop();
			}

			else if (keywords.top().GetType() == "ELSE_KW") {

				RPN_List.push_back(Token("END_NO_OPT", "ELSE_BODY"));
				RPN_List.push_back(Token("SCOPE", "DEC"));

				*jump_pos.top() = Token("DIGIT", std::to_string(RPN_List.size()));				

				jump_pos.pop();
				int_pos.pop();
				bodies.pop();
				keywords.pop();

			}

			else if (keywords.top().GetType() == "FUNCTION") {

				RPN_List.push_back(Token("EXIT_POINT", keywords.top().GetValue()));

				keywords.pop();
				bodies.pop();
			}

			else if (keywords.top().GetType() == "MAIN_KW") {

				RPN_List.push_back(Token("EXIT_POINT", keywords.top().GetValue()));

				keywords.pop();
				bodies.pop();
			}
		}

		else if (cur.GetType() == "L_FIG") 
			bodies.top() = true;		

		else if (cur.GetType() == "END_ST") {

			while (!operators.empty()) {
				RPN_List.push_back(operators.top());
				operators.pop();
			}

			while(!bodies.empty() && !bodies.top()) {
				
				//Same as in R_FIG, might be optimized
				if (keywords.top().GetType() == "IF_KW") {

					if (++token_it != correct_list.end() && (*token_it).GetType() == "ELSE_KW") 
						*jump_pos.top() = Token("DIGIT", std::to_string(RPN_List.size() + 2));								

					else {									
						
						RPN_List.push_back(Token("END_NO_OPT", "IF_BODY"));
						RPN_List.push_back(Token("SCOPE", "DEC"));

						*jump_pos.top() = Token("DIGIT", std::to_string(RPN_List.size()));						
					}

					--token_it;

					jump_pos.pop();
					int_pos.pop();
					bodies.pop();
					keywords.pop();

				}

				else if (keywords.top().GetType() == "WHILE_KW") {
										
					RPN_List.push_back(Token("DIGIT", std::to_string(int_pos.top())));
					RPN_List.push_back(Token("FUNC", "!!"));

					RPN_List.push_back(Token("END_NO_OPT", "WHILE_BODY"));

					*jump_pos.top() = Token("DIGIT", std::to_string(RPN_List.size()));

					jump_pos.pop();
					int_pos.pop();
					bodies.pop();
					keywords.pop();
				}

				else if (keywords.top().GetType() == "ELSE_KW") {

					RPN_List.push_back(Token("END_NO_OPT", "ELSE_BODY"));
					RPN_List.push_back(Token("SCOPE", "DEC"));

					*jump_pos.top() = Token("DIGIT", std::to_string(RPN_List.size()));
					
					jump_pos.pop();
					int_pos.pop();
					bodies.pop();
					keywords.pop();
				}
			}
		}		

		else if (cur.GetType() != "DATA_TYPE") {
		
		if (cur.GetType() == "IF_KW" || cur.GetType() == "WHILE_KW") {

			RPN_List.push_back(Token("NO_OPT", cur.GetType()));

			keywords.push(cur);
			bodies.push(false);
			int_pos.push(RPN_List.size());
			operators.push(cur);
		}

		else if (cur.GetType() == "ELSE_KW") {

			RPN_List.push_back(Token("VAR", "p"));

			int_pos.push(RPN_List.size());
			jump_pos.push(--RPN_List.end());
			bodies.push(false);
			keywords.push(Token("ELSE_KW", "else"));

			RPN_List.push_back(Token("FUNC", "!!"));
			RPN_List.push_back(Token("SCOPE", "INC"));
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
	
}

void RPN::PrintReverseTokens() {

	for (auto& tk : RPN_List)
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

bool RPN::Optimization() {

	/*TriadOptimizer tro(RPN_List);
	if (!tro.Optimization())
		return false;

	RPN_List = tro.ConvertToRPN();*/

	return true;
}

bool RPN::Computation() {

	std::stack<Token> operands;
	
	int p1, p2;
	bool correct_access = true,
		 not_initialized = false;

	if (!variables.insert({ 0, std::unordered_map<std::string, int>() }).second) {

		std::cout << "Unable to initialize global scope" << std::endl;
		return false;
	}

	std::stack<std::list<Token>::iterator> functions_jumps;
	std::stack<int> scope_jumps;

	auto pos = FindPosition(Token("ENTRY_POINT", "main"));	
	functions_jumps.push(FindPosition(Token("EXIT_POINT", "main")));

	while ( pos != RPN_List.end()) {

		auto cur = *pos;

		if (cur.GetType() == "FUNCTION") {

			functions_jumps.push(pos);

			if (functions_entries.find(cur.GetValue()) == functions_entries.end()) {

				pos = FindPosition(Token("ENTRY_POINT", cur.GetValue()));
				functions_entries[cur.GetValue()] = pos;
			}

			else
				pos = functions_entries[cur.GetValue()];		
		}

		else if (cur.GetType() == "ENTRY_POINT") {

			if (!InitializeScope())
				return false;

			scope_jumps.push(scope);
			++pos;
		}

		else if (cur.GetType() == "EXIT_POINT") {

			if (cur.GetValue() == "main")
				PrintResults();

			while (scope >= scope_jumps.top())
				RemoveScope();

			pos = functions_jumps.top();
			functions_jumps.pop();

			scope_jumps.pop();
			++pos;
		}

		else if (cur.GetType() == "RETURN_KW") {
						
			p2 = Value(operands.top(), scope_jumps.top(), correct_access);

			if (!correct_access) {

				std::cout << "Variable access error" << std::endl;
				return false;
			}

			operands.pop();
			operands.push(Token("DIGIT", std::to_string(p2)));

			while (pos->GetType() != "EXIT_POINT") 
				++pos;		
		}

		else if (cur.GetType() == "NO_OPT" || cur.GetType() == "END_NO_OPT") {

			++pos;
			continue;
		}

		else if (cur.GetType() == "VAR" || cur.GetType() == "DIGIT") {

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

				RemoveScope();
				operands.pop();
			}

			if (cur.GetValue() == "!F") {

				int jump = std::stoi(operands.top().GetValue());
				operands.pop();

				p1 = Value(operands.top(), scope_jumps.top(), correct_access);

				if (!correct_access) {

					std::cout << "Variable access error" << std::endl;
					return false;
				}

				operands.pop();

				if (!p1) {

					if (positions.find(jump) != positions.end())
						pos = positions.at(jump);

					else {

						pos = RPN_List.begin();
						std::advance(pos, jump);
						positions.insert({ jump, pos });
					}
				}

				else {

					if (!InitializeScope())
						return false;

					++pos;
				}
				
			}
				
		}		

		else if (cur.GetType() == "SCOPE") {

			if (cur.GetValue() == "INC") 						
				if (!InitializeScope())
					return false;			

			else 
				RemoveScope();
			
			++pos;
		}

		else {

			//Math binary ops

			if (cur.GetValue() == "*") {
				
				p2 = Value(operands.top(), scope_jumps.top(), correct_access);
				operands.pop();			

				p1 = Value(operands.top(), scope_jumps.top(), correct_access);
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

				p2 = Value(operands.top(), scope_jumps.top(), correct_access);
				operands.pop();

				p1 = Value(operands.top(), scope_jumps.top(), correct_access);
				operands.pop();

				if (!correct_access) {
					std::cout << "variable access error";
					return false;
				}

				if (!p2 || (p1 == std::numeric_limits<int>::min() && p2 == -1)) {
					std::cout << "Computation error";
					return false;
				}

				operands.push(Token("DIGIT", std::to_string(p1/p2)));
			}

			if (cur.GetValue() == "+" && cur.GetType() == "OP") {

				p2 = Value(operands.top(), scope_jumps.top(), correct_access);
				operands.pop();

				p1 = Value(operands.top(), scope_jumps.top(), correct_access);
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

				p2 = Value(operands.top(), scope_jumps.top(), correct_access);
				operands.pop();

				p1 = Value(operands.top(), scope_jumps.top(), correct_access);
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

				p2 = Value(operands.top(), scope_jumps.top(), correct_access);
				operands.pop();			

				if (!correct_access) {
					std::cout << "variable access error";
					return false;
				}

				operands.push(Token("DIGIT", std::to_string(+p2)));
			}

			if (cur.GetValue() == "-" && cur.GetType() == "UNARY_OP") {

				p2 = Value(operands.top(), scope_jumps.top(), correct_access);
				operands.pop();

				if (!correct_access) {
					std::cout << "variable access error";
					return false;
				}

				operands.push(Token("DIGIT", std::to_string(-p2)));
			}

			//Compare ops

			if (cur.GetValue() == "<") {

				p2 = Value(operands.top(), scope_jumps.top(), correct_access);
				operands.pop();

				p1 = Value(operands.top(), scope_jumps.top(), correct_access);
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

				p2 = Value(operands.top(), scope_jumps.top(), correct_access);
				operands.pop();

				p1 = Value(operands.top(), scope_jumps.top(), correct_access);
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

				p2 = Value(operands.top(), scope_jumps.top(), correct_access);
				operands.pop();

				p1 = Value(operands.top(), scope_jumps.top(), correct_access);
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

				p2 = Value(operands.top(), scope_jumps.top(), correct_access);
				operands.pop();

				p1 = Value(operands.top(), scope_jumps.top(), correct_access);
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

				p2 = Value(operands.top(), scope_jumps.top(), correct_access);
				operands.pop();

				p1 = Value(operands.top(), scope_jumps.top(), correct_access);
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

				p2 = Value(operands.top(), scope_jumps.top(), correct_access);
				operands.pop();

				p1 = Value(operands.top(), scope_jumps.top(), correct_access);
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

				p2 = Value(operands.top(), scope_jumps.top(), correct_access);
				operands.pop();

				p1 = Value(operands.top(), scope_jumps.top(), correct_access);
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

				p2 = Value(operands.top(), scope_jumps.top(), correct_access);
				operands.pop();

				p1 = Value(operands.top(), scope_jumps.top(), correct_access);
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

				p2 = Value(operands.top(), scope_jumps.top(), correct_access);
				operands.pop();

				if (!correct_access) {
					std::cout << "variable access error";
					return false;
				}

				operands.push(Token("DIGIT", std::to_string(!p2)));
			}

			//Assign op
			if (cur.GetValue() == "=") {

				if (cur.GetType() == "ASSIGN_OP") {

					p2 = Value(operands.top(), scope_jumps.top(), correct_access);
					operands.pop();

					if (!correct_access) {
						std::cout << "variable access error";
						return false;
					}

					not_initialized = false;
					auto var = FindVariable(operands.top().GetValue(), scope_jumps.top(), not_initialized);
					if (not_initialized) {
						if (!variables[scope].insert({ operands.top().GetValue(), p2 }).second) {

							std::cout << "Variable initialization error" << std::endl;
							return false;
						}
					}

					else
						var->second = p2;
					operands.pop();
				}

				else {

					auto var = operands.top().GetValue();
					operands.pop();

					p1 = Value(operands.top(), scope_jumps.top(), correct_access);
					operands.pop();

					if (!correct_access) {
						std::cout << "variable access error";
						return false;
					}

					if (!variables[scope].insert({ var, p1 }).second) {

						std::cout << "Variable initialization error" << std::endl;
						return false;
					}
				}
			}

			//arguments pass op
			if (cur.GetValue() == "REFERENCE") {

				p2 = Value(operands.top(), scope_jumps.top(), correct_access);
				operands.pop();
				operands.push(Token("DIGIT", std::to_string(p2)));
			}

			++pos;
		}

	}

	std::cout << std::endl;	

	return true;
}

void RPN::PrintResults() {

	std::cout << std::endl << "main variables:" << std::endl;
	for (auto& var : variables[1])
		std::cout << var.first << " = " << var.second << std::endl;

}

int RPN::Value(Token tk, int allowed_min_scope, bool &abort_flag) {

	if (tk.GetType() == "DIGIT")
		return std::stoi(tk.GetValue());

	else {

		bool not_init = false;
		auto var = FindVariable(tk.GetValue(), allowed_min_scope, not_init);
		if (not_init) {
			abort_flag = false;
			return 0;
		}

		else 
			return var->second;
	}

}

std::list<Token>::iterator RPN::FindPosition(Token tk) {

	auto start = RPN_List.begin();

	while (*start != tk)
		++start;

	return start;
}

std::unordered_map<std::string, int>::iterator RPN::FindVariable(std::string variable, int allowed_min_scope, bool &not_init_flag) {

	std::unordered_map<std::string, int>::iterator var;

	for (int i = scope; i >= allowed_min_scope; i--) {

		var = variables[i].find(variable);
		if (var != variables[i].end())
			return var;
	}

	var = variables[0].find(variable);
	if (var != variables[0].end())
		return var;

	not_init_flag = true;
	return variables[0].end();
}

bool RPN::InitializeScope() {

	scope++;
	if (!variables.insert({ scope, std::unordered_map<std::string, int>() }).second) {

		std::cout << "Unable to initialize local scope" << std::endl;
		return false;
	}

	return true;
}

void RPN::RemoveScope() {

	variables[scope].clear();
	variables.erase(scope);
	scope--;
}

RPN::~RPN() {}