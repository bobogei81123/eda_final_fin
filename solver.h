#ifndef SOLVER_H_GAURD
#define SOLVER_H_GAURD

#include "global.h"
#include "parser.h"
#include "circuit.h"

namespace Solver {
    using Parser::circuit;
    typedef vector<string> vstring;
    void solve(char* pattern_filen, char* fault_filen);
    vstring get_patterns(char*);
    vstring get_faults(char*);

    extern int V;
    extern vector<int> order;
    extern vector<bool> output;

    void init();
    void set_pattern(const string&);
    void peek();
    
    void get_topological_order();
    void dfs(int v);
}

#endif
