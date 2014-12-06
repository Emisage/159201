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
    void check_ifstream(std::ifstream& ifs) const
    {
        if(! ifs.is_open())
        {
            std::cout << "Could not read file: " << std::endl ;
            exit(0);
        }
    }

    //! II
    void init_pools(std::ifstream& ifs)
    {
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

    //! II
    void fill_rx_pools(std::ifstream& ifs)
    {
        int port = 1;
        for(std::string buff = "expect a line of digits"; ifs.eof(); ++port)
        {
            while(! std::isdigit(buff[0]))    std::getline(ifs, buff);
            std::stringstream line{buff};
            for(std::string token; std::getline(line, token, ' ') && token[0] != '\r'; )
            {
                int dest = std::stoi(token);
                if(dest < 0 or dest > rx_pool_.size() or port > rx_pool_.size())
                {
                    std::cout << "ERROR in text file" << std::endl;
                    exit(0);
                }


            }

        }
    }

    //! I
    void do_construct(std::string const fn)
    {
        std::ifstream ifs{fn};
        check_ifstream(ifs);
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
