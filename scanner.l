%{
#include <bits/stdc++.h>
#include "driver.h"
#include "parser.hpp"
using namespace std;
%}

%option noyywrap
digit [0-9]
idchar [a-zA-Z_]
ws [ \t]+

%%
{ws} 
\n          cout << "\n";
module      cout << "module "; return MODBEGIN;
endmodule   cout << "endmod "; return MODEND;
input       cout << "input "; return INPUT;
output      cout << "output "; return OUTPUT;
wire        cout << "wire "; return WIRE;
reg        cout << "reg "; return REG;
and        cout << "and "; return AND;
nand        cout << "nand "; return NAND;
or        cout << "or "; return OR;
nor        cout << "nor "; return NOR;
not        cout << "not "; return NOT;
always      cout << "always "; return ALWAYS;
begin       cout << "begin "; return BEG;
end       cout << "begin "; return END;
(pos|neg)edge       cout << "edge "; return EDGE;
"//"[^\n]*    cout << "Comment = " << yytext << endl;
{idchar}({idchar}|{digit})*      {
    cout << "ID(" << yytext << ") ";
    yylval.strval = new string(yytext);
    return ID;
}
. cout << "op(" << yytext << ") "; return yytext[0];
%%
