#pragma once
#include <iostream>

#include <vector>
#include <cstdlib>
#include <functional>
#include <utility>

template<typename T> 
struct treap_node {
    typedef treap_node<T>* node;

    T key;
    float priority;
    int size;

    node left;
    node right;
    
    treap_node(const T& _key, const float& _p = rand()):
        key(_key),
        priority(_p),
        size(1),
        left(0),
        right(0) { }

    inline void update() { 
        size = (left ? left->size : 0) + (right ? right->size : 0) + 1;
    }
};

template<typename T>
std::ostream &operator <<(std::ostream& os, const treap_node<T>* node) {
    os << node->key << "-" << node->priority;
    return os;
}

template<typename T> 
struct treap {
    typedef treap_node<T>* node;
    typedef std::pair<node, node> node_pair;

    node root;
    
    treap(): root(NULL) { }
    
    size_t count(const T& x) const;
    int size() const;
    bool empty() const;
    
    bool insert(const T& x, const float& p = rand());
    bool erase(const T& x);
    
    void pop();
    const T& top() const;
    void push(const T& key, const float p);

    static node_pair treap_split(node v, const T& key, bool less = true);
    static node treap_merge(node left, node right);
};

#include <cassert>
#include <cstdio>

template<typename T>
void check(treap_node<T>* v) {
    if (!v) 
        return;
    
    check(v->left);
    check(v->right);

    if (v->left) {
        assert(v->left->key < v->key);
        assert(v->left->priority > v->priority);
    }

    if (v->right) {
        assert(v->key < v->right->key);
        assert(v->right->priority > v->priority);
    }

    assert(v->size == (v->left ? v->left->size : 0) + (v->right ? v->right->size : 0) + 1);
}

template<typename T>
void treap<T>::pop() {
    erase(root->key);
}

template<typename T>
void treap<T>::push(const T& key, const float p) {
    insert(key, p);
}

template<typename T>
const T& treap<T>::top() const {
    return root->key;
}

template<typename T> 
typename treap<T>::node_pair treap<T>::treap_split(node v, const T& key, bool less) {
    if (!v) 
        return node_pair(NULL, NULL);

    bool cmp_result = less ? (!(key < v->key)) : (v->key < key);

    if (cmp_result) {
        node_pair p = treap_split(v->right, key, less);
        v->right = p.first;
        v->update();
        p.first = v;
        return p;
    } else {
        node_pair p = treap_split(v->left, key, less);
        v->left = p.second;
        v->update();
        p.second = v;
        return p;
    }
}

template<typename T> 
typename treap<T>::node treap<T>::treap_merge(node left, node right) {

    if (!left) 
        return right;

    if (!right) 
        return left;

    if (left->priority > right->priority) {
        left->right = treap_merge(left->right, right);
        left->update();
        return left;
    } else {
        right->left = treap_merge(left, right->left);
        right->update();
        return right;
    }
}

template<typename T> 
size_t treap<T>::count(const T& x) const {
    node v = root;
    while (v) {
        if (v->key < x) {
            v = v->right;
        } else if (x < v->key) {
            v = v->left;
        } else {
            return 1;
        }
    }
    return 0;
}

template<typename T> 
bool treap<T>::insert(const T& x, const float& p) {
    if (!root) {
        root = new treap_node<T>(x, p);
        return true;
    }
 
    node_pair pair = treap_split(root, x);
    node l = pair.first;
    
    while (l && l->right)
        l = l->right;
    
    if (l && l->key == x) {
        root = treap_merge(pair.first, pair.second);
        return false;
    }
    
    node vertex = new treap_node<T>(x, p);
    root = treap_merge(pair.first, treap_merge(vertex, pair.second));
    
    return true;
}

template<typename T> 
bool treap<T>::erase(const T& x) {
    if (!root) 
        return false;
    
    node_pair p = treap_split(root, x);
    
    if (!p.first)
        return false;
    
    node_pair q = treap_split(p.first, x, false);
    root = treap_merge(q.first, p.second);
    
    return true;
}

template<typename T> 
inline int treap<T>::size() const {
    return root ? root->size : 0;
}

template<typename T> 
inline bool treap<T>::empty() const {
    return size() == 0;
}
