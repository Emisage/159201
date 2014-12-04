#include <iostream>
#include <stdexcept>


namespace ads {


template<typename T>
class List
{
    struct Node{    T value;    Node* next; };
public:
    List() = default;

    void push_front(T const& new_data)
    {
        do_push_front(new_data);
    }

    T const& front() const
    {
        return head_->value;
    }

    void pop()
    {
        do_pop();
    }

    bool empty() const
    {
        return !head_ and !tail_;
    }

    std::size_t size() const
    {
        return size_;
    }

private:
    Node* head_{nullptr};
    Node* tail_{nullptr};
    std::size_t size_{0};

    void do_push_front(T const& new_value)
    {
        if(empty())
            head_ = tail_   =   new Node{new_value, nullptr};
        else
            head_           =   new Node{new_value, head_};
        ++size_;
    }

    void do_pop()
    {
        if(empty())
            throw std::runtime_error{"underflow!"};
        head_ =head_->next;
        --size_;
    }
};



}//namespace


int main()
{
    ads::List<int> l;
    l.push_front(1);
    l.push_front(42);
    std::cout << l.size() << std::endl;
    std::cout << l.front() << std::endl;

    return 0;
}

