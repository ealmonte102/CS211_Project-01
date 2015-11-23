 #include "Stack.hpp"

Stack::Stack ( ) : top (nullptr), size (0) { }


Stack::Stack (int data) : top (new Node (data)), size (1) { }

Stack::~Stack ( ) {
	while (top != nullptr) {
		Node* tempNext = top->next;
		delete top;
		top = tempNext;
	}
}

int Stack::getSize ( ) const {
	return size;
}

int Stack::peek( ) {
	return top->data;
}

int Stack::pop ( ) {
	int dataintoReturn = top->data;
	Node* nextTop = top->next;
	delete top;
	top = nextTop;
	--size;
	return dataintoReturn;
}


void Stack::push (int data) {
	top = new Node (data, top);
	++size;
}

std::ostream& operator<<(std::ostream& output, const Stack& rhs) {
	Stack::Node* current = rhs.top;
	while(current != nullptr) {
		output << current->data << " ";
		current = current->next;
	}
	output << "\n";
	return output;
}