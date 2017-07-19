#ifndef INPUTPROMPTER_H
#define INPUTPROMPTER_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

template <typename T>
T stringToNumber(const string &Text) {
    istringstream ss(Text);
    T result;
    return ss >> result ? result : 0;
}

class InputPrompter {
public:

    static vector<vector<int>> prompt_col() {
        cout << "How many columns? ";
        int col;
        cin >> col;
        vector< vector<int> > col_hints(col, vector<int>());
        cout << "Enter hints for column, top down, separate by space" << endl;
        char delim = ' ';
        cin.ignore();
        for(int k = 0; k < col; k++) {
            cout << "Column " << (k + 1) << ": ";
            string s;
            getline(cin, s);
            int token;
            istringstream iss(s);
            while(iss >> token) {
                col_hints[k].push_back(token);
            }
        }
        for(int c = 0; c < col_hints.size(); c++) {
            for(int val = 0; val < col_hints[c].size(); val++) {
                cout << col_hints[c][val] << " ";
            }
            cout << endl;
        }
    }

};

#endif // INPUTPROMPTER_H
