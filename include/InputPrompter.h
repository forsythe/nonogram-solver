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
        for(int k = 0; k < SIZE; k++) {
            cout << (CHOICE == COLUMN ? "columns " : "rows ") << (k + 1) << ": ";
            string s;
            getline(cin, s);
            int token;
            istringstream iss(s);
            while(iss >> token) {
                ans[k].push_back(token);
            }
        }
        for(int c = 0; c < ans.size(); c++) {
            for(int val = 0; val < ans[c].size(); val++) {
                cout << ans[c][val] << " ";
            }
            cout << endl;
        }
        return ans;
    }

};

#endif // INPUTPROMPTER_H
