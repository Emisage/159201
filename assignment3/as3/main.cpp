#include <iostream>
#include <stdexcept>

namespace ads {

template<typename T>
class Queue
{
    struct Node
    {
        T data_;
        Node* next_;
    };

public:
    using SizeType = std::size_t;

    void join(T t)
    {
        do_join(std::move(t));
    }

    void leave()
    {
        do_leave();
    }

    T const& front() const
    {
        return head_->data_;
    }


    SizeType size()const
    {
        return size_;
    }

    bool empty()const
    {
        return !head_ and !tail_ ;
    }

private:
    Node* head_{nullptr};
    Node* tail_{nullptr};
    SizeType size_{0};

    void do_join(T&& t)
    {
        if(empty())
            head_ = tail_        = new Node{std::move(t), nullptr};
        else
            tail_ = tail_->next_ = new Node{std::move(t), nullptr};
        ++size_;
    }

    void do_leave()
    {
        if(size_ == 0)
            throw std::runtime_error{"underflow!"};

        if(size_ == 1)
        {
            delete head_;
            head_ = tail_ = nullptr;
        }
        else
        {
            auto tmp = head_;
            head_ = head_->next_;
            delete tmp;
        }
        --size_;
    }
};

}//namespace

int main()
{
    struct Packet{};
    using Data = std::pair<int, Packet>;
    ads::Queue<Data> q;
    for(auto && data : { Data{42,Packet{}}, Data{99,Packet{}} } )
        q.join(std::move(data));

    std::cout << "test for asd::Queue:\n";
    for(;   not q.empty();  q.leave())
        std::cout << q.front().first << " size=" << q.size() << std::endl;
    std::cout << q.empty() << std::endl;

    std::cout << "exit\n";
    return 0;
}
//! test for Queue, output:
//!
//test for asd::Queue:
//42 size=2
//99 size=1
//1
//exit
