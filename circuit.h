#ifndef CIRCUIT_H_GAURD
#define CIRCUIT_H_GAURD
#include "global.h"

struct Wire {
    string name;
    int from, to;
};


struct Gate {
    enum class TYPE {
        AND,
        OR,
        NAND,
        NOR,
        NOT,
        DFF,
    };
    TYPE type;
    int id;
    string name;
    vector<int> inputs;
    int output;

    static TYPE string_to_type(string s);
    bool is_dff();

    bool feed(const vector<bool> &in);
};

struct Circuit {
    int wire_count, gate_count;
    map<string, int> wireID;
    vector<Wire> wires;
    vector<Gate> gates;
    vector<int> inputs;
    vector<int> outputs;

    static inline bool wire_blacklisted(string c) {
        return (c == "CK" or c == "VDD" or c == "GND");
    }

    inline Wire& get_wire(string s) {
        return wires[wireID[s]];
    }

    void add_wires(vector<string>);
    void add_gate(const vector<string> &);
    void add_logic_gate(const vector<string> &, int);
    void add_dff(const vector<string> &, int);

    void print();
};
#endif
