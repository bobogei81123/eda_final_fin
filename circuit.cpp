#include "circuit.h"

Gate::TYPE Gate::string_to_type(string s) {
    if (s == "and")
        return Gate::TYPE::AND;
    if (s == "or")
        return Gate::TYPE::OR;
    if (s == "nand")
        return Gate::TYPE::NAND;
    if (s == "nor")
        return Gate::TYPE::NOR;
    if (s == "not")
        return Gate::TYPE::NOT;
    if (s == "xor")
        return Gate::TYPE::XOR;
    if (s == "dff")
        return Gate::TYPE::DFF;

    ASSERT(false);
}

bool Gate::is_dff() {
    return type == TYPE::DFF;
}

bool Gate::feed(const vector<bool> &in) {
    switch (type) {
        case TYPE::DFF:
            ASSERT(false);
            return false;
        case TYPE::NOT:
            ASSERT(in.size() == 1);
            return !in[0];

        case TYPE::AND:
        case TYPE::NAND:
            for (auto x: in) {
                if (not x) return type == TYPE::AND ? false : true;
            }
            return type == TYPE::AND ? true : false;

        case TYPE::OR:
        case TYPE::NOR:
            for (auto x: in) {
                if (x) return type == TYPE::OR ? true : false;
            }
            return type == TYPE::OR ? false : true;
        case TYPE::XOR:
            bool res = 0;
            for (auto x: in) {
                if (x) res = !res;
            }
            return res;
    }
    ASSERT(false);
}

void Circuit::add_wires(vector<string> wl) {
    int sz = wl.size();
    ASSERT(sz >= 1);

    auto type = wl[0];

    for (int i=1; i<sz; i++) {
        auto name = wl[i];
        if (wire_blacklisted(name)) continue;

        if (wireID.find(name) != wireID.end())
            continue;

        int me;
        wireID[name] = me = wire_count;
        wire_count ++;
        if (type == "input")
            inputs.push_back(me);
        else if(type == "output")
            outputs.push_back(me);

        wires.push_back({name, -1});
    }
}

void Circuit::add_gate(const vector<string> &desc) {
    int sz = desc.size();
    ASSERT(sz >= 4);
    auto type = desc[0];
    int _id = gate_count++;
    if (type == "dff")
        add_dff(desc, _id);
    else
        add_logic_gate(desc, _id);
}

void Circuit::add_dff(const vector<string> &desc, int _id) {
    int sz = desc.size();
    string type = desc[0], 
           name = desc[1], inpn, outn;

    if (sz == 5) {
        inpn = desc[3];
        outn = desc[4];
    } else {
        // A hack because td s1196.v is broken...
        inpn = desc[2];
        outn = desc[3];
    }

    ASSERT(get_wire(inpn).from == -1);
    get_wire(inpn).from = _id;
    inputs.push_back(wireID[inpn]);
    //get_wire(outn).to   = _id;
    outputs.push_back(wireID[outn]);

    Gate gate = {Gate::TYPE::DFF, _id, name};
    // circuit input = DFF output
    gate.inputs.push_back(wireID[outn]);
    gate.output = wireID[inpn];
    gates.push_back(gate);
    gateID[name] = _id;
}

void Circuit::add_logic_gate(const vector<string> &desc, int _id) {
    int sz = desc.size();
    auto type = desc[0] 
        ,name = desc[1]
        ,outn = desc[2];

    Gate gate = {
        Gate::string_to_type(type),
        _id,
        name
    };
    gate.output = wireID[outn];
    ASSERT(get_wire(outn).from == -1);
    get_wire(outn).from = _id;

    for (int i=3; i<sz; i++) {
        gate.inputs.push_back(wireID[desc[i]]);
        //get_wire(desc[i]).to = _id;
    }
    gates.push_back(gate);
    gateID[name] = _id;
}

void Circuit::finish() {
    auto functor = [&](int a, int b) {
        return wires[a].name < wires[b].name;
    };
    sort(inputs.begin(), inputs.end(), functor);
    sort(outputs.begin(), outputs.end(), functor);
}

void Circuit::print() {
    cout << "Gate count = " << gate_count << endl;
    for (auto x: gates) {
        cout << x.name << ' ';
        cout << "I = ";
        for (auto y: x.inputs)
            cout << wires[y].name << ' ';
        cout << ", O = " << wires[x.output].name << endl;
    }
    cout << "Output = ";
    for (auto x: outputs) {
        cout << wires[x].name << ' ';
    } cout << endl;

    cout << "Input = ";
    for (auto x: inputs) {
        cout << wires[x].name << ' ';
    } cout << endl;

    cout << "Wires = " << wires.size() << endl;
    for (auto x: wires) {
        cout << x.name << ": " << 
            (x.from != -1 ? gates[x.from].name : "Input") << endl;
    }
}

void Circuit::dump_faults(string fn) {
    ofstream fout(fn);

    for (auto x: inputs) {
        string name = wires[x].name;
        fout << name << "_" << name << "_sa0" << endl;
        fout << name << "_" << name << "_sa1" << endl;
    }
    for (auto x: outputs) {
        string name = wires[x].name;
        fout << name << "_" << name << "_sa0" << endl;
        fout << name << "_" << name << "_sa1" << endl;
    }

    for (auto g: gates) {
        for (auto x: g.inputs) {
            string name = wires[x].name;
            fout << name << "_" << g.name << "_sa0" << endl;
            fout << name << "_" << g.name << "_sa1" << endl;
        }
        string name = wires[g.output].name;
        fout << name << "_" << g.name << "_sa0" << endl;
        fout << name << "_" << g.name << "_sa1" << endl;
    }
    return;
}
