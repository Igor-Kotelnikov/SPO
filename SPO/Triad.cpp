#include "Triad.h"

Triad::Triad(Token operand_left, Token operand_right, Token operation) {

	this->op1 = operand_left;
	this->op2 = operand_right;
	this->operation = operation;
}

Token Triad::GetLeftOperand() {

	return this->op1;
}

Token Triad::GetOperation() {

	return this->operation;
}

Token Triad::GetRightOperand() {

	return this->op2;
}

void Triad::SetLeftOperand(Token tk) {

	this->op1 = tk;
}

void Triad::SetOperation(Token tk) {

	this->operation = tk;
}

void Triad::SetRightOperand(Token tk) {

	this->op2 = tk;
}

Triad::~Triad() {}