#pragma once
#include "Node.h"

class LinkedList
{

private:

	int size;
	
	Node* head;
	Node* tail;

public:

	LinkedList();
	~LinkedList();

	void push_front(int val);
	void push_back(int val);	

	void pop_front();
	void pop_back();

	void insert(int pos, int val);
	void remove(int pos);
		
	int get(int pos);

	int getSize();

};

