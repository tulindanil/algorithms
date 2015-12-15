#include <iostream>
#include <set>
#include <utility>
#include "wrapper.cc"

#include "btree.h"

class btree: public stx::btree_set<std::pair<float, int> > {

public:

   typedef std::pair<float, int> T;

   void push(const T& key) {
       insert(key);
   }

   int top() const {
       return begin()->second;
   }

   void pop() {
        erase(begin());
   }

   void replace(const T& oldKey, const T& newKey) {
       erase(oldKey);
       insert(newKey);
   }

};

int main() {
    wrap<btree>();
}
