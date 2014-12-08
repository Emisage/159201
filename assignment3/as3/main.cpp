#include <iostream>
#include <fstream>
#include <locale>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>


namespace ads {

/**
 * @brief The Queue class
 *
 * implemented with linked list.
 */
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

    ~Queue()
    {
        do_destructor();
    }

private:
    Node* head_{nullptr};
    Node* tail_{nullptr};
    SizeType size_{0};

    void do_destructor()
    {
        if(! empty())
            for(Node* curr = head_ , *temp; (temp=curr); delete temp)
                curr = curr->next_;
        head_ = tail_ = nullptr;
    }

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
 * generator based on ifstream
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


/**
 * @brief The Pool struct
 *
 * i.e. vector of queues
 */
template<typename T>
struct Pool : public std::vector<ads::Queue<T> >
{
    using Super     =   std::vector<ads::Queue<T> >;
    using SizeType  =   typename Super::size_type;
    void pop_each() //for output queue
    {
        for(auto& q : *this)
            if(! q.empty()) q.leave();
    }


    SizeType total_size() const
    {
        T ret = 0;
        for(auto const& q : *this) ret += q.size();
        return ret;
    }
};

template<typename T>
struct Congestion : public std::vector<T>
{
    typename std::vector<T>::size_type
    total_size()const
    {
        return std::accumulate(this->cbegin(), this->cend(), 0);
    }
};

class Simulator
{
public:
    explicit Simulator(std::string const& fn): rx_{}, tx_{}, peak_{}
    {
        do_constructor(fn);
    }

    void run()
    {
        do_run();
        do_print_result();
    }

private:
    Pool<int> rx_;
    Pool<int> tx_;
    Congestion<std::size_t> peak_;

    //! level I
    std::ostream& do_print_result()const
    {
        for(std::size_t idx=0; idx != tx_.size(); ++ idx)
            std::cout << "output port "     << idx+1        << ": "
                      << peak_[idx]         << " packets"   << std::endl;

        return std::cout;
    }

    //! level I
    void do_constructor(std::string const& fn)
    {
        std::size_t port = 0;
        for(FileReader file{fn}; ! file.is_end(); )
        {
            std::string line = file.read();

            if(line[0] == '#')
                continue;

            if(line[0] == 'P')
            {
                std::stringstream ss{line};
                std::string token;
                std::getline(ss, token, ' ');
                std::getline(ss, token, ' ');
                int num_of_ports = std::stoi(token);

                rx_.resize(num_of_ports);
                tx_.resize(num_of_ports);
                peak_.resize(num_of_ports, 0);
                continue;
            }

            if(std::isdigit(line[0]))
            {
                std::stringstream ss{line};
                for(std::string token; std::getline(ss, token, ' ');  )
                {
                    if(token[0] == '\r') continue;
                    rx_[port].join(std::stoi(token));
                }
                ++port;
                continue;
            }
        }
    }

    //! level I
    void do_run()
    {
        for(std::size_t port=0, clock=0, curr_sum=1; curr_sum > 0; ++port)
        {
            //! rx to tx
            port %= tx_.size();
            if(! rx_[port].empty())
            {
                auto dest = rx_[port].front() - 1;
                rx_[port].leave();
                tx_[dest].join(dest);
            }

            update_tx(++clock);
            curr_sum = update_congestion_record();
        }

    }

    //! level II
    template<typename Clock>
    void update_tx(Clock & clock)
    {
        if(clock % (3 * tx_.size()) == 0 and clock != 0)
            tx_.pop_each();
    }

    //! level II
    std::size_t update_congestion_record()
    {
        if(tx_.total_size() > peak_.total_size())
            for(std::size_t idx=0; idx != tx_.size(); ++idx)
                peak_[idx] = tx_[idx].size();
        return tx_.total_size();
    }
};


}//namespace

int main(int argc, char** argv)
{
    if(argc!=2)
    {
        std::cout<< "Type a file name. " << std::endl << argv[1] << std::endl;
        exit(0);
    }

    ads::Simulator sim{argv[1]};
    sim.run();
    return 0;
}
