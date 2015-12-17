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

        std::list<Result> proceed(const std::string& s) {
            std::list<Result> results;
            for (auto it: root->to) {
              if (it.second == fiction) 
                 continue;
              it.second->proceed(s, 0);
            }

            Node* current = root;
            Node* next = nullptr;
            Result result;
            size_t offset = 0, length = 0;
            char ch = s[0];
            for (auto it = s.begin(); it != s.end(); ++it) {
                if (offset == 0) {
                    if (current->to.count(*it) == 0) {
                        if (current == root) {
                            length = 0;
                            next = nullptr;
                        } else {
                            length--;
                            it--;
                            current = current->link;
                            next = next->link;
                            continue;
                        }
                    } else {
                        ch = *it;
                        next = current->to[*it];
                        offset++;
                        length++;
                    }
                } else {
                    size_t edge_length = next->right - next->left;
                    if (next->getContent(pttrn)[offset] == *it) {
                        length++;
                        offset++;
                        if (offset >= edge_length) {
                           offset = 0;
                           current = next;
                           next = nullptr;
                        }
                    } else {
                        if (current == root) {
                            length = 0;
                            next = nullptr;
                        } else {
                            length--;
                            it--;
                            current = current->link;
                            next = current->to[ch];
                            continue;
                        }
                    }
               }
               std::cout << length << " | " << current->matching + offset << std::endl;
            }
            return results;
        }

        long long count() {
            return root->count(pttrn.size());
        }

        SuffixTree(): root(new Node(0, 0)), currentNode(root), fiction(new Node(0, 0)), pos(0) {
            fiction->link = root->link = root->parent = fiction;
        }

        SuffixTree(const std::string& s): SuffixTree() {
            for (auto it: s) {
                push_back(it);
            }
        }

        ~SuffixTree() {
            cleanUp(root);
            delete fiction;
        }

        friend std::ostream& operator <<(std::ostream& os, const SuffixTree& tree) {
            for (auto it: tree.root->to) {
                if (it.second == tree.fiction) 
                    continue;
                it.second->print(os, tree.pttrn);
            }
            return os;
        }

    private:

        Node *root, *currentNode, *fiction;
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
            } else if (pos == currentNode->right) {
                setCurrentNode(currentNode->to[pttrn[left]]);
                setPos(std::min(currentNode->left + 1, currentNode->right));
                ++left;
            }
            jumpDown(left, right);
        }

        void jumpByLink() {
            if (currentNode->right == pos && currentNode->link != nullptr) {
                setCurrentNode(currentNode->link);
                setPos(currentNode->right);
            } else {
                int left = currentNode->left;
                int right = pos;
                setCurrentNode(currentNode->parent->link);
                setPos(currentNode->right);
                jumpDown(left, right);
            }
        }

        inline void addLeaf(int leftPosition) {
            Node *leaf = new Node(leftPosition, INT_MAX);
            leaf->isLeaf = true;
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
