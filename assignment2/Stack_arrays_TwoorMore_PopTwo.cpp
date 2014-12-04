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
		  bool TopTwo(float *array);
		  void PopTwo();
		  bool TwoorMore();
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

void Stack::PopTwo() {
// remove the 2 top items from the stack
	if (index > -1) { index--; } 
	if (index > -1) { index--; } 
}

float Stack::Top() {
// return the value of the top item in the stack
	return data[index]; //Warning: what if stack is empty?
}

bool Stack::TopTwo(float *array) {
// return a pointer to an array[2]
	if(index>=1){
		array[0]=data[index];
		array[1]=data[index-1];
		return true; //success
	}
	else{ return false;}//less than two items
}

bool Stack::isEmpty() {
// return true if the stack is empty
	if (index < 0) { return true; }
	return false;
}

bool Stack::TwoorMore() {
// return true if the stack has at least two items
	if (index >= 1 ) { return true; }
	return false;
}

Stack A; //This is how to declare a stack
float temparray[2];
int main() {
	A.Push(45.3);
	A.Push(62.3);
	A.Push(2.0);
	A.Push(3.0);
	A.Push(4.0);
	if (A.TwoorMore()) {
		printf("Stack A has more than two items\n");
		A.TopTwo(&temparray[0]);
		printf("two top items are %f and %f \n",temparray[0],temparray[1]);
	} else {
		printf("Stack A has less than two items\n");
	}
	A.PopTwo();
	if (A.TwoorMore()) {
		printf("Stack A has more than two items\n");
		A.TopTwo(&temparray[0]);
		printf("two top items are %f and %f \n",temparray[0],temparray[1]);
	} else {
		printf("Stack A has less than two items\n");
	}
	A.PopTwo();
	if (A.TwoorMore()) {
		printf("Stack A has more than two items\n");
		A.TopTwo(&temparray[0]);
		printf("two top items are %f and %f \n",temparray[0],temparray[1]);
	} else {
		printf("Stack A has less than two items\n");
	}

}
