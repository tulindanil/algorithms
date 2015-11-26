#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <streambuf>
#include <map>
#include <memory>


class finder {

    public:

        finder(): root(0) {
            vertices.push_back(vertex(0,'#'));
        }

        void addPattern(const std::string& s) {
            int num = 0;
            for (int i = 0; i < s.length(); i++) {
                char ch = s[i];
                if (vertices[num].neighbors.count(ch) == 0) {
                    vertices.push_back(vertex(num, ch));
                    vertices[num].neighbors[ch] = vertices.size() - 1;
                }
                num = vertices[num].neighbors[ch];
            }
            vertices[num].ispttrn = true;
            pttrns.push_back(s);
            vertices[num].pttrn_number = pttrns.size() - 1;
        }

        void findAll(const std::string& s) {
            int current = 0;
            for (int i = 0; i < s.length(); i++) {
                current = getMove(current, s[i]);
                for (int k = current; k != 0; k = getSuffixLink(k)) {
                    if (vertices[k].ispttrn) 
                        std::cout << i - pttrns[vertices[k].pttrn_number].length() - 1 << std::endl;
                }
            }
        }

    private:

        struct vertex;
        int root;

        std::vector<vertex> vertices;
        std::vector<std::string> pttrns;

        struct vertex {

            typedef std::map<char, int> map;
            typedef std::shared_ptr<vertex> reference;

            map neighbors;
            map move;
            int pttrn_number;
            int suff_link;
            int par;
            bool ispttrn;
            char symbol;

            vertex(int p, char c): par(p), symbol(c), suff_link(-1), ispttrn(false) { 

            }

        };

        int getSuffixLink(int v) {
            if (vertices[v].suff_link == -1) {
                if (v == root || vertices[v].par == root) {
                    vertices[v].suff_link = root;
                } else {
                    vertices[v].suff_link = getMove(getSuffixLink(vertices[v].par), vertices[v].symbol);
                }
            }
            return vertices[v].suff_link;
        }

        int getMove(int v, char ch) {
            if (vertices[v].move.count(ch) == 0) {
                if (vertices[v].neighbors.count(ch)) {
                    vertices[v].move[ch] = vertices[v].neighbors[ch];
                } else {
                    if (v == root)
                        vertices[v].move[ch] = root;
                    else
                        vertices[v].move[ch] = getMove(getSuffixLink(v), ch);
                }
            }
            return
                vertices[v].move[ch];
        }
};

int main() {
    finder f;
    std::string filepath;
    std::cin >> filepath;
    size_t q = 0;
    std::cin >> q;
    for (int i = 0; i < q; i++) {
        std::string s;
        std::cin >> s;
        f.addPattern(s);
    }
    std::ifstream file(filepath);
    std::string str;
    std::string file_contents;
    while (std::getline(file, str)) {
        file_contents += str;
        file_contents.push_back('\n');
    }
    f.findAll(file_contents);   
}
