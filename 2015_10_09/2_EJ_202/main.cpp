#include <iostream>
#include <vector>

size_t partitions(int n, int k, std::vector<std::vector<int> > &matrix)
{
    if (matrix[n][k] != -1)
        return matrix[n][k];
    
    if (n == 0 && k == 0)
    {
        matrix[n][k] = 1;
        return 1;
    }
    
    if (k == 0)
    {
        matrix[n][k] = 0;
        return 0;
    }
    
    if (k <= n)
    {
        matrix[n][k] = partitions(n, k - 1, matrix) + partitions(n - k, k, matrix);
        return matrix[n][k];
    }
    else
    {
        matrix[n][k] = partitions(n, n, matrix);
        return matrix[n][k];
    }
}

int main()
{
    size_t price = 0;
    std::cin >> price;
    
    std::vector<std::vector<int> > matrix = std::vector<std::vector<int> >(price + 1, std::vector<int>(price + 1, -1));
    std::cout << partitions(price, price, matrix);
    
    return 0;
}