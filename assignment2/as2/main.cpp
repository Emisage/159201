#include <iostream>
#include <stdexcept>


namespace ads {


template<typename T>
class List
{
    struct Node{    T value;    Node* next; };
public:
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



}//namespace


int main()
{
    ads::List<int> l;
    for(auto i : {1,2,3,4,5,6}) l.push_front(i);
    for(;   not l.empty();  l.pop_front())
        std::cout << l.front() << std::endl;

    return 0;
}

