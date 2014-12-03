#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace ads{

struct Node
{
    int row     = 0;
    int column  = 0;
    int value   = 0;
    Node *next  = nullptr;
};

template<typename Node>
class SparseMatrix
{
public:
    using Value = decltype(Node::value);

    SparseMatrix() = delete;

    explicit SparseMatrix(std::string fn):
        Head_{nullptr}, rows_{0}, cols_{0}
    {
        construct_linked_list(fn);
    }

private:
    Node* Head_;
    std::size_t rows_;
    std::size_t cols_;

    Node* add()

    std::ifstream& read_and_init_dimensions(std::ifstream& ifs)
    {
        std::string line;
        std::getline(ifs, line);
        std::stringstream stream{line};
        stream >> rows_ >> cols_;
        std::cout << "Matrix dimensions " << rows_ << " " << cols_ << std::endl;
        return ifs;
    }

    std::ifstream& read_and_init_matrix_body(std::ifstream& ifs)
    {
        std::string line;
        for(unsigned r=0; r != rows_; ++r)
        {
            std::getline(ifs,line);
            std::stringstream stream{line};
            for(unsigned c=0; c != cols_; ++c){
                Value value{};
                stream >> value;
                if(value == 0) continue;
            }
        }
    }

    void construct_linked_list(std::string const& fn)
    {
        std::ifstream ifs{fn};
        if(!ifs.good())
            throw std::runtime_error{"Cannot open file " + fn};
        ifs = read_and_init_dimensions(ifs);

        //working

    }

};

}//namespace

int main()
{
    return 0;
}

