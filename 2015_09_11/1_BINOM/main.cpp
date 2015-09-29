#include <iostream>
#include <queue>
#include <algorithm>
#include <numeric>
#include <cstdlib>

#include "binomial_queue.cpp"

void fill(binomial_queue<int> &b_queue, std::priority_queue<int> &queue, int count)
{
    std::vector<int> content(count, 0);
    std::iota(content.begin(), content.end(), 1);
    
    while (content.size())
    {
        int value = content.at(rand() % content.size());
        content.erase(std::find(content.begin(), content.end(), value));
        
        queue.push(value);
        b_queue.push(value);
    }
}

void pop(binomial_queue<int> &b_queue, std::priority_queue<int> &queue, size_t count)
{
    while (count)
    {
        if (b_queue.top() != queue.top() || b_queue.size() != queue.size())
            throw std::runtime_error("top test failed");
        b_queue.pop();
        queue.pop();
        
        count--;
    }
}

void doTest()
{
    for (int i = 1000; i <= 10000; i += 1000)
    {
        binomial_queue<int> b_queue;
        std::priority_queue<int> queue;
        
        try
        {
            fill(b_queue, queue, i);
            pop(b_queue, queue, i/2);
            
            fill(b_queue, queue, i/3);
            pop(b_queue, queue, i/5);
            
            fill(b_queue, queue, i);
            pop(b_queue, queue, b_queue.size());
            
            std::cout << "passed :" << i << std::endl;
        }
        catch (std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    }
    
}

int main()
{
    doTest();
    return 0;
}
