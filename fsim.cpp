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


bool file_exists(const char* name) {
    if (FILE *file = fopen(name, "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
}

int main(int argc, char* argv[]){
    ASSERT(argc == 5);

    for (int i=1; i<=4; i++) {
        if (not file_exists(argv[i])) {
            cout << "File " << argv[i] << " doesn't exists!" << endl;
            ASSERT(false);
        }
    }

    Parser::parse(argv[1]);
    circuit.print_desc();
    Solver::solve(argv[2], argv[3], argv[4]);
    return 0;
}
