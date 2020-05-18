#include "Lexer.h"
#include "Parser.h"
#include "RPN.h"

int main(int argc, char** argv)
{

	//Program cannot operate with only part of code, code must be input fully
	std::string TestString = "x = 5;while (x<10) x = x+1;";

	Lexer lexer(TestString);
	if (!lexer.GenLexems())
		return 1;

	std::list<Token> TokenPrint = lexer.GetTokens();

	for (auto const &p : TokenPrint)
		std::cout << p << " ";

	Parser parser(lexer.GetTokens());
	if (!parser.CheckSyntax())
		return 2;
	
	RPN rpn(lexer.GetTokens(), parser.GetVariableHash());
	if (!rpn.Computation())
		return 3;
		
	
	return 0;
}