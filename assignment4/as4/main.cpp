#include <iostream>

namespace ads {

template<typename T>
class List
{
    struct Node
    {
        T value_;
        Node* prev_;
        Node* next_;
    };

public:

    void push_front(T const& new_value)
    {
        if(empty())
        {
            head_ = tail_   =   new Node{new_value, nullptr, nullptr};
        }
        else
        {
            head_ = new Node{new_value, nullptr, head_};
            head_->next_->prev_ = head_;
        }
        ++size_;
    }


    bool empty()const
    {
        return !head_ and !tail_;
    }

    std::size_t size() const
    {
        return size_;
    }

private:
    Node* head_{};
    Node* tail_{};
    std::size_t size_{0};
};

}


int main()
{
    return 0;
}

