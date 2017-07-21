#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <vector>
#include "utils.h"
using namespace std;


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
        //cout << "called setcol" << endl;
        for(int r = 0; r < H; r++) {
            grid[r][c] = col[H - r - 1];
        }
    }
    void setRow(int r, const bitset<W>& row) {
        //cout << "called setrow" << endl;
        for(int c = 0; c < W; c++) {
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
                os << "##";
                break;
            case CROSS:
                os << ". ";
                break;
            default:
                os << "  ";
            }
        }
        os << endl;
    }
    return os;
}


#endif // GRID_H
