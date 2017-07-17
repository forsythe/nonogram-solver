#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

template <typename T>
  T stringToNumber ( const string &Text )
  {
     istringstream ss(Text);
     T result;
     return ss >> result ? result : 0;
  }

int main()
{
    vector< vector<int> > col_hints;
    cout << "How many columns?" << endl;

    int col;
    cin >> col;

    cout << "Enter hints for column, top down, separate by space" << endl;
    string delim = " ";
    for (int k = 0; k < col; k++){
        cout << "Column " << (k+1) << ": " << endl;
        string s;
        std::cin.clear();

        cin >> s;

        size_t pos = 0;
        string token;
        while ((pos = s.find(delim)) != std::string::npos) {
            token = s.substr(0, pos);
            col_hints[k].push_back(stringToNumber<int>(token));
            s.erase(0, pos + delim.length());
        }
    }

    for (int r = 0; r < 3; r++){
        for (int c = 0; c < 3; c++){
            cout << col_hints[r][c] << endl;
        }
    }


}
