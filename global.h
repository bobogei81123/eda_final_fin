#ifndef GLOBAL_H_GUARD
#define GLOBAL_H_GUARD
#include <bits/stdc++.h>
using namespace std;

#ifdef DEBUG
#define ASSERT(x) assert(x)
#else
#define ASSERT(x) ;
#endif

#define SIZE(x) ((int)(x).size())

template<typename T>
ostream& operator << (ostream& o, const vector<T> &v) {
    o << "[";
    for (int i=0; i<(int)v.size(); i++) {
        o << v[i] << (i == (int)v.size()-1 ? "" : ", ");
    }
    cout << "]";
    return o;
}

#endif
