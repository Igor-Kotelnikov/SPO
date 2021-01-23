#pragma once
#include "Token.h"

class Triad
{

private:

	Token operation;
	Token op1;
	Token op2;

public:

	Triad(Token operand_left, Token operand_right, Token operation);

	Token GetLeftOperand();
	Token GetRightOperand();
	Token GetOperation();

	void SetLeftOperand(Token tk);
	void SetRightOperand(Token tk);
	void SetOperation(Token tk);

	~Triad();
};

