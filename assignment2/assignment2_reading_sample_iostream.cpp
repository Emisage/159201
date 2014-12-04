#include <cstdlib>
#include <iostream>
#include <fstream>
#include <locale>
#include <sstream>

using namespace std;

/* implement your stack here */

main( int argc, char** argv ){//get arguments from command line, i.e., yourexec filename
	int result, op1,op2,number,i;
	char oper;
	string expression;
	ifstream input_file;
	if(argc==2) input_file.open(argv[1]);
	else { cout<< "cannot read file " << argv[1] << endl; exit(0);}
	while(!input_file.eof()){
		getline(input_file,expression);
		if(isdigit(expression[0])){
		  stringstream line(expression);
		  line >> number;
		  cout << "reading number " << number << endl;
		  //modify here to deal with the Stack
		  //PUSH number
		}
		else {
		  if(expression[0]=='+' || expression[0]=='-'|| expression[0]=='/'||expression[0]=='*') {
		    stringstream line(expression);
		    line >> oper;
		    cout << "reading operator " << oper << endl;
		    //op2 = TOP
		    //POP
		    //op1 = TOP 
		    //POP
		    //compute result
		    //PUSH result
		  }
		}
	}
	//finalanswer = TOP
	//the last element of the stack should be the final answer...
}
