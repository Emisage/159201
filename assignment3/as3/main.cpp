#include <iostream>
#include <stdexcept>
#include <fstream>
#include <locale>
#include <sstream>
#include <string>
#include <vector>
#include <mutex>
#include <chrono>
#include <thread>


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

class Simulator
{
public:
    using Pool = std::vector<ads::Queue<int> >;

    explicit Simulator(std::string const fn) {   do_construct(fn);  }

    void run()
    {
        do_run();
    }

private:
    Pool rx_pool_{};
    Pool tx_pool_{};


    //! II
    void init_pools(std::ifstream& ifs)
    {
        if(! ifs.is_open())
        {
            std::cout << "Could not read file: " << std::endl ;
            exit(0);
        }

        for(std::string expr; not ifs.eof(); )
        {
            std::getline(ifs, expr);
            std::stringstream sstream{expr};

            if(ifs.eof())       break;
            if(expr[0] == '#')  continue;

            if(expr[0] == 'P')
            {
                std::string token;
                std::getline(sstream, token, ' ');
                std::getline(sstream, token, ' ');
                auto num_of_ports = std::stoi(token);
                rx_pool_.resize(num_of_ports);
                tx_pool_.resize(num_of_ports);
                continue;
            }
        }
    }

    //!
    void fill_rx_pools(std::ifstream& ifs)
    {

    }

    //! I
    void do_construct(std::string const fn)
    {
        std::ifstream ifs{fn};
        init_pools(ifs);
        fill_rx_pools(ifs);
    }

    void do_run()
    {

    }
};

}//namespace

int main()
{
    ads::Simulator simulator{"something"};

    std::cout << "exit\n";
    return 0;
}
