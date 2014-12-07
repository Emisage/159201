#include <iostream>
#include <fstream>
#include <locale>
#include <sstream>
#include <string>
#include <vector>
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

/**
 * @brief The FileReader class
 *
 * RAII for ifstream
 */
class FileReader
{
public:
    FileReader(std::string fn): ifs_{fn}
    {
        if(!ifs_.good())
            throw std::runtime_error{"can not open file"};
    }

    bool is_end() const
    {
        return ifs_.eof();
    }

    std::string read()
    {
        std::string line;
        std::getline(ifs_, line);
        return line;
    }

private:
    std::ifstream ifs_;
};


}//namespace

int main()
{
    std::cout << "exit\n";
    return 0;
}
