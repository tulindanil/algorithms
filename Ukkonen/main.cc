#include <iostream>
#include <cassert>
#include <string>
#include <map>
#include <vector>
#include "SuffixTree.h"

class Solver {

    public:


       static void solve(const std::string& orig, const std::string& current) {
//           Solver s(orig);
       }


    private:

        SuffixTree tree;

//        Solver(const std::string& orig): tree(orig) { }

        std::vector<Result> proceed(const std::string& s) const {
            std::vector<Result> results;
           return results;
        }

};

int main() {
    std::string s1 = "I-love-basketball";
    std::string s2 = "I-love-football";
    SuffixTree tree(s1  + std::string("#") + s2);
    std::cout << s1 << std::endl << s2 << std::endl;
    tree.solve(std::cout);
    return 0;
}
