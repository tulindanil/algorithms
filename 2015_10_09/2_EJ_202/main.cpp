#include <iostream>

size_t partitions(int n, int k)
{
    if (n == 0 && k == 0)
        return 1;
    
    if (k == 0)
        return 0;
    
    if (k <= n)
        return partitions(n, k - 1) + partitions(n - k, k);
    else
        return partitions(n, n);
}

int main()
{
    size_t price = 0;
    std::cin >> price;
    
    std::cout << partitions(price, price);
    return 0;
}