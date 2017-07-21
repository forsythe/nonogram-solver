#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <conio.h>
#include <windows.h>

#include "utils.h"
using namespace std;

extern bool invalid;

template <int W, int H>
class Grid {
private:
    template <int A, int B>
    friend ostream& operator<<(ostream& os, const Grid<A, B>& g);

    int**grid;
    int width;
    int height;
public:

    Grid(): width(W), height(H) {
        grid = new int*[H];
        for(int k = 0; k < H; k++) {
            grid[k] = new int[W];
        }
        for(int r = 0; r < H; r++) {
            for(int c = 0; c < W; c++) {
                grid[r][c] = UNKNOWN;
            }
        }
    }
    ~Grid() {
        for(int k = 0; k < H; k++) {
            delete[] grid[k];
        }
        delete[] grid;
    }

    //warning: either gets the known blocks, or known crosses, but not both
    bitset<H> getCol(int c, int want) {
        bitset<H> ans;
        //cout << "called getCol" << endl;
        if(want == BLOCK) {
            //cout << "block wanted" << endl;
            for(int r = 0; r < H; r++) { //iterate backwards
                ans <<= 1;
                //cout << ans << endl;
                if(grid[r][c] == BLOCK) {
                    ans.set(0, BLOCK);
                }
            }
        } else {
            for(int r = 0; r < H; r++) {
                ans <<= 1;
                if(grid[r][c] == CROSS) {
                    ans.set(0, 1 - CROSS);
                }
            }
            ans.flip();
        }
        //cout << "mode: " << (want == BLOCK ? "BLOCK" : "CROSS") << " "  << c << " looks like " << ans << endl;
        return ans;
    }

    //warning: either gets the known blocks, or known crosses, but not both
    bitset<W> getRow(int r, int want) {
        bitset<W> ans;
        if(want == BLOCK) {
            for(int c = 0; c < W; c++) {
                ans <<= 1;
                if(grid[r][c] == BLOCK) {
                    ans.set(0, BLOCK);
                }
            }
        } else {
            for(int c = 0; c < W; c++) {
                ans <<= 1;
                if(grid[r][c] == CROSS) {
                    ans.set(0, 1 - CROSS);
                }
            }
            ans.flip();
        }
        return ans;
    }

    void setCol(int c, const bitset<H>& col) {
        for(int r = 0; r < H; r++) {
            if(VALIDATION) {
                if(grid[r][c] == 1 - col[H - r - 1]) {
                    invalid = true;
                }
            }
            grid[r][c] = col[H - r - 1];
        }
    }
    void setRow(int r, const bitset<W>& row) {
        for(int c = 0; c < W; c++) {
            if(VALIDATION) {
                if(grid[r][c] == 1 - row[W - c - 1]) {
                    invalid = true;
                }
            }
            grid[r][c] = row[W - c - 1];
        }
    }

    //allow for [][] access
    int* operator[](int row) {
        return grid[row];
    }
    const int* operator[](int row) const {
        return grid[row];
    }


};

template <int A, int B>
ostream& operator<<(ostream& os, const Grid<A, B>& g) {
    for(int r = 0; r < g.height; r++) {
        for(int c = 0; c < g.width; c++) {
            switch(g[r][c]) {
            case BLOCK:
                os << "¢i";
                break;
            case CROSS:
                os << "  ";
                break;
            default:
                os << "  ";
            }
        }
        os << endl;
    }
    return os;
}

template<int WIDTH, int HEIGHT>
void pretty_print(const Grid<WIDTH, HEIGHT>& g, const vector<vector<int>>& col_hints,
                  const vector<vector<int>>& row_hints) {
    int max_row_hint_length = 0;
    int max_col_hint_length = 0;
    if(SCROLL) {
        COORD coord;
        coord.X = 0;
        coord.Y = HEIGHT + max_col_hint_length + 10;
        SetConsoleCursorPosition(
            GetStdHandle(STD_OUTPUT_HANDLE),
            coord
        );
    }
    for(int k = 0; k < row_hints.size(); k++) {
        max_row_hint_length = max(max_row_hint_length, (int)row_hints[k].size());
    }
    for(int k = 0; k < col_hints.size(); k++) {
        max_col_hint_length = max(max_col_hint_length, (int)col_hints[k].size());
    }
    for(int k = 0; k <= max_col_hint_length; k++) {
        for(int k = 0; k < max_row_hint_length; k++) {
            cout << "  "; //horizontal offset for the column hints
        }
        cout << " ";
        for(int col = 0; col < col_hints.size(); col++) {
            if(max_col_hint_length - k <  col_hints[col].size()) {
                cout << setw(2) << col_hints[col][k - (max_col_hint_length - col_hints[col].size()) - 1];
            } else {
                cout << "  ";
            }
        }
        cout << endl;
    }
    for(int row = 0; row < row_hints.size(); row++) {
        for(int s = 0; s < max_row_hint_length - row_hints[row].size(); s++) { //horizontal offset for row hints
            cout << "  ";
        }
        for(int v = 0; v < row_hints[row].size(); v++) {
            cout << setw(2) << row_hints[row][v];
        }
        cout << " ";
        for(int c = 0; c < WIDTH; c++) {
            switch(g[row][c]) {
            case BLOCK:
                cout << "¢i";
                break;
            case CROSS:
                cout << ". ";
                break;
            default:
                cout << "  ";
            }
        }
        cout << endl;
    }
}
#endif // GRID_H
