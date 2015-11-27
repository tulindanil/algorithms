class finder {

    public:

        finder(): root(0) {
            vertices.push_back(vertex(root,'#'));
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
            pttrns.push_back(s.length());
            vertices[num].pttrn_number = pttrns.size() - 1;
        }

        void findAll(const std::string& s) {
            int current = 0;
            for (int i = 0; i < s.length(); i++) {
                current = getMove(current, s[i]);
                for (int k = current; k != 0; k = getSuffixLink(k)) {
                    if (vertices[k].ispttrn) { 
                        std::cout << i - pttrns[vertices[k].pttrn_number] + 1 << std::endl;
                    }
                }
            }
        }

        friend std::ostream& operator <<(std::ostream& os, const finder& f) {
            os << f.vertices.size();
            return os;
        }

    private:

        struct vertex;
        int root;

        std::vector<vertex> vertices;
        std::vector<size_t> pttrns;

        struct vertex {

            typedef std::map<char, int> map;
            typedef std::shared_ptr<vertex> reference;

            map neighbors;
            map move;
            int pttrn_number;
            int link;
            int parent;
            bool ispttrn;
            char symbol;

            vertex(int p, char c): parent(p), symbol(c), link(-1), ispttrn(false) { 

            }

            friend std::ostream& operator <<(std::ostream& os, const vertex& v) {
                os << "vertex" << std::endl;
                os << v.neighbors << std::endl << v.move;
                return os;
            }
        };

        int getSuffixLink(int v) {
            if (vertices[v].link == -1) {
                if (v == root || vertices[v].parent == root) {
                    vertices[v].link = root;
                } else {
                    vertices[v].link = getMove(getSuffixLink(vertices[v].parent), vertices[v].symbol);
                }
            }
            return vertices[v].link;
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
            return vertices[v].move[ch];
        }
};


