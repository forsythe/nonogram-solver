#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <vector>
#include <bitset>

using namespace std;

int sum(vector<int> v) {
    int sum = 0;
    for(std::vector<int>::const_iterator it = v.begin(); it != v.end(); ++it) {
        sum += *it;
    }
    return sum;
}

void print(vector<int> v) {
    for(std::vector<int>::const_iterator it = v.begin(); it != v.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
}


template <int size>
void print_cl(bitset<size> v, string cross = "X ", string block = "¢i") {
    for(int k = v.size() - 1; k >= 0; --k) {
        if(v[k] == 0) {
            cout << cross;
        } else {
            cout << block;
        }
    }
    cout << endl;
}


#endif // UTILS_H_INCLUDED
