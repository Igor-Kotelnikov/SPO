#include "Lexer.h"
#include "Parser.h"
#include "RPN.h"
#include "LinkedList.h"
int main(int argc, char** argv)
{
	
	//if (((1) > 0 - 2) || 1) x = ((1+2)*2);

	std::string TestString = "x.create_list(); x.push_back(3); x.push_front(1); x.insert(1, 2); y = x.get(0) + x.get_size();";

	Lexer lexer(TestString);
	if (!lexer.GenLexems())
		return 1;	

	Parser parser(lexer.GetTokens());
	if (!parser.CheckSyntax())
		return 2;
	
	RPN rpn(lexer.GetTokens());
	if (!rpn.Computation())
		return 3;		
	
	return 0;
}