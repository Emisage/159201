#include <fstream>
#include <string>
#include <iostream>

using std::cout; using std::endl;

struct StackNode {
    int val;
    StackNode *next;
    StackNode(int x) : val(x), next(nullptr) {}
};

class Stack {
public:
    ~Stack() 
    {
        while (node) {
            StackNode *pDel = node;
            node = pDel->next;
            delete pDel;
        }   
    }
    
    bool empty() { return node == nullptr; }
    void push(int val)
    {
        StackNode *new_node = new StackNode(val);
        new_node->next = node;
        node = new_node;
    }
    int pop()
    {
        StackNode *top = node;
        int ret = top->val;
        node = top->next;
        delete top;
        return ret;
    }
private:
    StackNode *node{nullptr};
};

int main(int argc, char **argv)
{
    if (argc != 2) { cout << "cannot read file" << endl; exit(0); }
    std::ifstream ifs(argv[1]);
    Stack stk;
    for (std::string exp; getline(ifs, exp); ) {
        if (isdigit(exp[0])) {
            cout << "reading number " << exp << endl; 
            stk.push(stoi(exp));
        }
        else if (exp[0] == '+' || exp[0] == '-' || exp[0] == '*' || exp[0] == '/') {
            cout << "reading operator " << exp[0] << endl;
            int op1{0}, op2{0};
            if (!stk.empty()) op2 = stk.pop();
            else { cout << "too many operators" << endl; exit(0); }
            if (!stk.empty()) op1 = stk.pop();
            else { cout << "too many operators" << endl; exit(0); }
            switch (exp[0]) {
            case '+' : stk.push(op1 + op2); break;
            case '-' : stk.push(op1 - op2); break;   
            case '*' : stk.push(op1 * op2); break;
            case '/' : stk.push(op1 / op2); break;
            }
        }
    }
    int ret = stk.pop();
    if (stk.empty()) cout << "The result is " << ret << endl;
    else cout << "too many numbers" << endl;
}
