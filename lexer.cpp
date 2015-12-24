#include "lexer.h"
using namespace std;

namespace Lexer {
    const int MAX_BUF = 10000;
    vector<string> parse(FILE *f) {
        char c = fgetc(f);
        vector<string> ret;
        while (c != EOF) {
            while (isspace(c)) {
                c = fgetc(f);
            }
            string s;
            if (isalnum(c) or c == '_') {
                while (isalnum(c) or c == '_') {
                    s += c;
                    c = fgetc(f);
                }
            } else if (isprint(c)) {
                if (c == '/') {
                    char tc = fgetc(f);
                    if (tc == '/') {
                        while (c != '\n' and c != EOF) {
                            c = fgetc(f);
                        }
                        continue;
                    } else {
                        s += c;
                        c = tc;
                    }
                } else {
                    s += c;
                    c = fgetc(f);
                }
            }
            ret.push_back(s);
        }
        return ret;
    }
}

/*
int main(){
    FILE *f = fopen("s38584.v", "r");
    auto l = Lexer::parse(f);
    for (auto x: l) {
        cout << x << ' ';
    }
}
*/
