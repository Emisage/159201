#include <iostream>
#include <stdexcept>


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

}//namespace


int main()
{
    //! test for List
    ads::List<int> l;
    for(auto i : {1,2,3,4,5,6}) l.push_front(i);
    for(;   not l.empty();  l.pop_front())
        std::cout << l.front() << " ";
    std::cout << std::endl;

    //! test for stack
    ads::Stack<int> stk;
    for(auto i : {1,2,3,4,5,6}) stk.push(i);
    for(;   not stk.empty();  stk.pop())
        std::cout << stk.top() << " ";
    std::cout << std::endl;

    return 0;
}

//! output  :
//!
//6 5 4 3 2 1
//6 5 4 3 2 1
