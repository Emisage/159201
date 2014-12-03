#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <stack>


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
    auto ret    =   lhs;
    ret.value   +=  rhs.value;
    ret.next    =   nullptr;
    return ret;
}

inline bool is_same_position(Node const& lhs, Node const& rhs)
{
    return lhs.row == rhs.row and lhs.column == rhs.column;
}

inline bool is_precedent(Node const& lhs, Node const& rhs)
{
    if(lhs.row < rhs.row)       return true;
    else if(lhs.row == rhs.row) return lhs.column < rhs.column;
    else                        return false;
}


///////////////////////////////////////////////////////////////////////////////


//! forward declarations
template<typename T>
class SparseMatrix;

template<typename T>
SparseMatrix<T> operator+(SparseMatrix<T> const& lhs, SparseMatrix<T> const& rhs);

template<typename T>
std::ostream& operator<<(std::ostream& os, SparseMatrix<T> const& rhs);


template<typename Node>
class SparseMatrix
{
    friend SparseMatrix
    operator+  <Node> (SparseMatrix const& lhs, SparseMatrix const& rhs);

    friend std::ostream&
    operator<< <Node> (std::ostream& os, SparseMatrix<Node> const& rhs);

public:
    using ValueType =   decltype(Node::value);
    using IndexType =   decltype(Node::row);
    using SizeType  =   std::size_t;

    SparseMatrix(SparseMatrix const&)               = delete;
    SparseMatrix& operator=(SparseMatrix const&)    = delete;
    SparseMatrix& operator=(SparseMatrix &&)        = delete;

    //! default Ctor
    SparseMatrix():
        head_{nullptr}, tail_{nullptr}, rows_{0}, cols_{0}
    {}

    //! Ctor with dimensions
    SparseMatrix(SizeType rows, SizeType cols):
        head_{nullptr}, tail_{nullptr}, rows_{rows}, cols_{cols}
    {}

    //! Ctor with file name
    explicit SparseMatrix(std::string fn):
        SparseMatrix{}
    {
        construct_linked_list(fn);
    }

    //! move Ctor
    SparseMatrix(SparseMatrix && m) noexcept :
        SparseMatrix{m.head_, m.tail_, m.rows_, m.cols_}
    {
        m.head_ = m.tail_ = nullptr;
    }


    bool empty()const
    {
        return !head_ and !tail_;
    }

    SizeType data_size()const
    {
        return do_data_size();
    }

    std::ostream& print_data() const
    {
        return do_print_data();
    }

    ~SparseMatrix()
    {
        deallocate_data();
    }

private:
    Node* head_;
    Node* tail_;
    SizeType rows_;
    SizeType cols_;

    //! private Ctor with 4 args
    SparseMatrix(Node* h, Node* t, SizeType rows, SizeType cols):
        head_{h},
        tail_{t},
        rows_{rows},
        cols_{cols}
    {}

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
//        std::cout << "ads> added:" << *tail_ << std::endl;
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
//        std::cout << "Matrix dimensions " << rows_ << " " << cols_ << std::endl;
        return ifs;
    }

    /**
     * @abstraction II
     */
    std::ifstream& read_and_init_body(std::ifstream& ifs)
    {
        std::string line;
        for(IndexType r=0; r != rows_; ++r)
        {
            std::getline(ifs,line);
            std::stringstream stream{line};
            for(IndexType c=0; c != cols_; ++c)
            {
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

    /**
     * @abstraction I
     *
     * @complx O(n)
     */
    SizeType do_data_size() const
    {
        SizeType len = 0;
        for(auto curr = head_; curr; curr = curr->next) ++len;
        return len;
    }

    /**
     * @abstraction I
     *
     * @complx O(n)
     */
    std::ostream& do_print_data() const
    {
        for(auto curr = head_; curr; curr = curr->next)
            std::cout << curr->value << " ";
        return std::cout;
    }

    /**
     * @abstraction I
     *
     * @complx O(n)
     */
    void deallocate_data()
    {
        if(not empty())
        {
            std::stack<Node*> stk;
            for(auto poi = head_; poi; poi = poi->next) stk.push(poi);
            for( ; not stk.empty(); stk.pop()) delete stk.top();
            head_ = tail_ = nullptr;
        }
    }
};


template<typename T>
std::ostream& operator<<(std::ostream& os, SparseMatrix<T> const& rhs)
{
    auto curr = rhs.head_;
    using IndexType = typename SparseMatrix<T>::IndexType;
    for(IndexType r = 0; r != rhs.rows_; ++r)
    {
        for(IndexType c = 0; c != rhs.cols_; ++c)
        {
            if(!curr or is_precedent(Node{r, c, 0, nullptr}, *curr))
            {
                os << 0 << " ";
            }
            else
            {
                os << curr->value << " ";
                curr = curr->next;
            }
        }
        std::cout << std::endl;
    }

    return os;
}


//! add two matrices
template<typename T>
SparseMatrix<T> operator+(SparseMatrix<T> const& lhs, SparseMatrix<T> const& rhs)
{
    if(lhs.cols_ != rhs.cols_ or lhs.rows_ != rhs.rows_)
        throw std::runtime_error{"trying to add matrix with different dimensions!"};

    SparseMatrix<T> ret{lhs.rows_, lhs.cols_};

    //! copy until either one exausted
    auto l = lhs.head_, r = rhs.head_;
    while(l and r)
    {
        if(is_same_position(*l, *r))
        {
            ret.add(*l + *r);
            l = l->next;
            r = r->next;
        }
        else if(is_precedent(*l, *r))
        {
            ret.add(Node(*l));
            l = l->next;
        }
        else
        {
            ret.add(Node(*r));
            r = r->next;
        }
    }

    //! copy the rest
    for(auto rest = (l ? l : r);  rest;  rest = rest->next)  ret.add(Node(*rest));

    return ret;
}


}//namespace

int main()
{
    ads::SparseMatrix<ads::Node> lhs{"matrix1.txt"}, rhs{"matrix2.txt"};
    auto sum = lhs + rhs;

    std::cout << "Matrix 1: ";
    lhs.print_data() << std::endl;
    std::cout << lhs;

    std::cout << "Matrix 2: ";
    rhs.print_data() << std::endl;
    std::cout << rhs;

    std::cout << "Matrix Result: ";
    sum.print_data() << std::endl;
    std::cout << sum;

    return 0;
}
