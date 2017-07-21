#ifndef INPUTPROMPTER_H
#define INPUTPROMPTER_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "utils.h"
using namespace std;

template <typename T>
T stringToNumber(const string &Text) {
    istringstream ss(Text);
    T result;
    return ss >> result ? result : 0;
}

class InputPrompter {
public:

    template<int CHOICE, int SIZE>
    static vector<vector<int>> prompt() {
        vector< vector<int> > ans(SIZE, vector<int>());
        if(CHOICE == COLUMN) {
            cout << "Enter hints for column, top down, separate by space" << endl;
        } else {
            cout << "Enter hints for row, left to right, separate by space" << endl;
        }
        for(int row = 0; row < SIZE; row++) {
            cout << (CHOICE == COLUMN ? "columns " : "rows ") << (row + 1) << ": ";
            string s;
            getline(cin, s);
            int token;
            istringstream iss(s);
            while(iss >> token) {
                ans[row].push_back(token);
            }
        }

        return ans;
    }

};

#endif // INPUTPROMPTER_H
