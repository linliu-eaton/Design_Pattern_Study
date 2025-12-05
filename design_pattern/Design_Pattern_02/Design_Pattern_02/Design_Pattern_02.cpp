#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <optional>
#include <stdlib.h>

namespace
{
    std::mutex g_mutex;

    class bufferQueue
    {
    public:
        bufferQueue() = default;
        virtual ~bufferQueue() = default;

        bool full()
        {
            size_t sz = size();
            return sz >= MAX_QUEUE_SZIE;
        }

        bool empty()
        {
            size_t sz = size();
            return sz == 0;
        }

        void push(int elem)
        {
            size_t sz = size();
            if (sz <= MAX_QUEUE_SZIE) {
                m_que.push(elem);
            }
        }

        std::optional<int> pop()
        {
            size_t sz = size();
            if (sz != 0) {
                int elem = m_que.front();
                m_que.pop();
                std::optional<int> ret{ elem };
                return ret;
            }
            return std::nullopt;
        }

    private:
        size_t size()
        {
            return m_que.size();
        }

    private:
         static constexpr size_t MAX_QUEUE_SZIE{ 50 };
         std::queue<int> m_que;
    };

    static bufferQueue g_queue{};

    void producer(int id)
    {
        {
            std::lock_guard<std::mutex> lck(g_mutex);
            std::cout << "producer " << id << " created! -------" << std::endl;
        }
        
        while (1)
        {
            std::lock_guard<std::mutex> lck(g_mutex);

            int rd = rand() % 10000 + id * 10000;
            if (!g_queue.full()) {
                g_queue.push(rd);
                std::cout << "producer " << id << " push data " << rd << std::endl;
            }
            else
            {
                std::cout << "queue full, producer " << id << " discard data " << rd << std::endl;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(200));  // construct queue full/empty
        }
    }

    void consumer(int id)
    {
        {
            std::lock_guard<std::mutex> lck(g_mutex);
            std::cout << "consumer " << id << " created! -------" << std::endl;
        } 

        while (1)
        {
            std::lock_guard<std::mutex> lck(g_mutex);

            if (!g_queue.empty()) {
                auto val = g_queue.pop();
                if (val) {
                    std::cout << "consumer " << id << " consum data " << val.value() << std::endl;
                }
            }
            else
            {
                std::cout << "queue empty, consumer " << id << " not pop anyting." << std::endl;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(200));  // construct queue full/empty
        }
    }

}

class Producer
{
public:
    explicit Producer(int n)
        :m_pnum{ n }
    {
        for (int i = 1; i <= m_pnum; i++) {
            m_prodVec.push_back(std::thread(producer, i));
        }
    }
    virtual ~Producer() = default;

    void join()
    {
        for (auto &th : m_prodVec) {
            th.join();
        }
    }

private:
    int m_pnum{ 0 };
    std::vector<std::thread> m_prodVec{};
};

class Consumer
{
public:
    explicit Consumer(int n)
        :m_cnum{ n }
    {
        for (int i = 1; i <= m_cnum; i++) {
            m_consVec.push_back(std::thread(consumer, i));
        }
    }
    virtual ~Consumer() = default;

    void join()
    {
        for (auto &th : m_consVec) {
            th.join();
        }
    }

private:
    int m_cnum{ 0 };
    std::vector<std::thread> m_consVec{};
};


int main()
{
    std::cout << "----------------main 1----------------\n";
    Producer producer(1);
    Consumer consumer(5);
    consumer.join();
    producer.join();

    return 0;
}

