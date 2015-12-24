#include<bits/stdc++.h>
#include "parser.h"

using namespace std;

namespace Parser {
    void parse(char* filename) {
        FILE *f = fopen(filename, "r");
        auto tokens = Lexer::parse(f);


    }
}

int main(){
    Parser::parse("s27.v");
}

