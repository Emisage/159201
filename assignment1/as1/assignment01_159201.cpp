#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace ads{


struct Node
{
    int row     ;
    int column  ;
    int value   ;
    Node *next  ;
};


inline std::ostream&
operator<<(std::ostream& os, Node const& rhs)
{
    return os << rhs.row << " " << rhs.column << " " << rhs.value << std::endl;
}

inline Node
operator +(Node const& lhs, Node const& rhs)
{
    auto ret = lhs;
    ret.value += rhs.value;
    ret.next = nullptr;
    return ret;
}


///////////////////////////////////////////////////////////////////////////////


//! forward declarations
template<typename T>
class SparseMatrix;
template<typename T>
std::ostream& print_data(SparseMatrix<T> const& m);
template<typename T>
SparseMatrix<T> operator+(SparseMatrix<T> const& lhs, SparseMatrix<T> const& rhs);


template<typename Node>
class SparseMatrix
{
    friend std::ostream&
    print_data<Node>(SparseMatrix const& m);
    friend SparseMatrix
    operator+ <Node> (SparseMatrix const& lhs, SparseMatrix const& rhs);

public:
    using ValueType =   decltype(Node::value);
    using IndexType =   decltype(Node::row);
    using SizeType  =   std::size_t;

    SparseMatrix():
        head_{nullptr},
        tail_{nullptr},
        rows_{0},
        cols_{0}
    {}

    explicit SparseMatrix(std::string fn):
        SparseMatrix{}
    {
        construct_linked_list(fn);
    }

    bool empty()const
    {
        return !head_ and !tail_;
    }


    SizeType data_size()const
    {
        SizeType len = 0;
        for(auto curr = head_; curr; curr = curr->next) ++len;
        return len;
    }

private:
    Node* head_;
    Node* tail_;
    SizeType rows_;
    SizeType cols_;

    /**
     * @abstraction III
     */
    void add(Node && node)
    {
        if(empty()){
            head_ = tail_ = new Node(std::move(node));
        }
        else{
            tail_->next =   new Node(std::move(node));
            tail_       =   tail_->next;
        }

        std::cout << "ads> added:" << *tail_ << std::endl;
    }

    /**
     * @abstraction II
     */
    std::ifstream& read_and_init_dimensions(std::ifstream& ifs)
    {
        std::string line;
        std::getline(ifs, line);
        std::stringstream stream{line};
        stream >> rows_ >> cols_;
        std::cout << "Matrix dimensions " << rows_ << " " << cols_ << std::endl;
        return ifs;
    }

    /**
     * @abstraction II
     */
    std::ifstream& read_and_init_body(std::ifstream& ifs)
    {
        std::string line;
        for(IndexType r=0; r != rows_; ++r){
            std::getline(ifs,line);
            std::stringstream stream{line};
            for(IndexType c=0; c != cols_; ++c){
                ValueType value{0};
                stream >> value;
                if(value)
                    add({r, c, value, nullptr});
            }
        }
        return ifs;
    }

    /**
     * @abstraction I
     */
    void construct_linked_list(std::string const& fn)
    {
        std::ifstream ifs{fn};
        if(!ifs.good())
            throw std::runtime_error{"Cannot open file " + fn};
        read_and_init_body(read_and_init_dimensions(ifs));
    }
};


template<typename T>
inline std::ostream& print_data(SparseMatrix<T> const& m)
{
    for(auto curr = m.head_; curr; curr = curr->next)
        std::cout << curr->value << " ";
    return std::cout;
}

template<typename T>
SparseMatrix<T> operator+(SparseMatrix<T> const& lhs, SparseMatrix<T> const& rhs)
{
    if(lhs.cols_ != rhs.cols_ or lhs.rows_ != rhs.rows_)
        throw std::runtime_error{"trying to add matrix with different dimensions!"};
    auto rows = lhs.rows_, cols = rhs.cols_;

    SparseMatrix<T> ret;

    //! working
}


}//namespace

int main()
{
    std::cout << (ads::Node{1,2,42,nullptr} + ads::Node{1,2,10,nullptr});

    ads::SparseMatrix<ads::Node> m{"matrix1.txt"};
    std::cout << "size : " <<m.data_size() << std::endl;
    ads::print_data(m);

    return 0;
}
