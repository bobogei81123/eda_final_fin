#include "global.h"
#include "parser.h"
#include "solver.h"

using namespace std;

using Parser::circuit;

template <typename T2, typename T1, typename Functor>
vector<T2> foreach(const T1& c, Functor &&f)
{
    vector<T2> ret;
    std::transform(std::begin(c), std::end(c), inserter(ret, ret.begin()), f);
    return ret;
}

int main(int argc, char* argv[]){
    ASSERT(argc == 4);
    Parser::parse(argv[1]);
    //circuit.print();
    Solver::solve(argv[2], argv[3]);
    return 0;
}

