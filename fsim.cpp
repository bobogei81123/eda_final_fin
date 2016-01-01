#include "global.h"
#include "parser.h"

using namespace std;

using Parser::circuit;

template <typename T2, typename T1, typename Functor>
vector<T2> foreach(const T1& c, Functor &&f)
{
    vector<T2> ret;
    std::transform(std::begin(c), std::end(c), inserter(ret, ret.begin()), f);
    return ret;
}

int main(int argc, char* argv[]){
    ASSERT(argc == 2);
    Parser::parse(argv[1]);
    auto wire2string = [&](int i) -> string {
        return circuit.wires[i].name;
    };
    cout << foreach<string>(circuit.inputs, wire2string) << endl;
    cout << foreach<string>(circuit.outputs, wire2string) << endl;
}
