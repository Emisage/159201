#include <iostream>
#include <stdexcept>


namespace ads {

template<typename Data> struct Node
{
    Data value;
    Node* next;
};

template<typename Node>
class List
{
public:
    using DataType = decltype(Node::Data);

    List() = default;

    void push_front(DataType const& new_data)
    {
        do_push_front(new_data);
    }

    DataType const& front()const
    {
        return head_->value;
    }

    void pop()
    {
        do_pop();
    }

    bool empty()const
    {
        return !head_ and !tail_;
    }

private:
    Node* head_{nullptr};
    Node* tail_{nullptr};
    std::size_t size_{0};

    void do_push_front(DataType const& new_data)
    {
        if(empty())
            head_ = tail_   =   new Node{new_data, nullptr};
        else
            head_           =   new Node{new_data, head_};
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
    ads::List<ads::Node<int>> l;
    return 0;
}

