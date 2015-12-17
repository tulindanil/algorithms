#include <algorithm>
#include <cmath>

class Node {

public:

    int left, right;
    std::map<char, Node *> to;
    Node *parent, *link;
    bool isLeaf;

    size_t matching;

    Node(int left, int right): 
        left(left), 
        right(right),
        parent(nullptr), 
        link(nullptr) { }

    void setParent(Node *newParent, char firstChar) {
        if (newParent != nullptr)
            newParent->to[firstChar] = this;
        parent = newParent;
    }

    long long count(int length) {
        long long count = std::min(length, right) - left;
        for (auto node: to)
            count += node.second->count(length);
        return count;
    }

    std::string getContent(const std::string& s) const {
        return s.substr(left, right - left);
    }

   void proceed(const std::string& s, size_t length) {
        matching = length;
        right = std::min(right, (int)s.length());
        for(auto node: to) {
            auto child = node.second; 
            child->proceed(s, length + (right - left));
        }
    } 
    int id() const {
        return (long int)this - ((long int)this / 10000) * 10000;
    }
    
    void print(std::ostream& os, const std::string& s, int offset = 0) const {
        os << std::string(offset, '*'); 
        std::string content = getContent(s); 
        os << content << (isLeaf ? "(leaf) " : " ") << matching << " " << id() << " " << link->id() << std::endl; 
        for (auto it: to) {
            it.second->print(os, s, offset + content.length());
        }
    }

};
