#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace ads{

/**
 * @brief Node
 */
struct Node
{
    int row     ;
    int column  ;
    int value   ;
    Node *next  ;
};


/**
 * @brief operator << for Node
 */
inline std::ostream&
operator<<(std::ostream& os, Node const& rhs)
{
    return os << rhs.row << " " << rhs.column << " " << rhs.value << std::endl;
}


///////////////////////////////////////////////////////////////////////////////


//! forward declarations
template<typename T> class SparseMatrix;
template<typename T> std::ostream& print_data(SparseMatrix<T> const& m);


/**
 * @brief SparseMatrix
 *
 * using linked list for data storing
 */
template<typename Node>
class SparseMatrix
{
    friend std::ostream& print_data<Node>(SparseMatrix const& m);
public:
    using ValueType =   decltype(Node::value);
    using IndexType =   decltype(Node::row);
    using SizeType  =   std::size_t;

    SparseMatrix() = delete;

    /**
     * @brief Ctor using file name
     * @param fn
     *
     * @abstraction Top
     */
    explicit SparseMatrix(std::string fn):
        head_{nullptr},
        tail_{nullptr},
        rows_{0},
        cols_{0}
    {
        construct_linked_list(fn);
    }

    /**
     * @brief empty
     *
     * @abstraction Top
     */
    bool empty()const
    {
        return !head_ and !tail_;
    }

    /**
     * @brief size
     *
     * @abstraction Top
     */
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
     * @brief add
     * @param node
     *
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
     * @brief read_and_init_dimensions
     * @param ifs
     * @return ifs
     *
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
     * @brief read_and_init_matrix_body
     * @param ifs
     * @return ifs
     *
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
     * @brief construct_linked_list
     * @param fn
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


}//namespace

int main()
{
    ads::SparseMatrix<ads::Node> m{"matrix1.txt"};
    std::cout << "size : " <<m.data_size() << std::endl;
    ads::print_data(m);

    return 0;
}
