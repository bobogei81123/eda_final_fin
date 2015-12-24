#include<bits/stdc++.h>
#include "parser.h"

using namespace std;

namespace Parser {
    int size;
    int pos;
    vector<string> tokens;

    void parse(char* fn) {
        FILE *f = fopen(fn, "r");
        string filename = fn;

        int dot_pos = filename.find('.');
        assert(dot_pos != string::npos);
        string prefix = filename.substr(0, dot_pos);

        tokens = Lexer::parse(f);
        pos = 0;
        size = tokens.size();

        while (pos < size) {
            parse_module(prefix);
        }
    }

#define next_tok curtok = tokens[++pos]
    void parse_module(string main_module) {
        auto curtok = tokens[pos];
        assert(curtok == "module");
        next_tok;
        bool is_main = (curtok == main_module);

        
    }
}

int main(){
    Parser::parse("s27.v");
}

