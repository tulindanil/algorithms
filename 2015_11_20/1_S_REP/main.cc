#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <streambuf>
#include <map>
#include <memory>


class finder {

    public:

        finder() {
            vertices.push_back(vertex(0,'$'));
        }

        void addPattern(const std::string& s) {
            int num = 0;
            for (int i = 0; i < s.length(); i++) {
                char ch = s[i] - 'a';
                if (vertices[num].neighbors.count(ch) == 0) {
                    vertices.push_back(vertex(num,ch));
                    vertices[num].neighbors[ch] = vertices.size() - 1;
                }
                num = vertices[num].neighbors[ch];
            }
            vertices[num].ispttrn = true;
            pttrns.push_back(s);
            vertices[num].pttrn_number = pttrns.size() - 1;
        }

        void check(int v, int i) {
            for (int u = v; u != 0; u = getFastLink(u)) {
                if (vertices[u].ispttrn) 
                    std::cout << i - pttrns[vertices[u].pttrn_number].length() << std::endl;
            }
        }

        void findAll(const std::string& s) {
            int u = 0;
            for (int i = 0; i < s.length(); i++) {
                u = getMove(u, s[i]-'a');
                check(u, i + 1);
            }
        }

        int getLink(int v) {
            if (vertices[v].suff_link == -1) {
                if (v == 0 || vertices[v].par == 0) {
                    vertices[v].suff_link = 0;
                } else {
                    vertices[v].suff_link=getMove(getLink(vertices[v].par), vertices[v].symbol);
                }
            }
            return vertices[v].suff_link;
        }

        int getMove(int v, char ch) {
            if (vertices[v].move.count(ch) == 0) {
                if (vertices[v].neighbors.count(ch)) {
                    vertices[v].move[ch] = vertices[v].neighbors[ch];
                } else {
                    if (v == 0)
                        vertices[v].move[ch] = 0;
                    else
                        vertices[v].move[ch] = getMove(getLink(v), ch);
                }
            }
            return
                vertices[v].move[ch];
        }

        int getFastLink(int v) {
            if (vertices[v].suff_flink == -1) {
                int u = getLink(v);
                if (u == 0)
                    vertices[v].suff_flink = 0;
                else
                    vertices[v].suff_flink = (vertices[u].ispttrn) ? u : getFastLink(u);
            }
            return vertices[v].suff_flink;
        }

    private:

        struct vertex;

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
            int suff_flink;
            bool ispttrn;
            char symbol;

            vertex(int p, char c): par(p), symbol(c), suff_link(-1), suff_flink(-1), ispttrn(false) { 

            }

        };
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
