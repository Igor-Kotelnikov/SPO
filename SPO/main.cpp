#include "Lexer.h"
#include "Parser.h"

int main(int argc, char** argv)
{

	std::string TestString = "if (a < 5 || 1) while (x < 3) while (1) x = a + b * 2 - 1;";

	Lexer lexer(TestString);
	lexer.GenLexems();
	
	std::list<Token> TokenPrint = lexer.GetTokens();

	for (auto const &p: TokenPrint)
		std::cout << p << " ";

	Parser parser(lexer.GetTokens());
	parser.CheckSyntax();

	return 0;
}