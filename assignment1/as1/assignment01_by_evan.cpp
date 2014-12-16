#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>


using std::size_t;
using std::ostream;


template<typename T> struct Node
{
    size_t row_;
    size_t col_;
    T val_;
    Node* next_;
};


template<typename T>
inline bool is_same_position(Node<T> const& lhs, Node const& rhs)
{
    return lhs.row_ == rhs.row_ and lhs.col_ == rhs.col_;
}


template<typename T>
inline bool is_precedent(Node<T> const& lhs, Node<T> const& rhs)
{
    if(lhs.row_ < lhs.row_)         return true;
    else if (lhs.row_ == rhs.row_)  return lhs.col_ < rhs.col_;
    else                            return false;
}


template<typename T>
inline Node<T> operator+(Node<T> const& lhs, Node<T> const& rhs)
{
    return {lhs.row_, lhs.col_, lhs.val_+rhs.val_, nullptr};
}


template<typename T> class Matrix
{
    using Nd = Node<T>;
public:
    bool empty() const{ return not data_; }

    ~Matrix()
    {
        for(Nd* cur = data_, *tmp; (tmp = cur); cur=cur->next_) delete tmp;
    }

private:
    Nd* data_;
    size_t rows_, cols_;
};

int main()
{

}
