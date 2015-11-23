#ifndef STACK_HPP
#define STACK_HPP

#include <iostream>

class Stack {
public:
	Stack( );

	Stack(int data);

	~Stack( );

	int getSize ( ) const;

	int peek ( );

	int pop( );

	void push(int data);

	friend std::ostream& operator<<(std::ostream& output, const Stack& rhs);
private:
	struct Node {
		int data;
		Node* next;
		Node (int data) : data (data), next(nullptr) { }
		Node (int data, Node* next) : data(data), next(next) { }
	};
	Node* top;
	int size;
	
	Stack (const Stack& rhs) = delete;

	Stack& operator=(const Stack& rhs) = delete;
};
#endif //STACK_HPP



