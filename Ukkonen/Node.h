class Node {

public:

    int left, right;
    std::map<char, Node *> to;
    Node *parent, *link;

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

    long long substringsCount(int length) {
        long long count = std::min(length, right) - left;
        for (auto node: to)
            count += node.second->substringsCount(length);
        return count;
    }

};
