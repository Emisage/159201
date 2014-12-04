#include <stdio.h>

class Stack {
		private:
		  float data[100];
		  int index;
		public:
		  Stack();
		  ~Stack();
		  void Push(float newthing);
		  void Pop();
		  float Top();
		  bool isEmpty();
};

Stack::Stack() { //Note: no data type in front
// constructor
	index = -1;
}

Stack::~Stack() {
// destructor

}

void Stack::Push(float newthing) {
// place the new thing on top of the stack
	index++;
	data[index] = newthing; //Warning: watch for overflow
}

void Stack::Pop() {
// remove the top item from the stack
	if (index > -1) { index--; } //Takes care of underflow
}

float Stack::Top() {
// return the value of the top item in the stack
	return data[index]; //Warning: what if stack is empty?
}

bool Stack::isEmpty() {
// return true if the stack is empty
	if (index < 0) { return true; }
	return false;
}

Stack A, B; //This is how to declare a stack

int main() {
	A.Push(62.3);
	A.Push(2.0);
	A.Push(3.0);
	A.Push(4.0);
	A.Pop();
	A.Pop();
	A.Pop();
	A.Pop();
	B.Push(5.0);
	if (A.isEmpty()) {
		printf("Stack A is empty\n");
	} else {
		float x = A.Top();
		printf("Top item in A is %1.1f\n", x);
	}

}
