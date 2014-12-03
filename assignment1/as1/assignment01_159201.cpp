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

template<typename Node>
class SparseMatrix
{
public:
    using ValueType =   decltype(Node::value);
    using SizeType  =   decltype(Node::row);

    SparseMatrix() = delete;

    /**
     * @brief Ctor using file name
     * @param fn
     *
     * @abstraction interface
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
     * @abstraction interface
     */
    bool empty()const
    {
        return !head_ and !tail_;
    }



private:
    Node* head_;
    Node* tail_;
    std::size_t rows_;
    std::size_t cols_;

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
    std::ifstream& read_and_init_matrix_body(std::ifstream& ifs)
    {
        std::string line;
        for(SizeType r=0; r != rows_; ++r)
        {
            std::getline(ifs,line);
            std::stringstream stream{line};
            for(SizeType c=0; c != cols_; ++c){
                ValueType value{0};
                stream >> value;
                if(value == 0) continue;
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
        read_and_init_matrix_body(read_and_init_dimensions(ifs));
    }
};

}//namespace

int main()
{
    ads::SparseMatrix<ads::Node> m{"matrix1.txt"};

    return 0;
}

