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


struct RpnParser
{
    int parse(std::string const& fn)
    {
       return do_parse_and_evaluate(fn);
    }
private:
    struct Eval{
        bool is_evaluable(char op)  const   {return dic_.find(op) != dic_.cend();}
        int operator ()(char op, int lhs, int rhs)  const   {return dic_.at(op)(lhs, rhs);}
        std::map<char, std::function<int(int,int)> > const dic_ =
        {
            {   '+', [](int lhs, int rhs){  return lhs + rhs;}  },
            {   '-', [](int lhs, int rhs){  return lhs - rhs;}  },
            {   '*', [](int lhs, int rhs){  return lhs * rhs;}  },
            {   '/', [](int lhs, int rhs){  return lhs / rhs;}  },
        };
    }
    const eval_;
    ads::Stack<int> stk_;

    //! abstraction II
    int get_operand_and_check() throw()
    {
        if(stk_.empty())
            throw std::logic_error{"too many operators!"};
        auto operand = stk_.top();
        stk_.pop();
        return operand;
    }

    //! abstraction II
    int get_result_and_check() throw()
    {
        if(1 != stk_.size())
            throw std::logic_error{"too many numbers!"};
        auto result = stk_.top();
        stk_.pop();
        return result;
    }

    //! abstraction II
    void check_file(std::ifstream& ifs) throw()
    {
        if(!ifs.good())
            throw std::runtime_error{"Bad file name"};
    }

    //! abstraction I
    int do_parse_and_evaluate(std::string const& fn)
    {
        std::ifstream ifs{fn};
        check_file(ifs);
        for(std::string expr; !ifs.eof(); )
        {
            std::getline(ifs, expr);
            auto it = expr.cbegin();
            for( ; it != expr.cend()  and  std::isspace(*it); ++it);
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
            if(eval_.is_evaluable(*it))
            {
                std::cout << "reading operator " << *it << std::endl;
                auto rhs = get_operand_and_check();
                auto lhs = get_operand_and_check();
                stk_.push(eval_(*it, lhs, rhs));
                continue;
            }
        }
        return get_result_and_check();
    }
};


}//namespace


///////////////////////////////////////////////////////////////////////////////


int main( int argc, char** argv )
{
    try
    {
        if(argc != 2)
            throw std::runtime_error{"Cannot read file.\n"};
    }
    catch (std::runtime_error e)
    {
        std::cerr << e.what() << "Programme terminated.\n";
        return -1;
    }

    auto result = ads::RpnParser{}.parse(argv[1]);
    std::cout << "The result is " << result << std::endl;

    return 0;
}
