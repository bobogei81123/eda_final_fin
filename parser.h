#include "global.h"
#include "lexer.h"
#include "circuit.h"

namespace Parser {
    void parse(char*);
    void parse_module(string);
    vector<string> parse_argument();
    void parse_inner(bool);
    vector<string> parse_io();
    vector<string> parse_gate();
    
    extern Circuit circuit;
}
