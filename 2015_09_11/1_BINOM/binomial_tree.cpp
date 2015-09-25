#include <memory>
#include <vector>
#include <exception>
#include <iostream>

template <typename T>
class vertex
{

    T value;

public:
    
    vertex(const T &value): value(value), index(1)
    {

    }

    size_t index;
    std::vector<std::shared_ptr<vertex<T>>> children;
    inline const T &getValue() const;

};

template <typename T>
const T &vertex<T>::getValue() const
{
    return value;
}

template <typename T>
class binomial_tree
{

public:

    binomial_tree();
    binomial_tree(const vertex<T> &v);
    binomial_tree(const binomial_tree<T> &tree);
    
    typedef T value_type;
    typedef size_t size_type;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    
    typedef std::shared_ptr<binomial_tree<value_type>> tree_reference;
    
    inline const_reference top() const;
    
    inline bool empty() const;
    inline size_type size() const;
    
    void operator =(const tree_reference rhs);
    bool operator ==(const tree_reference rhs) const;
    
    void mergeWithTree(const tree_reference tree);

    std::vector<tree_reference> split();
    
    template<class U>
    friend bool operator<(const tree_reference &lhs, const tree_reference &rhs);
    
private:

    std::shared_ptr<vertex<T>> root;
    size_t qty;

};

template <typename T>
binomial_tree<T>::binomial_tree(): qty(1), root(new vertex<T>(T()))
{
    
}

template <typename T>
binomial_tree<T>::binomial_tree(const binomial_tree<T> &tree): qty(tree.qty), root(tree.root)
{
    
}

template <typename T>
binomial_tree<T>::binomial_tree(const vertex<T> &v): qty(1), root(new vertex<T>(v))
{
    
}

template <typename T>
void binomial_tree<T>::mergeWithTree(const tree_reference tree)
{
    if (size() != tree->size())
        throw std::out_of_range("You are not allowed to merge no size equavilent trees");
    root->children.push_back(std::shared_ptr<vertex<T>>(tree->root));
    tree->root->index = tree->qty;
    qty += tree->qty;
    root->index = qty;
}

#pragma mark - Pop

template <typename T>
std::vector<typename binomial_tree<T>::tree_reference> binomial_tree<T>::split()
{
    std::vector<tree_reference> splitted;
    for (auto vertex: root->children)
    {
        tree_reference tree(new binomial_tree<T>(*vertex));
        tree->qty = vertex->index;
        splitted.push_back(tree);
    }
    return splitted;
}

#pragma mark - Top

template <typename T>
typename binomial_tree<T>::const_reference binomial_tree<T>::top() const
{
    return root->getValue();
}

template <typename T>
typename binomial_tree<T>::size_type binomial_tree<T>::size() const
{
    return qty;
}

template <typename T>
bool binomial_tree<T>::empty() const
{
    return !qty;
}

template <typename T>
void binomial_tree<T>::operator =(const tree_reference rhs)
{
    root = rhs->root;
    qty = rhs->qty;
}

template <typename T>
bool binomial_tree<T>::operator ==(const tree_reference rhs) const
{
    return qty == rhs.qty;
}
