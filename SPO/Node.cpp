#include "Node.h"

Node::Node(int val, Node* left, Node* right)
{
	this->value = val;
	this->prev = left;
	this->next = right;
}

Node::~Node()
{

	if (this->prev != nullptr)
		this->prev->next = this->next;

	if (this->next != nullptr)
		this->next->prev = this->prev;
}