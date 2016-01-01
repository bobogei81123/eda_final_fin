#include "solver.h"

using namespace std;


namespace Solver {
    void solve(char* pattern_filen, char* fault_filen) {
        circuit.finish();
        auto patterns = get_patterns(pattern_filen);
        auto fault_names = get_faults(fault_filen);
        int fault_n = fault_names.size();

        vector<Fault> faults;
        int _i = 0;
        for (auto x: fault_names) {
            faults.push_back(fault_from_string(x, _i++));
        }

        init();

        for (auto p: patterns) {
            set_pattern(p);

            _i = 0;
            cout << p << endl;
            //cout << true_result << endl;
            while (_i < SIZE(faults)) {
                auto f = faults[_i];
                if (get<3>(f) == result[get<0>(f)]) {
                    _i ++;
                    continue;
                }
                set_fault(f);
                //cout << _i << endl;
                vector<bool> temp_res = result;

                //auto &ips = circuit.inputs;
                //vector<bool> temp_res(result.size());
                //for (int i=0; i<SIZE(ips); i++) {
                    //temp_res[ips[i]] = (p[i] == '1');
                    //is_input[ips[i]] = true;
                //}

                auto r = peek(temp_res);
                if (true_result != r) {
                    swap(faults[_i], faults.back());
                    faults.pop_back();
                } else _i++;

                /*
                cout << fault_names[get<4>(f)] << ": ";
                cout << "(" << circuit.wires[get<0>(f)].name << "," << 
                    (get<2>(f) == FAULT_TYPE::INPUT ? "INPUT" : "OUTPUT")
                    << ") ";
                cout << r << endl;
                */
            }
        }

        int undetected_n = faults.size();
        cout << "DETECTED " << fault_n - undetected_n << '\n' 
             << "UNDETECTED " << undetected_n << endl;
        sort(faults.begin(), faults.end());
        for (auto f: faults)
            cout << fault_names[get<4>(f)] << endl;
    }

    void set_fault(Fault f) {
        tie(fault_wire, fault_gate, fault_type, fault_value, ignore) = f;
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
    vector<bool> visit, result, is_input, true_result;
    FAULT_TYPE fault_type;
    int fault_wire, fault_gate;
    bool fault_value;

    
    Fault fault_from_string(string s, int id) {
        //regex Rwire_circuit_io("([^_]+)_sa([01])");
        //regex Rwire_gate_io("([^_]+)_(.*)_sa([01])");

        //smatch match_res;
        //bool matched;

        //cout << s << endl;
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

        /*
            }
        }

        matched = regex_match(s, match_res, Rwire_gate_io);
        if (matched) {
            string swire = match_res[1].str(),
                   sgate = match_res[2].str();
            bool val = (match_res[3].str() == "1");
            */
        {
            ASSERT(ISIN(circuit.wireID, swire));

            int wid = circuit.wireID[swire];
            Wire wire = circuit.wires[wid];
            FAULT_TYPE type; 

            if (swire == sgate) {
                type = wire.from == -1 ?
                    FAULT_TYPE::OUTPUT : FAULT_TYPE::INPUT;
                return make_tuple(wid, -1, type, val, id);
            }

            ASSERT(ISIN(circuit.gateID, sgate));

            int gid = circuit.gateID[sgate];
            Gate gate = circuit.gates[gid];
            type = wire.from == gid ?
                FAULT_TYPE::OUTPUT : FAULT_TYPE::INPUT;
            return make_tuple(wid, gid, type, val, id);
        }

        ASSERT(false);
    }

    void init() {
        V = circuit.wire_count;
        visit.resize(V);
        revord.resize(V);
        result.resize(V);
        is_input.resize(V);
        get_topological_order();

        clear_fault();
    }

    void clear_fault() {
        fault_type = FAULT_TYPE::NONE;
        fault_wire = fault_gate = -2;
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
        clear_fault();
        true_result = peek();
        //for (int i=0; i<SIZE(circuit.wires); i++) {
            //cout << circuit.wires[i].name << ' ' << result[i] << endl;
        //}
    }

    vector<bool> peek(vector<bool> &res) {
        int start = 0;
        if (fault_type != FAULT_TYPE::NONE and fault_wire >= 0)
            start = revord[fault_wire];

        for (int i=start; i<V; i++) {
            int wire = order[i];

            int from = circuit.wires[wire].from;

            if (fault_type == FAULT_TYPE::OUTPUT
                and wire == fault_wire) {

                res[wire] = fault_value;
                continue;
            }

            if (is_input[wire]) continue;

            ASSERT(from != -1);
            Gate gate = circuit.gates[from];
            ASSERT(not gate.is_dff()); 


            vector<bool> in(gate.inputs.size());
            if (fault_type == FAULT_TYPE::INPUT and from == fault_gate) {
                for (int j=0; j<SIZE(gate.inputs); j++) {
                    int tw = gate.inputs[j];
                    if (tw == fault_wire) {
                           in[j] = fault_value;
                    } else in[j] = res[tw];
                }
                //cout << fault_value << ' ' << gate.name << endl;
                //cout << gate.feed(in) << endl;
            } else {
                for (int j=0; j<SIZE(gate.inputs); j++) {
                    in[j] = res[gate.inputs[j]];
                }
            }

            res[wire] = gate.feed(in);
        }

        vector<bool> out(circuit.outputs.size());
        int _i = 0;
        for (auto x: circuit.outputs) {
            if (fault_type == FAULT_TYPE::INPUT
                and x == fault_wire) {

                out[_i++] = fault_value;
            } else {
                out[_i++] = res[x];
            }
        } 
        return out;
    }
}

