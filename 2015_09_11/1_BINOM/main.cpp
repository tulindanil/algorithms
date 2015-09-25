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

void top(binomial_queue<int> b_queue, std::priority_queue<int> queue, int count)
{
    while (count)
    {
        if (b_queue.top() != queue.top())
            throw std::runtime_error("top test failed");
        b_queue.pop();
        queue.pop();
        
        count--;
    }
}

void doTest()
{
    for (int i = 7; i <= 2000; i++)
    {
        binomial_queue<int> b_queue;
        std::priority_queue<int> queue;
        
//        try
        {
            fill(b_queue, queue, i);
            top(b_queue, queue, i);
            
            fill(b_queue, queue, 3);
            
            std::cout << "passed :" << i << std::endl;
        }
//        catch (std::exception &e)
        {
//            std::cout << e.what() << std::endl;
        }
    }
    
}

int main()
{
    doTest();
    return 0;
}
