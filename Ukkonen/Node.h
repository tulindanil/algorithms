#include <algorithm>

class Node {

public:

    int left, right;
    std::map<char, Node *> to;
    Node *parent, *link;
    bool isLeaf;

    int value;

    int length;

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

    std::pair<int, int> proceed(const std::string& s, int divider) {
        std::string content = s.substr(left, right - left);
        int pos = content.find("#");
        if (pos != -1) {
            right = left + pos;
        }
        if (isLeaf) {
            value = (left >= divider) ? 2 : 1;
            return {value, 0}; 
        }
        value = 0;
        length = 0;
        for(auto node: to) {
            auto p = node.second->proceed(s, divider);
            if (p.first == 3) {
                length = std::max(p.second + (right - left), length);
            }
            value |= p.first;
        }
        if (value == 3 && length == 0) {
            length = right - left;
        }
        return {value, length};
    }
    
    void print(std::ostream& os, const std::string& s, int offset = 0) const {
        os << std::string(offset, '*'); 
        std::string content = s.substr(left, right - left);
        os << content << (isLeaf ? "(leaf) " : " ") << value << " " << length << std::endl; 
        for (auto it: to) {
            it.second->print(os, s, offset + content.length());
        }
    }

};
