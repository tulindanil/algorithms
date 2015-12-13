#include "Node.h"

class SuffixTree {
    
    Node *currentNode, *root, *blank;
    int stringPosition;
    std::string word;

    void deleteRecursively(Node *node) {
        for (auto child: node->to)
            deleteRecursively(child.second);
        delete node;
    }

    void splitCurrentNode() {
        Node *middleNode = new Node(currentNode->left, stringPosition);
        currentNode->left = stringPosition;
        middleNode->setParent(currentNode->parent, word[middleNode->left]);
        currentNode->setParent(middleNode, word[stringPosition]);
        currentNode = middleNode;
    }

    void jumpDown(int left, int right) {
        if (left >= right)
            return;
        if (stringPosition != currentNode->right)
        {
            int diff = std::min(currentNode->right - stringPosition, right - left);
            stringPosition += diff;
            left += diff;
            jumpDown(left, right);
            return;
        }
        currentNode = currentNode->to[word[left]];
        stringPosition = std::min(currentNode->left + 1, currentNode->right);
        ++left;
        jumpDown(left, right);
    }

    void jumpByLink() {
        if (currentNode->right == stringPosition && currentNode->link != nullptr) {
            currentNode = currentNode->link;
            stringPosition = currentNode->right;
            return;
        }
        int left = currentNode->left;
        int right = stringPosition;
        currentNode = currentNode->parent->link;
        stringPosition = currentNode->right;
        jumpDown(left, right);
    }

    void addLeaf(int leftPosition) {
        Node *leaf = new Node(leftPosition, __INT_MAX__);
        leaf->setParent(currentNode, word[leftPosition]);
    }

    bool canGo(char nextChar) {
        if (stringPosition == currentNode->right)
            return currentNode->to.count(nextChar);
        else
            return word[stringPosition] == nextChar;
    }

    void updateLink(Node *badNode) {
        if (badNode != nullptr)
            badNode->link = currentNode;
    }

    public:

    void appendChar(char nextChar) {
        blank->to[nextChar] = root;
        word.push_back(nextChar);

        Node *badNode = nullptr;
        while (!canGo(nextChar)) {
            if (stringPosition != currentNode->right) {
                splitCurrentNode();
                updateLink(badNode);
                badNode = currentNode;
            } else {
                updateLink(badNode);
                badNode = nullptr;
            }
            addLeaf(word.size() - 1);
            jumpByLink();
        }
        updateLink(badNode);
        jumpDown(word.size() - 1, word.size());
    }

    long long substringsCount() {
        return root->substringsCount(word.size());
    }

    SuffixTree() {
        root = new Node(0, 0);
        currentNode = root;
        blank = new Node(0, 0);
        blank->link = root->link = root->parent = blank;
        stringPosition = 0;
    }

    ~SuffixTree() {
        deleteRecursively(root);
        delete blank;
    }
};

