#include <stdio.h>
#include <iostream>
using namespace std;

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
  bool TopTwo(float *array);
  void PopTwo();
  bool TwoorMore();
  void PopTwoandReturn(float *a, float *b );
  void PopTwoandReturn2(float &a, float &b );
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

void Stack::PopTwo() {
// remove the 2 top items from the stack
	Node *p;
	p = listpointer;
	if (listpointer != NULL) {     //        check to avoid underflow
		listpointer = listpointer->next;
		delete p;                //always delete a TEMPORARY variable
	}
	p = listpointer;
	if (listpointer != NULL) {
		listpointer = listpointer->next;
		delete p;                
	}
}

void Stack::PopTwoandReturn(float *a, float *b ) {
// remove the 2 top items from the stack
	Node *p;
	p = listpointer;
	if (listpointer != NULL) {     //        check to avoid underflow
	        *a = listpointer->data;
		listpointer = listpointer->next;
		delete p;                //always delete a TEMPORARY variable
	}
	p = listpointer;
	if (listpointer != NULL) {
  	        *b = listpointer->data;
		listpointer = listpointer->next;
		delete p;                
	}
}


void Stack::PopTwoandReturn2(float &a, float &b ) {
// remove the 2 top items from the stack
	Node *p;
	p = listpointer;
	if (listpointer != NULL) {     //        check to avoid underflow
	        a = listpointer->data;
		listpointer = listpointer->next;
		delete p;                //always delete a TEMPORARY variable
	}
	p = listpointer;
	if (listpointer != NULL) {
  	        b = listpointer->data;
		listpointer = listpointer->next;
		delete p;                
	}
}

float Stack::Top() {
// return the value of the top item
  return listpointer->data;  //WARNING: what if listpointer is NULL?
}

bool Stack::TopTwo(float *array) {
// return a pointer to an array[2]
	if(listpointer==NULL || listpointer->next==NULL) {
		return false;
		array[0]=0.0;
		array[1]=0.0;
	}//not two items
	else{
		array[0]=listpointer->data;
		array[1]=(listpointer->next)->data;
		return true; //success
	}
}

bool Stack::isEmpty() {
// returns true if the stack is empty
  if (listpointer == NULL) {
     return true;
  }
  return false;
}

bool Stack::TwoorMore() {
// return true if the stack has at least two items
	if(listpointer==NULL)return false;
	else{
	  if(listpointer->next==NULL) return false;
	  else return true;
	}
}


Stack A; //This is how to declare a stack
float temparray[2];
int main() {
	A.Push(1.0);
	A.Push(2.0);
	A.Push(3.0);
	A.Push(4.8);
	A.Push(5.3);
	A.Push(6.3);
	A.Push(7.0);
	A.Push(8.0);
	A.Push(9.0);
	A.Push(10.0);
	if (A.TwoorMore()) {
		printf("Stack A has more than two items\n");
		//A.TopTwo(&temparray[0]);
		A.TopTwo(temparray);
		printf("two top items are %f and %f \n",temparray[0],temparray[1]);
	} else {
		printf("Stack A has less than two items\n");
	}
	float a,b;
	A.PopTwoandReturn(&a,&b);
	printf("Return PopTwoandReturn %f %f \n",a,b);
	//or
	float *c,*d;
	c=new float;
	d=new float;
	A.PopTwoandReturn(c,d);
	printf("Return PopTwoandReturn %f %f \n",*c,*d);
	cout << "COUT Return PopTwoandReturn " << *c << " " << *d << endl;
	//or 
	A.PopTwoandReturn2(a,b);
	printf("Return PopTwoandReturn2  %f %f \n",a,b);
	
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
