#include "LinkedList.h"

LinkedList::LinkedList()
{
	size = 0;

	head = nullptr;
	tail = nullptr;
}

LinkedList::~LinkedList()
{

	if (size) {

		while (head->next != nullptr) {

			head = head->next;
			delete head->prev;
		}

		delete head;
	}
}

void LinkedList::push_front(int val)
{
	Node *tmp = head;
	head = new Node(val, nullptr, tmp);

	if (!size)
		tail = head;
	else
		tmp->prev = head;

	size++;
}

void LinkedList::push_back(int val)
{
	Node *tmp = tail;
	tail = new Node(val, tmp, nullptr);

	if (!size)
		head = tail;
	else
		tmp->next = tail;

	size++;
}

void LinkedList::pop_front()
{
	if (size > 1) {
		head = head->next;
		delete head->prev;
	}
	else
		delete head;

	size--;
}

void LinkedList::pop_back()
{
	if (size > 1) {
		tail = tail->prev;
		delete tail->next;
	}
	else
		delete tail;

	size--;
}

void LinkedList::insert(int pos, int val)
{
	Node *tmp = head;
	
	if (head == nullptr) {
		head = tail = new Node(val, nullptr, nullptr);
		size++;
	}

	else if (!pos)
		push_front(val);

	else if (pos == size)
		push_back(val);

	else {

		for (int i = 0; i != pos; i++)
			tmp = tmp->next;

		tmp = new Node(val, tmp->prev, tmp);
		
		tmp->prev->next = tmp;
		tmp->next->prev = tmp;

		size++;
	}		

}

void LinkedList::remove(int pos)
{

	if (!pos)
		pop_front();

	else if (pos == size - 1)
		pop_back();

	else {
		Node *tmp = head;

		for (int i = 0; i != pos; i++)
			tmp = tmp->next;

		delete tmp;

		size--;
	}
}

int LinkedList::get(int pos)
{
	Node *tmp = head;

	for (int i = 0; i != pos; i++)
		tmp = tmp->next;

	return tmp->value;
}

int LinkedList::getSize()
{
	return size;
}