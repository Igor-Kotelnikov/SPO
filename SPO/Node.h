class Node
{
public:

	int value;

	Node* next;
	Node* prev;

	Node(int val, Node* left, Node* right);
	~Node();
};