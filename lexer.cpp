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
            if (c == EOF) break;
            string s;
            if (isalnum(c) or c == '_') {
                while (isalnum(c) or c == '_') {
                    s += c;
                    c = fgetc(f);
                }
            } else if (isprint(c)) {

#define READ_TO_EOL \
    while (c != '\n' and c != EOF) { \
        c = fgetc(f); \
    } \

                if (c == '/') { // if comment
                    char tc = fgetc(f);
                    if (tc == '/') {
                        READ_TO_EOL;
                        continue;
                    } else {
                        s += c;
                        c = tc;
                    }
                } else if (c == '#') { // if comment
                    READ_TO_EOL;
                    continue;
                } else {
                    s += c;
                    c = fgetc(f);
                }
            }

            ASSERT(s.length());
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
