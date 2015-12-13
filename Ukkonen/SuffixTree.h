#include "Node.h"

class SuffixTree {

    public:

        void push_back(char nextChar) {
            fiction->to[nextChar] = root;
            pttrn.push_back(nextChar);

            Node *previousOne = nullptr;
            while (!canGo(nextChar)) {
                if (pos != currentNode->right) {
                    split();
                    updateLink(previousOne);
                    previousOne = currentNode;
                } else {
                    updateLink(previousOne);
                    previousOne = nullptr;
                }
                addLeaf(pttrn.size() - 1);
                jumpByLink();
            }
            updateLink(previousOne);
            jumpDown(pttrn.size() - 1, pttrn.size());
        }

        long long count() {
            return root->count(pttrn.size());
        }

        SuffixTree(): root(new Node(0, 0)), fiction(new Node(0, 0)), pos(0) {
            currentNode = root;
            fiction->link = root->link = root->parent = fiction;
        }

        ~SuffixTree() {
            cleanUp(root);
            delete fiction;
        }

    private:

        Node *currentNode, *root, *fiction;
        int pos;
        std::string pttrn;

        inline void setPos(int p) {
            pos = p;
        }

        inline void setCurrentNode(Node *n) {
            currentNode = n;
        }

        void cleanUp(Node *node) {
            for (auto child: node->to)
                cleanUp(child.second);
            delete node;
        }

        inline void split() {
            Node *middleNode = new Node(currentNode->left, pos);
            currentNode->left = pos;
            middleNode->setParent(currentNode->parent, pttrn[middleNode->left]);
            currentNode->setParent(middleNode, pttrn[pos]);
            setCurrentNode(middleNode);
        }

        void jumpDown(int left, int right) {
            if (left >= right)
                return;
            if (pos != currentNode->right) {
                int diff = std::min(currentNode->right - pos, right - left);
                pos += diff;
                left += diff;
                jumpDown(left, right);
                return;
            }
            setCurrentNode(currentNode->to[pttrn[left]]);
            setPos(std::min(currentNode->left + 1, currentNode->right));
            ++left;
            jumpDown(left, right);
        }

        void jumpByLink() {
            if (currentNode->right == pos && currentNode->link != nullptr) {
                setCurrentNode(currentNode->link);
                setPos(currentNode->right);
                return;
            }
            int left = currentNode->left;
            int right = pos;
            setCurrentNode(currentNode->parent->link);
            setPos(currentNode->right);
            jumpDown(left, right);
        }

        inline void addLeaf(int leftPosition) {
            Node *leaf = new Node(leftPosition, __INT_MAX__);
            leaf->setParent(currentNode, pttrn[leftPosition]);
        }

        inline bool canGo(char nextChar) {
            if (pos == currentNode->right) {
                return currentNode->to.count(nextChar);
            } else {
                return pttrn[pos] == nextChar;
            }
        }

        inline void updateLink(Node *node) {
            if (node != nullptr) {
                node->link = currentNode;
            }
        }

};
