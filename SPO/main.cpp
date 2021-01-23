#include <fstream>

#include "Lexer.h"
#include "Parser.h"
#include "RPN.h"

int main(int argc, char** argv)
{		

	std::ifstream ScriptFile;
	std::string buffer;
	std::string TestString;

	ScriptFile.open("script.txt");

	if (!ScriptFile.is_open())
		return -1;

	while (std::getline(ScriptFile, buffer)) 
		TestString += buffer;
	
	ScriptFile.close();

	Lexer lexer(TestString);
	if (!lexer.GenLexems())
		return 1;	

	lexer.PrintTokens();

	Parser parser(lexer.GetTokens());
	if (!parser.CheckSyntax())
		return 2;
	
	RPN rpn(lexer.GetTokens(), parser.GetFunctionsArgumentsNumber());

	rpn.PrintReverseTokens();

	if (!rpn.Optimization())
		return 31;

	std::cout << std::endl;

	rpn.PrintReverseTokens();

	if (!rpn.Computation())
		return 3;		
	
	return 0;
}