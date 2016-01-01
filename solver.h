#ifndef SOLVER_H_GAURD
#define SOLVER_H_GAURD

#include "global.h"
#include "parser.h"
#include "circuit.h"

namespace Solver {
    using Parser::circuit;
    typedef vector<string> vstring;

    enum class FAULT_TYPE {
        NONE,
        INPUT,
        OUTPUT,
    };

    typedef tuple<int, int, FAULT_TYPE, bool, int> Fault; 
    //           wire,      type , val , id

    void solve(char* pattern_filen, char* fault_filen);
    vstring get_patterns(char*);
    vstring get_faults(char*);
    void set_fault(Fault);
    void clear_fault();

    Fault fault_from_string(string, int id=0);

    extern int V;
    extern vector<int> order;
    extern vector<bool> visit, result, is_input, true_result;
    extern FAULT_TYPE fault_type;
    extern int fault_wire, fault_gate;
    extern bool fault_value;

    void init();
    void set_pattern(const string&);
    vector<bool> peek(vector<bool> &res = result);
    
    void get_topological_order();
    void dfs(int v);
}

#endif
