#include <stdio.h>

struct Node {
  float data;
  Node *next;
};

class Stack {
private:                         //the data differs from the array
  Node *listpointer;
public:                          //the functions (methods) are identical
  Stack();
  ~Stack();
  void Push(float newthing);
  void Pop();
  float Top();
  bool isEmpty();
};

Stack::Stack() {
// constructor
  listpointer = NULL;
}

Stack::~Stack() {
// destructor

}

void Stack::Push(float newthing) {
// place the new thing on top of the stack
Node *temp;
  temp = new Node;             //same as add node to front of linked-list
  temp->data = newthing;
  temp->next = listpointer;    //NOTE: no overflow problem
  listpointer = temp;
}
void Stack::Pop() {
// remove top item from the stack
Node *p;
  p = listpointer;
  if (listpointer != NULL) {     //        check to avoid underflow
     listpointer = listpointer->next;
     delete p;                //always delete a TEMPORARY variable
  }
}

float Stack::Top() {
// return the value of the top item
  return listpointer->data;  //WARNING: what if listpointer is NULL?
}

bool Stack::isEmpty() {
// returns true if the stack is empty
  if (listpointer == NULL) {
     return true;
  }
  return false;
}

Stack A, B;

int main() {
  // main is the same as it was with the stack based on an array
	Stack A, B;  //This is how to declare a stack
	A.Push(62.3);
	A.Push(2.4);
	B.Push(47.1);
	A.Pop();
	if (A.isEmpty()) {
		printf("Stack A is empty\n");
	} else {
		//float x = A.Top();
		//printf("Top item in A is %1.1f\n", x);
		printf("Top item in A is %1.1f\n", A.Top());
	}
	printf("Top item in A is %1.1f\n", A.Top());
	printf("Top item in B is %1.1f\n", B.Top());
	A.Pop();
	if (A.isEmpty()) {
		printf("Stack A is empty\n");
	} 
	else 	printf("Top item in A is %1.1f\n", A.Top());
}
