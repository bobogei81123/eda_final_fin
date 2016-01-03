#ifndef SOLVER_H_GAURD
#define SOLVER_H_GAURD

#include "global.h"
#include "parser.h"
#include "circuit.h"

namespace Solver {
    using Parser::circuit;
    typedef vector<string> vstring;

    struct Fault {
        enum class TYPE {
            NONE,
            INPUT,
            OUTPUT,
        };

        int wire;
        int gate;
        TYPE type;
        int value;
        int id;

        inline static Fault no_fault() {
            return {0, 0, TYPE::NONE, 0, 0};
        }

        inline bool has_fault() const {
            return type != TYPE::NONE;
        }

        inline bool is_output() const {
            return type == TYPE::OUTPUT;
        }

        inline bool is_input() const {
            return type == TYPE::INPUT;
        }
    };

    void solve(char* pattern_filen, char* fault_filen, char* output_filen=NULL);
    vstring get_patterns(char*);
    vstring get_faults(char*);

    Fault fault_from_string(string, int id=0);

    extern int V;
    extern vector<int> order;
    extern vector<bool> visit, is_input;

    void init();
    vector<bool> set_pattern(const string&, vector<bool>&);
    vector<bool> peek(vector<bool> &res,
            const Fault &fault = Fault::no_fault());
    
    void get_topological_order();
    void dfs(int v);
}

#endif
