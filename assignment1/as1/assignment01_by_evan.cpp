//!
//! @author Xiufeng ZHANG ID 06197620
//! @author Boxi Zhang ID 12238967
//! @brief : assignment 1 159.201
//!

#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>


using std::size_t;
using std::ostream;
using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::getline;
using std::stringstream;


template<typename T> struct Node
{
    Node() = default;
    Node(size_t r, size_t c, T val, Node* nxt):
        row_{r}, col_{c}, val_{val}, next_{nxt}
    {}

    size_t row_{0};
    size_t col_{0};
    T val_= T();
    Node* next_{nullptr};
};


template<typename T>
inline bool is_same_position(Node<T> const& lhs, Node<T> const& rhs)
{
    return lhs.row_ == rhs.row_ and lhs.col_ == rhs.col_;
}


template<typename T>
inline bool is_precedent(Node<T> const& lhs, Node<T> const& rhs)
{
    if(lhs.row_ < rhs.row_)         return true;
    else if (lhs.row_ == rhs.row_)  return lhs.col_ < rhs.col_;
    else                            return false;
}


template<typename T>
inline Node<T> operator+(Node<T> const& lhs, Node<T> const& rhs)
{
    return {lhs.row_, lhs.col_, lhs.val_+rhs.val_, nullptr};
}


//! forward declarations
template<typename T> class Matrix;
template<typename T> Matrix<T> operator+(Matrix<T> const&, Matrix<T> const&);
template<typename T> ostream& operator<<(ostream&, Matrix<T> const&);


template<typename T> class Matrix
{
    using Nd = Node<T>;

    friend Matrix operator+<T>(Matrix const& , Matrix const& );
    friend ostream& operator<< <T> (ostream&, Matrix<T> const&);

public:
    Matrix() = default;
    explicit Matrix(string const& fn): Matrix{} { do_ctor_from_file(fn); }
    Matrix(size_t r, size_t c): head_{nullptr},tail_{nullptr},rows_{r},cols_{c} {}

    bool empty() const { return not head_; }
    ostream& print_data() const { return do_print_data(); }

    ~Matrix()
    {
        for(Nd* cur = head_, *tmp; (tmp = cur); cur=cur->next_) delete tmp;
    }

private:
    Nd *head_{}, *tail_{};
    size_t rows_{0}, cols_{0};

    void push_back(Nd node)
    {
        if(empty())
        {
            head_ = tail_   =   new Nd(std::move(node));
        }
        else
        {
            tail_->next_    =   new Nd(std::move(node));
            tail_           =   tail_->next_;
        }
    }

    ostream& do_print_data() const
    {
        for(auto p = head_;  p;  p = p->next_)  cout << p->val_ << " ";
        return cout;
    }

    void do_ctor_from_file(string const& fn)
    {
        ifstream ifs{fn};

        if(!ifs.good())
        {
            cout << "Cannot open file " << fn;
            exit(-1);
        }

        string line;
        getline(ifs, line);
        std::stringstream ss{line};
        ss >> rows_ >> cols_;

        for(size_t r=0; r != rows_; ++r)
        {
            getline(ifs,line);
            stringstream ss{line};
            for(size_t c=0; c != cols_; ++c)
            {
                T value;
                ss >> value;
                if(value)
                    push_back(Nd{r, c, value, nullptr});
            }
        }
    }
};


template<typename T>
ostream& operator<<(ostream& os, Matrix<T> const& mat)
{
    auto curr = mat.head_;
    for(size_t r = 0; r != mat.rows_; ++r)
    {
        for(size_t c = 0; c != mat.cols_; ++c)
        {
            if(!curr or is_precedent(Node<T>{r, c, 0, nullptr}, *curr))
            {
                os << 0 << " ";
            }
            else
            {
                os << curr->val_ << " ";
                curr = curr->next_;
            }
        }
        os << endl;
    }
    return os;
}


template<typename T>
Matrix<T> operator+(Matrix<T> const& lhs, Matrix<T> const& rhs)
{
    Matrix<T> ret{lhs.rows_, lhs.cols_};
    auto append = [&ret] (Node<T> node) { ret.push_back(node); };

    //! copy until either one exausted
    auto l = lhs.head_, r = rhs.head_;
    while(l and r)
    {
        if(is_same_position(*l, *r))
        {
            append(*l + *r);
            l = l->next_;
            r = r->next_;
        }
        else if(is_precedent(*l, *r))
        {
            append(*l);
            l = l->next_;
        }
        else
        {
            append(*r);
            r = r->next_;
        }
    }

    //! copy the rest
    for(auto rest = l?l:r;  rest;  rest = rest->next_) append(*rest);

    return ret;
}


int main(int argc, char** argv )
{
    if(argc!=3)
    {
        std::cout << "needs two matrices\n";
        exit(-1);
    }

    Matrix<int> lhs{argv[1]}, rhs{argv[2]};
    auto sum = lhs + rhs;
    auto print = [](Matrix<int> const& m, string const& str){
        cout << str;
        m.print_data() << endl;
        cout << m;
    };

    print(lhs, "Matrix 1: ");
    print(rhs, "Matrix 2: ");
    print(sum, "Matrix Result: ");

    return 0;
}
