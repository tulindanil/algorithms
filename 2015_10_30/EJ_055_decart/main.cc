#include <iostream>
#include <utility>
#include "treap.cc" 
#include "wrapper.cc"

class queued_set: public treap<int> {

public:

   typedef std::pair<float, int> T;

   void push(const T& key) {
       insert(key.second, key.first);
   }

   int top() const {
       return treap<int>::top();
   }

   void pop() {
        treap<int>::pop(); 
   }

   void replace(const T& oldKey, const T& newKey) {
       erase(oldKey.second);
       push(newKey);
   }

};

int main() {
    wrap<queued_set>();
}
