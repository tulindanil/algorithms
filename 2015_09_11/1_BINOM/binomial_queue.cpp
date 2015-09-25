#include <set>
#include <map>
#include <vector>
#include <cassert>
#include <exception>

#include "binomial_tree.cpp"

template <typename T>
struct TreeReferenceFunctor
{
    bool operator()(const typename binomial_tree<T>::tree_reference &lhs, const typename binomial_tree<T>::tree_reference &rhs) const
    {
        return lhs->size() < rhs->size();
    }
};

template <typename T>
class binomial_queue
{

public:
    
    typedef T value_type;
    typedef size_t size_type;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    
    typedef typename binomial_tree<value_type>::tree_reference tree_reference;
    
    binomial_queue();
    binomial_queue(const binomial_queue<T> &queue);
    
    inline const_reference top() const;
    
    inline bool empty() const;
    inline size_type size() const;
    
    void push(const T &value);
    void push(T &&value);
    
    void pop();

private:

    void check() const;
    
    tree_reference top_tree;
    size_type qty;
    std::set<tree_reference, TreeReferenceFunctor<T>> trees;
    
    void mergeTrees(tree_reference existingTree, tree_reference treeToAdd);
    
    void resolveEqualTrees(tree_reference existingTree, tree_reference treeToAdd);
    void insertTree(tree_reference tree);
    
    void addTreeNotEmpty(tree_reference tree);
    void addTree(tree_reference);
    
    void setTop(tree_reference tree);
    void updateTop();

};

template <typename T>
binomial_queue<T>::binomial_queue(): top_tree(new binomial_tree<T>(T())), qty(0)
{
    
}

template <typename T>
binomial_queue<T>::binomial_queue(const binomial_queue<T> &queue): qty(queue.qty), top_tree(new binomial_tree<T>(*queue.top_tree))
{
    for (auto tree: trees)
        trees.erase(tree);
    
    for (auto tree: queue.trees)
        trees.insert(tree_reference(new binomial_tree<T>(*tree)));
}

#pragma mark - Push

template <typename T>
void binomial_queue<T>::insertTree(tree_reference tree)
{
    if(trees.find(tree) != trees.end())
        throw std::out_of_range("Hey, there is no two size equals tree in heap!");
    
    if (trees.empty())
        top_tree = tree;
    
    trees.insert(tree);
}

template <typename T>
void binomial_queue<T>::mergeTrees(tree_reference merging, tree_reference merged)
{
    trees.erase(merging);
    merging->mergeWithTree(merged);
    addTree(merging);
}

template <typename T>
void binomial_queue<T>::resolveEqualTrees(tree_reference existingTree, tree_reference treeToAdd)
{
    if (treeToAdd->top() > existingTree->top())
        mergeTrees(treeToAdd, existingTree);
    else
        mergeTrees(existingTree, treeToAdd);
}

template <typename T>
void binomial_queue<T>::addTreeNotEmpty(tree_reference tree)
{
    if (trees.find(tree) == trees.end())
        insertTree(tree);
    else
        resolveEqualTrees(*trees.find(tree), tree);
}

template <typename T>
void binomial_queue<T>::addTree(tree_reference treeToAdd)
{
    if (empty() == false)
        addTreeNotEmpty(treeToAdd);
    else
        insertTree(treeToAdd);
}

template <typename T>
void binomial_queue<T>::push(const T& value)
{
    qty++;
    tree_reference treeToAdd(new binomial_tree<T>(value));
    addTree(treeToAdd);
    updateTop();
    check();
}

#pragma mark - Pop

template <typename T>
void binomial_queue<T>::pop()
{
    if (empty())
        std::runtime_error("queue is already empty");
    qty--;
    
    trees.erase(top_tree);
    for (auto child: top_tree->split())
        addTree(child);
    
    updateTop();
    check();
}

#pragma mark - Top

template <typename T>
void binomial_queue<T>::setTop(tree_reference tree)
{
    if (top_tree == nullptr || top_tree->top() < tree->top())
        top_tree = tree;
}

template <typename  T>
void binomial_queue<T>::updateTop()
{
    top_tree = nullptr;
    for (auto tree: trees)
        setTop(tree);
}

template <typename T>
typename binomial_queue<T>::const_reference binomial_queue<T>::top() const
{
    if (trees.find(top_tree) != trees.end())
        return top_tree->top();
    else
        throw std::runtime_error("Nope");
}

#pragma mark - Size

template <typename T>
typename binomial_queue<T>::size_type binomial_queue<T>::size() const
{
    return qty;
}

template <typename T>
bool binomial_queue<T>::empty() const
{
    return trees.empty();
}

#pragma mark - Checking

template <typename  T>
void binomial_queue<T>::check() const
{
    int size = 0;
    std::map<size_t, binomial_tree<T>> size_map;
    for (auto tree: trees)
    {
        size_map[tree->size()] = tree;
        size += tree->size();
    }
    
    if (size != qty || size_map.size() != trees.size())
        throw std::runtime_error("bad size");
}
