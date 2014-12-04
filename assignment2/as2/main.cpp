#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <algorithm>
#include <map>
#include <functional>


namespace ads {

/**
 * @brief The List class
 */
template<typename T>
class List
{
    struct Node
    {
        T value;
        Node* next;
    };
public:
    using SizeType = std::size_t;

    List() = default;

    T const& front() const
    {
        return head_->value;
    }

    void push_front(T const& new_data)
    {
        do_push_front(new_data);
    }

    void pop_front()
    {
        do_pop_front();
    }

    bool empty() const
    {
        return !head_ and !tail_;
    }

    std::size_t size() const
    {
        return size_;
    }

    ~List()
    {
        do_deallocate();
    }

private:
    Node* head_{nullptr};
    Node* tail_{nullptr};
    std::size_t size_{0};

    void do_deallocate()
    {
        for(Node *ptr{head_}, *tmp;  (tmp=ptr);  delete tmp)
            ptr = ptr->next;
    }

    void do_push_front(T const& new_value)
    {
        if(empty())
            head_ = tail_   =   new Node{new_value, nullptr};
        else
            head_           =   new Node{new_value, head_};
        ++size_;
    }

    void do_pop_front()
    {
        if(empty())
            throw std::runtime_error{"underflow!"};

        if(head_ == tail_)
        {
            delete head_;
            head_ = tail_ = nullptr;
            size_ = 0;
        }
        else
        {
            auto tmp = head_;
            head_ = head_->next;
            delete tmp;
            --size_;
        }
    }
};


///////////////////////////////////////////////////////////////////////////////


/**
 * @brief The Stack class
 */
template<typename T, typename Container = ads::List<T> >
class Stack
{
public:
    using SizeType = typename Container::SizeType;
    Stack(Stack const&)             =   delete;
    Stack(Stack &&)                 =   delete;
    Stack& operator=(Stack const&)  =   delete;
    Stack& operator=(Stack &&)      =   delete;

    Stack() = default;

    void push(T const& new_value)
    {
        data_.push_front(new_value);
    }

    T const& top() const
    {
        return data_.front();
    }

    void pop()
    {
        data_.pop_front();
    }

    SizeType size() const
    {
        return data_.size();
    }

    bool empty() const
    {
        return data_.empty();
    }

private:
    Container data_{};
};


///////////////////////////////////////////////////////////////////////////////


template<typename T>    T add (T lhs, T rhs){   return lhs + rhs;   }
template<typename T>    T mns (T lhs, T rhs){   return lhs - rhs;   }
template<typename T>    T mul (T lhs, T rhs){   return lhs * rhs;   }
template<typename T>    T div (T lhs, T rhs){   return lhs / rhs;   }


///////////////////////////////////////////////////////////////////////////////


class RpnParser
{
public:
    using Evaluator = std::map<char, std::function<int(int,int)>>;

    RpnParser():
        stk_{},
        eval_
        {
            {'+', ads::add<int>},
            {'-', ads::mns<int>},
            {'*', ads::mul<int>},
            {'/', ads::div<int>}
        }
    {}

    int parse(std::string const& fn)
    {
       return do_parse_and_evaluate(fn);
    }

private:
    ads::Stack<int> stk_;
    const Evaluator eval_;

    //! abstraction II
    int evaluate(char op, int lhs, int rhs)
    {
        return eval_.at(op)(lhs, rhs);
    }

    //! abstraction II
    bool is_operator(char ch) const
    {
        return eval_.cend() != eval_.find(ch);
    }

    //! abstraction II
    int get_operand_and_check()
    {
        if(stk_.empty())
            throw std::logic_error{"too many operators!"};
        auto operand = stk_.top();
        stk_.pop();
        return operand;
    }

    //! abstraction II
    int get_result_and_check()
    {
        if(1 != stk_.size())
            throw std::logic_error{"too many numbers!"};
        auto result = stk_.top();
        stk_.pop();
        return result;
    }

    //! abstraction I
    int do_parse_and_evaluate(std::string const& fn)
    {
        for(std::ifstream ifs{fn}; !ifs.eof();/* */)
        {
            std::string expr;
            std::getline(ifs, expr);
            for(auto it = expr.cbegin(); it != expr.cend(); /* */)
            {
                if(std::isspace(*it))
                {
                    continue;
                }
                if(std::isdigit(*it))
                {
                    auto peek = it;
                    for(; peek != expr.cend() and !std::isspace(*peek); ++peek);
                    auto num = std::stoi(std::string(it, peek));
                    stk_.push(num);
                    it = peek;

                    std::cout << "reading number "   << num << std::endl;
                    continue;
                }
                if(is_operator(*it))
                {
                    std::cout << "reading operator " << *it << std::endl;
                    auto rhs = get_operand_and_check();
                    auto lhs = get_operand_and_check();
                    stk_.push(evaluate(*it, lhs, rhs));
                    continue;
                }
            }
        }
        return get_result_and_check();
    }
};


}//namespace


///////////////////////////////////////////////////////////////////////////////


int main(int argc, char** argv )
{
    //! test for List
//    ads::List<int> l;
//    for(auto i : {1,2,3,4,5,6}) l.push_front(i);
//    for(;   not l.empty();  l.pop_front())
//        std::cout << l.front() << " ";
//    std::cout << std::endl;


    //! test for stack
//    ads::Stack<int> stk;
//    for(auto i : {1,2,3,4,5,6}) stk.push(i);
//    for(;   not stk.empty();  stk.pop())
//        std::cout << stk.top() << " ";
//    std::cout << std::endl;


    //! test for parser
//    if(argc==2) input_file.open(argv[1]);
//	else { cout<< "cannot read file " << argv[1] << endl; exit(0);}
    if(argc != 2)
        throw std::runtime_error{"cannot read file "};

    ads::RpnParser parser;
    std::cout << parser.parse(argv[1]);

    return 0;
}
