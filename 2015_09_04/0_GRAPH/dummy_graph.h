#include <vector>

namespace dummy {

class graph {

public:

    typedef int T;
    typedef std::vector<T> adjancies;
    typedef std::vector<adjancies> container;

    graph(const size_t& size): list(size) {

    }
    
    inline void resize(const size_t& size) {
        list.resize(size);
    }

    inline void addEdge(const size_t& src, const size_t& dst) {
        list[src].push_back(dst);
    }

    inline adjancies& operator[](const size_t& index) {
        return list[index];
    }

    typedef adjancies::const_iterator edge_it;
    typedef container::const_iterator it;

    inline it begin() const {
        return list.begin();
    }

    inline it end() const {
        return list.end();
    }

    inline edge_it begin(const size_t& v) const {
        return list[v].begin();
    }

    inline edge_it end(const size_t& v) const {
        return list[v].end();
    }

    inline size_t size() const {
        return list.size();
    }

#ifdef DEBUG

    friend std::ostream& operator <<(std::ostream& os, const graph& g) {
        for (it i = g.list.begin(); i != g.list.end(); ++i) {
            os << i - g.list.begin() << ": " << *i << std::endl;
        }
        return os;
    }
    
#endif

private:

   container list;

};

}
