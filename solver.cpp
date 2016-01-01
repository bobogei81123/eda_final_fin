#include "solver.h"

using namespace std;


namespace Solver {
    void solve(char* pattern_filen, char* fault_filen) {
        auto patterns = get_patterns(pattern_filen);
        auto faults = get_faults(pattern_filen);

        init();
    }

    vstring get_patterns(char *fn) {
        FILE *f = fopen(fn, "r");
        auto ls = Lexer::parse(f);

        vstring ret;
        for (auto x: ls) {
            if (x != "{" and x != "}")
                ret.push_back(x);
        }
        return ret;
    }

    vstring get_faults(char *fn) {
        FILE *f = fopen(fn, "r");
        auto ls = Lexer::parse(f);

        return ls;
    }

    int V;
    vector<int> order;
    vector<bool> visit;
    vector<bool> result;
    vector<bool> is_input;

    void init() {
        V = circuit.wire_count;
        visit.resize(V);
        output.resize(V);
        get_topological_order();
    }

    void get_topological_order() {
        for (auto x: circuit.outputs) {
            dfs(x);
        }
    }

    void dfs(int v) {
        if (visit[v]) return;
        visit[v] = true;

#define RETURN {order.push_back(v); return;}

        Wire wire = circuit.wires[v];

        int gid = wire.from;
        if (gid == -1)
            RETURN;

        Gate gate = circuit.gates[gid];
        if (gate.is_dff())
            RETURN;

        for (auto x: gate.inputs) {
            dfs(x);
        }

        RETURN;
#undef RETURN
    }

    void set_pattern(const string &pat) {
        ASSERT(pat.length() == circuit.inputs.size());

        auto &ips = circuit.inputs;
        for (int i=0; i<SIZE(ips); i++) {
            result[ips[i]] = (pat[i] == '1');
            is_input[ips[i]] = true;
        }
    }

    void peek(int start) {
        for (int i=start; i<V; i++) {
            int wire = order[i];
            if (is_input[wire]) continue;

            int from = wire.from;
            ASSERT(from != -1);
            Gate gate = circuit.gates[from];
            ASSERT(not gate.is_dff()); 

            
        }
    }
}
