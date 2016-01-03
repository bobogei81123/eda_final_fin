#include "solver.h"

using namespace std;


namespace Solver {
    void solve(char* pattern_filen,
               char* fault_filen,
               char* output_filen) {

        circuit.finish();
        auto patterns = get_patterns(pattern_filen);
        auto fault_names = get_faults(fault_filen);
        int fault_n = fault_names.size();

        vector<Fault> faults;
        for (int i=0; i<SIZE(fault_names); i++) {
            faults.push_back(fault_from_string(fault_names[i], i));
        }

        init();

        vector<bool> wv_nofault(circuit.wire_count),
                     wv_temp   (circuit.wire_count);

        for (int pc=0; pc<SIZE(patterns); pc++) {
            auto result_nofault = set_pattern(patterns[pc], wv_nofault);
            int i = 0;
            while (i < SIZE(faults)) {
                auto fault = faults[i];
                if (fault.value == wv_nofault[fault.wire]) {
                    i ++;
                    continue;
                }
                wv_temp = wv_nofault;

                auto result = peek(wv_temp, fault);
                if (result != result_nofault) {
                    swap(faults[i], faults.back());
                    faults.pop_back();
                } else i++;
            }
        }

        int undetected_n = faults.size();
        ostream *o = output_filen ? new ofstream(output_filen) : &cout;
        ofstream fout(output_filen);
        *o << "DETECTED " << fault_n - undetected_n << '\n' 
           << "UNDETECTED " << undetected_n << endl;
        sort(faults.begin(), faults.end(), [](Fault f1, Fault f2) {
                    return f1.id < f2.id;
                });
        for (auto f: faults)
            *o << fault_names[f.id] << endl;
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
    vector<int> order, revord;
    vector<bool> visit, is_input;
    vector<int> max_back;
    
    Fault fault_from_string(string s, int id) {
        int len = s.length();
        ASSERT(len >= 4);

        string np = s.substr(0, len - 4);
        string swire, sgate;
    
        bool val = (s[len-1] == '1');
        for (int i=1; i<len-4; i++) {
            if (np[i] == '_') {
                string s1 = s.substr(0, i);
                string s2 = s.substr(i+1, len-4-i-1);
                //cout << s1 << ' ' << s2 << endl;
                if (ISIN(circuit.wireID, s1) and
                    (s1 == s2 or ISIN(circuit.gateID, s2))) {
                    swire = s1;
                    sgate = s2;
                    break;
                }
            }
        }

        ASSERT(ISIN(circuit.wireID, swire));

        int wid = circuit.wireID[swire];
        Wire wire = circuit.wires[wid];
        Fault::TYPE type; 

        if (swire == sgate) {
            type = wire.from == -1 ?
                Fault::TYPE::OUTPUT : Fault::TYPE::INPUT;
            return {wid, -1, type, val, id};
        }

        ASSERT(ISIN(circuit.gateID, sgate));

        int gid = circuit.gateID[sgate];
        Gate gate = circuit.gates[gid];
        type = wire.from == gid ?
            Fault::TYPE::OUTPUT : Fault::TYPE::INPUT;
        return {wid, gid, type, val, id};
    }

    void init() {
        V = circuit.wire_count;
        visit.resize(V);
        revord.resize(V);
        is_input.resize(V);
        max_back.resize(V);
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

#define RETURN {revord[v]=order.size(); order.push_back(v); return;}

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

        int id = order.size();
        for (auto x: gate.inputs) {
            max_back[x] = max(max_back[x], id);
        }

        RETURN;
#undef RETURN
    }

    vector<bool> set_pattern(const string &pat, vector<bool> &wire_value) {
        ASSERT(pat.length() == circuit.inputs.size());

        auto &ips = circuit.inputs;
        vector<bool> result;
        for (int i=0; i<SIZE(ips); i++) {
            wire_value[ips[i]] = (pat[i] == '1');
            is_input[ips[i]] = true;
        }
        return peek(wire_value);
    }

    inline bool quick_feed(const Gate &gate, const vector<bool> &res) {
        switch (gate.type) {
            case Gate::TYPE::NOT:
                return !res[gate.inputs[0]];
            case Gate::TYPE::AND:
            case Gate::TYPE::NAND:
                for (auto x: gate.inputs) {
                    if (!res[x]) 
                        return gate.type == Gate::TYPE::AND ? false : true;
                }
                return gate.type == Gate::TYPE::AND ? true : false;
            case Gate::TYPE::OR:
            case Gate::TYPE::NOR:
                for (auto x: gate.inputs) {
                    if (res[x]) 
                        return gate.type == Gate::TYPE::OR ? true : false;
                }
                return gate.type == Gate::TYPE::OR ? false : true;
        }
        assert(false);
    }

    vector<bool> peek(vector<bool> &res, const Fault &fault) {
        int start = 0;
        if (fault.has_fault() and fault.wire >= 0)
            if (fault.is_output())
                start = revord[fault.wire];
            else if (fault.gate != -1)
                start = revord[circuit.gates[fault.gate].output];
            else start = V;
        int back = start;

        for (int i=start; i<V; i++) {
            if (fault.has_fault() and i > back) break;
            int wire = order[i];
            int from = circuit.wires[wire].from;

            if (fault.is_output() and wire == fault.wire) {
                if (res[wire] != fault.value) {
                    res[wire] = fault.value;
                    back = max_back[wire];
                }
                continue;
            }

            if (is_input[wire]) continue;

            ASSERT(from != -1);
            Gate &gate = circuit.gates[from];
            ASSERT(not gate.is_dff()); 


            bool ans;
            if (fault.is_input() and from == fault.gate) {
                vector<bool> in(gate.inputs.size());
                for (int j=0; j<SIZE(gate.inputs); j++) {
                    int tw = gate.inputs[j];
                    if (tw == fault.wire) {
                           in[j] = fault.value;
                    } else in[j] = res[tw];
                }
                ans = gate.feed(in);
            } else {
                ans = quick_feed(gate, res);
            }

            if (ans != res[wire]) {
                back = max(back, max_back[wire]);
                res[wire] = ans;
            }
        }

        vector<bool> out(circuit.outputs.size());
        for (int i=0; i<SIZE(circuit.outputs); i++) {
            int wire = circuit.outputs[i];
            if (fault.is_input()
                and (fault.gate == -1 or circuit.gates[fault.gate].is_dff())
                and wire == fault.wire) {

                out[i] = fault.value;
            } else {
                out[i] = res[wire];
            }
        } 
        return out;
    }
}

