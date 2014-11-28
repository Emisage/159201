//159201 assignment 1 skeleton
//You need to add your own AddNode and PrintLL functions, as well as an AddMatrices function
//

#include <stdio.h>
#include <stdlib.h>
struct Node {  //declaration
	int row;
	int column;
	int value;
	Node *next;
};
Node *A, *B;  //declaration


void read_matrix(Node* &a, char *file_name){
    //reads a matrix from a file
    
    int col = 0, row = 0, value = 0;  
    FILE *input = NULL;
    input = fopen(file_name, "r");
    
    if(input == NULL){
        printf("Cannot open file %s. Exiting.\n", file_name);
        exit(0);
    }
    //reads the matrix dimensions from the first line 
    fscanf(input, "%d %d", &row, &col); 
    
    //read matrix 
    for(int i = 0; i < row; ++i){
        for(int j = 0; j < col; ++j){
	    //reads each value from this row (second line onwards)
            fscanf(input, "%d", &value);
            if(value == 0) continue;
	    //
            //Include your own add_node(a, i, j, value); function here
	    //
	    //The next line is for debbuging, it can be commented later
	    printf("Element at (%d %d) is different than zero and it is: %d ",i,j,value);
        }
	//the next line is for debbuging purposes, it can be commented out later
	printf("\n");
    }
    
    fclose(input);
}

int main( int argc, char** argv ) {
	if(argc!=3) {printf("needs two matrices \n"); exit(0);}
        read_matrix(A, argv[1]);
	read_matrix(B, argv[2]);
	
}

