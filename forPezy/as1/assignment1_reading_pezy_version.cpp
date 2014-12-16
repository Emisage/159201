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

void add_node(Node* &a, int i, int j, int value){
    Node *node = (Node*)malloc(sizeof(Node));
    node->row = i;
    node->column = j;
    node->value = value;
    
    Node **p = &a;
    while (*p) 
        p = &(*p)->next;
        
    node->next = *p;
    *p = node;
} 

Node* add_matrices(Node* a, Node* b){
    Node *sum = NULL, *p1 = a, *p2 = b;
    
    while (p1 && p2) {
        int pos1 = p1->row * 10 + p1->column;
        int pos2 = p2->row * 10 + p2->column;
        
        if (pos1 == pos2) {
            add_node(sum, p1->row, p1->column, p1->value + p2->value);
            p1 = p1->next;
            p2 = p2->next;
        } else if (pos1 < pos2) {
            add_node(sum, p1->row, p1->column, p1->value);
            p1 = p1->next;
        } else {
            add_node(sum, p1->row, p1->column, p1->value + p2->value);
            p2 = p2->next;
        }
    }
    while (p1) {
        add_node(sum, p1->row, p1->column, p1->value);
        p1 = p1->next;
    }
    while (p2) {
        add_node(sum, p2->row, p2->column, p2->value);
        p2 = p2->next;
    }
    
    return sum;
}

void printAndClear(Node *node){
    int col = 0, row = 0;
    for (Node *p=node; p; p=p->next) {
        if (!p->next) {
            row = p->row;
            col = p->column;
        }
        printf("%d ", p->value);
    }
    printf("\n");
    
    for (int i=0; i<=row; ++i) {
        for (int j=0; j<=col; ++j)
            if (node->row == i && node->column == j) {
                printf("%d ", node->value);
                // free
                Node *del = node;
                node = node->next;
                free(del);
            }
            else printf("0 ");
        printf("\n");
    }
}

void print_all(Node* a, Node* b, Node* ret){
    printf("Matrix 1: ");
    printAndClear(a);
    printf("Matrix 2: ");
    printAndClear(b);
    printf("Matrix Result: ");
    printAndClear(ret);
}

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
            else add_node(a, i, j, value);
        }
    }
    
    fclose(input);
}

int main( int argc, char** argv ) {
	if(argc!=3) {printf("needs two matrices \n"); exit(0);}
    read_matrix(A, argv[1]);
	read_matrix(B, argv[2]);
	Node *sum = add_matrices(A, B);
	print_all(A, B, sum);
}

