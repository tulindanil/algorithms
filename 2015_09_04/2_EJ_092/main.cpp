#include <iostream>
#include <stack>
#include "dummy_graph.h"

class Solver {

public:

    static void solve(std::istream& is, std::ostream& os) {
        size_t vertices;
        is >> vertices;
        Solver s = Solver(vertices);
        s.read(is);
        os << s.solve();
    }

private:

    typedef dummy::graph::it iterator;
    typedef dummy::graph::edge_it edge_iterator;

    dummy::graph graph, transposed;
    std::vector<bool> hasAncestor;

    std::vector<bool> isCovered; 

    Solver(size_t size): graph(size), 
                         transposed(size),
                         isCovered(size, false),
                         hasAncestor(size, false) { }

    int solve() {
        size_t count = 0;

        for (iterator it = graph.begin(); it != graph.end(); ++it) {
            size_t vertex = it - graph.begin();
            if (!hasAncestor[vertex]) {
                count++;
                coverWithDFS(graph, vertex);
            }
        }

        for (iterator it = graph.begin(); it != graph.end(); ++it) {
            size_t vertex = it - graph.begin();
            if (!isCovered[vertex]) {
                count++;
                coverWithDFS(transposed, coverWithDFS(graph, vertex));
            }
        }

        return count;
    }

    int coverWithDFS(const dummy::graph& graph, int v) {
        std::vector<bool> visited(graph.size(), false);
        int vertex, latest = 0;

        typedef std::pair<int, edge_iterator> position;

        std::stack<position> stack;
        stack.push(std::make_pair(v, graph.begin(v)));
        isCovered[v] = true;
        visited[v] = true;

        while(!stack.empty()) {
            vertex = stack.top().first;
            if (stack.top().second == graph.end(vertex)) {
                stack.pop();
            } else {
                int neighbour = *stack.top().second;
                ++stack.top().second;
                if (visited[neighbour])
                    continue;
                isCovered[neighbour] = true;
                visited[neighbour] = true;
                latest = neighbour;
                stack.push(std::make_pair(neighbour, graph.begin(neighbour)));
            }
        }
        return latest;
    }

    void read(std::istream& is) {
        size_t edges;
        is >> edges;
        size_t from, to;
        for (size_t i = 0; i < edges; ++i) {
            is >> from >> to;
            hasAncestor[to - 1] = true;
            graph.addEdge(from - 1, to - 1);
            transposed.addEdge(to - 1, from - 1);
        }
    }

};

int main() {
   Solver::solve(std::cin, std::cout);
   return 0;
}
