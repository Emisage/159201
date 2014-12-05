#include <iostream>

namespace ads {

template<typename T>
class Queue
{
    struct Node
    {
        T data_;
        Node* nest_;
    };
public:
    using SizeType  =   std::size_t;

    void join()
    {

    }

    void leave()
    {

    }

    T const& front()
    {

    }


    SizeType size()const
    {
        return size_;
    }

    bool empty()const
    {

    }

private:
    Node* head_{nullptr};
    Node* tail_{nullptr};
    SizeType size_{0};
};

}//namespace

int main()
{
    return 0;
}

