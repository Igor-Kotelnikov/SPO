#include "Lexer.h"

int main(int argc, char** argv)
{

	std::string TestString = "x = a + b * 2 - 1";

	Lexer lexer(TestString);
	lexer.GenLexems();
	
	std::list<Token> TokenPrint = lexer.GetTokens();

	for (auto const &p: TokenPrint)
		std::cout << p << " ";

	return 0;
}