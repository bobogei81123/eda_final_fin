#include "parser.h"

using namespace std;

namespace Parser {
    typedef vector<string> vstring;
    int size;
    int pos;
    vstring tokens;
    Circuit circuit;

    void parse(char* fn) {
        FILE *f = fopen(fn, "r");
        string path = fn;

        int slash_pos = (path.find_last_of("/") == string::npos ?
                0 : path.find_last_of("/")+1);
        int dot_pos = path.find('.');
        int sub_len = dot_pos - slash_pos;
        string prefix = path.substr(slash_pos, sub_len);

        tokens = Lexer::parse(f);
        pos = 0;
        size = tokens.size();
        tokens.push_back("EOF");

        while (pos < size) {
            parse_module(prefix);
            pos ++;
            cout << pos << ' ' << size << endl;
        }
    }

#define next_tok curtok = tokens[++pos]
    void parse_module(string main_module) {
        auto curtok = tokens[pos];
        next_tok;
        auto module_name = curtok;
        bool is_main = (module_name == main_module);

        // Parse argument
        next_tok;
        auto args = parse_argument();
        next_tok;
        next_tok;
        parse_inner(is_main);
    }

    vector<string> parse_argument() {
        auto curtok = tokens[pos];
        next_tok;
        vector<string> args;
        while (curtok != ")") {
            if (curtok == ",") next_tok;
            args.push_back(curtok);
            next_tok;
        }
        return args;
    }

    void parse_inner(bool is_main) {
        auto curtok = tokens[pos];
        while (curtok != "endmodule") {
            if (curtok == "input" or curtok == "output" or curtok == "wire") {
                auto io = parse_io();
                next_tok;

                if (is_main) {
                    circuit.add_wires(io);
                }
            } else {
                if (is_main) {
                    auto gate = parse_gate();
                    circuit.add_gate(gate);
                }
                next_tok;
            }
        }
    }

    vstring parse_io() {
        vstring res;
        auto curtok = tokens[pos];
        res.push_back(curtok); //type;
        next_tok;

        while (curtok != ";") {
            if (curtok == ",") next_tok;
            res.push_back(curtok);
            next_tok;
        }
        return res;
    }

    vstring parse_gate() {
        vstring res;
        auto curtok = tokens[pos]; //type
        res.push_back(curtok);
        next_tok; //name
        res.push_back(curtok);
        next_tok;

        auto args = parse_argument();
        res.reserve(res.size() + args.size());
        res.insert(res.end(), args.begin(), args.end());
        next_tok;

        return res;
    }
}


