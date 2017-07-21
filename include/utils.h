#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <vector>
#include <bitset>


#define SHOWSTEPS true
#define SCROLL false
#define VALIDATION true

enum cell_contents {UNKNOWN = -1, CROSS = 0, BLOCK = 1};
enum line {ROW = 0, COLUMN = 1};

using namespace std;

int sum(const vector<int>& v) {
    int sum = 0;
    for(vector<int>::const_iterator it = v.begin(); it != v.end(); ++it) {
        sum += *it;
    }
    return sum;
}

template <typename T>
void print(const vector<T>& v) {
    // "typename" it:
    // This tells the compiler that vector<T>::iterator should be treated as a type,
    // something it can't assume since iterator is dependent on what T is.
    for(typename vector<T>::const_iterator it = v.begin(); it != v.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
}


template <int size>
void print_cl(const bitset<size>& v, string cross = "X", string block = "#") {
    for(int row = v.size() - 1; row >= 0; --row) {
        if(v[row] == 0) {
            cout << cross;
        } else {
            cout << block;
        }
    }
    cout << endl;
}


#endif // UTILS_H_INCLUDED
