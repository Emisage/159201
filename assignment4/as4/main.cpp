#include <iostream>
#include <fstream>
#include <string>

namespace ads {

/**
 * @brief Section A : The List class
 */
template<typename T>
class List
{
    struct Node{    T value_;   Node* prev_;    Node* next_;    };
public:
    using SizeType = std::size_t;

    void push_front(T const& new_value)
    {
        if(empty())
        {
            head_ = tail_       =   new Node{new_value, nullptr, nullptr};
        }
        else
        {
            head_               =   new Node{new_value, nullptr, head_};
            head_->next_->prev_ =   head_;
        }
        ++size_;
    }

    bool empty()const
    {
        return !head_ and !tail_;
    }

    SizeType size() const
    {
        return size_;
    }

    ~List()
    {
        if(not empty())
            for(Node* ptr = head_, *tmp; (tmp=ptr); ptr=ptr->next_)  delete tmp;
        head_ = tail_ = nullptr;
    }

protected:
    Node* head_{};
    Node* tail_{};
private:
    SizeType size_{0};
};


template<typename T>
struct BigNumber;

template<typename T>
BigNumber<T> operator+(BigNumber<T> const& lhs, BigNumber<T> const& rhs);


/**
 * @brief Section B : The BigNumber struct
 */
template<typename T>
struct BigNumber : private List<T>
{
    friend BigNumber operator+ <T>(BigNumber const&, BigNumber const&);

    using Super = List<T>;
    using Super::push_front;

    BigNumber() = default;
    explicit BigNumber(std::string const& num)
    {
        for(auto c = num.crbegin(); c != num.crend(); ++c)
            if(std::isdigit(*c))    push_front(*c - 48);
    }

    std::ostream& print() const
    {
        for(auto ptr = this->head_; ptr; ptr = ptr->next_)
            std::cout << ptr->value_;
        return std::cout << std::endl;
    }
};


/**
 * @brief Section C : operator +
 * @param lhs
 * @param rhs
 * @return
 */
template<typename T>
BigNumber<T> operator+(BigNumber<T> const& lhs, BigNumber<T> const& rhs)
{
    BigNumber<T> sum;
    int carry = 0;
    auto l = lhs.tail_, r = rhs.tail_;

    for(; l and r; l = l->prev_, r = r->prev_)          //add two numbers.
    {
        auto digit_sum = carry + l->value_ + r->value_ ;
        sum.push_front(digit_sum%10);
        carry = digit_sum/10;
    }

    for(auto rest = l?l:r; rest; rest = rest->prev_)  //when either one exhausted.
    {
        auto digit_sum = carry + rest->value_;
        sum.push_front(digit_sum%10);
        carry = digit_sum/10;
    }

    if(carry)                                       //for the last carry,if any.
        sum.push_front(carry);

    return sum;
}


}//namespace


int main(int argc, char ** argv)
{
    if(argc!=2)
    {
        std::cout<< "cannot read the file " << argv[1] << std::endl;
        exit(0);
    }

    std::string l,r;
    {
        std::ifstream ifs{argv[1]};
        std::getline(ifs,l);
        std::getline(ifs,r);
    }

    ads::BigNumber<int> lhs{l}, rhs{r};
    lhs.print() << "+\n";
    rhs.print() << "=\n";
    (lhs+rhs).print();
    return 0;
}

