#include <iostream>
#include <iomanip>
#include <vector>
#include <bitset>
#include <cassert>
#include <unordered_set>
#include <ctime>
#include <clocale>

#include "utils.h"
#include "Grid.h"
#include "InputPrompter.h"
#include "PascalTriangle.h"

using namespace std;


const int HEIGHT = 25;
const int WIDTH = 25;
static PascalTriangle<2 * WIDTH> ncr;
static Grid<WIDTH, HEIGHT> grid;



/*
* A combinatorial number system maps all subsets
* size k from n items (there are (n choose k) such sets) to the
* integers from [0 to ((n choose k)-1)]. We want an inverse
* function that takes an integer from [0 to nCk-1], and finds
* the corresponding subset sized k={c1, c2, ...ck} from n items.
*
* Enumerate the n items c1=0, c2=1, ...cn=(n-1).  Then,
* N = (ck choose k) + ... (c2 choose 2) + (c1 choose 1). To find the
* corresponding subset {ck, ...c2, c1} from an integer N, solve for:
* -the largest ck satisfying (ck choose k) <= N
* -the largest c_(k-1) satisfying (c_(k-1) choose (k-1)) <= N - (ck choose k)
* -etc...
*
* Given the inequality (n choose k) <= thresh,
* this function finds the biggest value of n.
* Also lowers the threshold for the next element,
* and lowers the starting value (since we can't
* rechoose an element ck).
*/
inline int findBiggestN(int& nstart, int row, int& thresh) {
    for(int n = nstart; n >= 0; --n) {
        if(ncr(n, row) <= thresh) {
            thresh -= ncr(n, row);
            nstart = n;
            return n;
        }
    }
}

/*
* Given a list of integers reprsenting the location
* of bars (in stars & bars), this function interprets
* it as a "bins and balls" solution.
*
* E.g. sab = {5, 4, 3}, setsize = 6
* stars and bars: ***|||
* balls and bins: (3 balls) (0 balls) (0 balls) (0 balls)
* returns: {3, 0, 0, 0}
*/
vector<int> convertSABtoBAB(const vector<int>& sab, int setsize) {
    vector<int> ans;
    int old = -1;
    for(std::vector<int>::const_reverse_iterator it = sab.rbegin();
            it != sab.rend(); ++it) {
        ans.push_back(*it - old - 1);
        old = *it;
    }
    ans.push_back(setsize - old - 1);
    return ans;
}

/*
* Given a crosslogic line (row/col) with hints, this function
* fills it with values and returns the line as a bitset,
* where: crosses are 0s, and blocks are 1s
*
* E.g. WIDTH = 10, blocks = {2, 3}, crosses={3, 1}
* the output will be 000(11)(0111)0, but without the ().
* The () is only used to indicate the groupings from hints.
* Note that every group of blocks after the first implicitly begin
* with a cross.
*/
template <int SIZE>
bitset<SIZE> fillLine(const vector<int>& crosses, const vector<int>& blocks) {
    bitset<SIZE> ans;
    int sz = blocks.size();
    //can ignore first group of crosses, since the entire row starts as crosses (0)
    for(int row = 0; row < sz; row++) {
        if(row > 0) { //every group after the first should have an X before it
            ans <<= 1; //implicitly put an X
        }
        for(int j = 0; j < blocks[row]; j++) {
            ans <<= 1;
            ans.set(0, true);
        }
        ans <<= (crosses[row + 1]);
    }
    //cout << "filled row looks like: ";
    // print_cl<SIZE>(ans);
    return ans;
}

/*
* Given some crosslogic hints, finds possibilities for this row or column.
* Also checks to skip known impossible rows/cols, and will add new ones
* if necessary by comparing it to known to be correct values
*/
template <int SIZE>
inline int findLinePossibilities(const vector<int>& blocks,
                                 unordered_set<int>& bad_index,
                                 const bitset<SIZE>& confirmed_blocks,
                                 const bitset<SIZE>& confirmed_crosses, bitset<SIZE>& commonBlocks, bitset<SIZE>& commonCrosses) {
    int num_solutions = 0;
    commonBlocks.set();
    commonCrosses.reset();
    int num_groups = blocks.size();
    int num_blocks = sum(blocks);
    int num_confirmed_xs = num_groups - 1;
    int balls = SIZE - num_blocks - num_confirmed_xs;//balls
    int bins = num_groups + 1; //bins
    int n = balls + bins - 1;
    int row = bins - 1;
//    cout << "balls = " << balls << endl;
//    cout << "bins = " << bins << endl;
//    cout << "n = " << n << endl;
//    cout << "k = " << k << endl;
    int maxindex = ncr(n, row); //upper bound on how many combinations
    int nstart; //0 index, enumerate the items to be chosen (k of them, from n total)
    int indexstart; //start here and go upwards to check new k-combinations
    for(int index = 0; index < maxindex; ++index) {
        if(bad_index.find(index) != bad_index.end()) {
            continue;
        }
        nstart = n - 1; //start looking from the rightmost obj (largest enumueratedy)
        indexstart = index;
        vector<int> sab;
        for(int i = row; i > 0; --i) {
            sab.push_back(findBiggestN(nstart, i, indexstart));
        }
        bitset<SIZE> temp = fillLine<SIZE>(convertSABtoBAB(sab, n), blocks);
        if(((temp & confirmed_blocks) == confirmed_blocks)
                && ((temp | confirmed_crosses) == confirmed_crosses)) {
            num_solutions++;
            commonBlocks &= temp; //find common blocks
            commonCrosses |= temp; //find common crosses
        } else {
            bad_index.insert(index);
        }
    }
    return num_solutions;
}

bool invalid = false;
int main() {
    cout << "WIDTH: " << WIDTH << endl;
    cout << "HEIGHT: " << HEIGHT << endl;
    vector<vector<int>> col_hints = InputPrompter::prompt<COLUMN, WIDTH>();
    vector<vector<int>> row_hints = InputPrompter::prompt<ROW, HEIGHT>();
    bitset<WIDTH> commonRowBlocks;
    bitset<WIDTH> commonRowCrosses;
    bitset<HEIGHT> commonColBlocks;
    bitset<HEIGHT> commonColCrosses;
    vector<unordered_set<int>> ignore_index_row(HEIGHT, unordered_set<int>());
    vector<unordered_set<int>> ignore_index_col(WIDTH, unordered_set<int>());
    unordered_set<int> finished_row;
    unordered_set<int> finished_col;
    bool changed = false;
    time_t start = clock();
    while(true) {
        changed = false;
        for(int row = 0; row < HEIGHT ; row++) {
            if(finished_row.find(row) != finished_row.end()) {
                continue;
            }
            int num_candidates = findLinePossibilities<WIDTH>(row_hints[row], ignore_index_row[row],
                                 grid.getRow(row, BLOCK), grid.getRow(row, CROSS), commonRowBlocks, commonRowCrosses);
            if(num_candidates == 1) {
                finished_row.insert(row);
                grid.setRow(row, commonRowBlocks); //can either use commonRowBlocks or commonRowCrosses
                changed = true;
                if(SHOWSTEPS) {
                    pretty_print(grid, col_hints, row_hints);
                }
                continue;
            }
            if(commonRowBlocks.any()) {
                for(int col = 0; col < WIDTH; col++) {
                    if(commonRowBlocks[WIDTH - col - 1] == BLOCK && grid[row][col] == UNKNOWN) { //iterate backwards
                        grid[row][col] = BLOCK;
                        changed = true;
                        if(SHOWSTEPS) {
                            pretty_print(grid, col_hints, row_hints);
                        }
                    }
                }
            }
            if(!commonRowCrosses.all()) {
                for(int col = 0; col < WIDTH; col++) {
                    if(commonRowCrosses[WIDTH - col - 1] == CROSS && grid[row][col] == UNKNOWN) { //iterate backwards
                        grid[row][col] = CROSS;
                        changed = true;
                        if(SHOWSTEPS) {
                            pretty_print(grid, col_hints, row_hints);
                        }
                    }
                }
            }
        }
        for(int col = 0; col < WIDTH ; col++) {
            if(finished_col.find(col) != finished_col.end()) {
                continue;
            }
            int num_candidates = findLinePossibilities<HEIGHT>(col_hints[col], ignore_index_col[col],
                                 grid.getCol(col, BLOCK), grid.getCol(col, CROSS), commonColBlocks, commonColCrosses);
            if(num_candidates == 1) {
                finished_col.insert(col);
                grid.setCol(col, commonColBlocks); //can either use commonColBlocks or commonColCrosses
                changed = true;
                if(SHOWSTEPS) {
                    pretty_print(grid, col_hints, row_hints);
                }
                continue;
            }
            if(commonColBlocks.any()) {
                for(int row = 0; row < HEIGHT; row++) {
                    if(commonColBlocks[HEIGHT - row - 1] == BLOCK && grid[row][col] == UNKNOWN) {
                        grid[row][col] = BLOCK;
                        changed = true;
                        if(SHOWSTEPS) {
                            pretty_print(grid, col_hints, row_hints);
                        }
                    }
                }
            }
            if(!commonColCrosses.all()) {
                for(int row = 0; row < HEIGHT; row++) {
                    if(commonColCrosses[HEIGHT - row - 1] == CROSS && grid[row][col] == UNKNOWN) {
                        grid[row][col] = CROSS;
                        changed = true;
                        if(SHOWSTEPS) {
                            pretty_print(grid, col_hints, row_hints);
                        }
                    }
                }
            }
            //cout << grid << endl;
        }
        if(!changed || invalid) {
            break;
        }
    }
    //cout << grid << endl;
    pretty_print(grid, col_hints, row_hints);
    if(VALIDATION) {
        for(int r = 0; r < HEIGHT; r++) {
            for(int c = 0; c < WIDTH; c++) {
                if(grid[r][c] == UNKNOWN) {
                    invalid = true;
                    break;
                }
            }
            if(invalid) {
                break;
            }
        }
        if(finished_row.size() != HEIGHT && finished_col.size() != WIDTH) {
            invalid = true;
        }
    }
    if(invalid) {
        cout << "no unique solution exists (" << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms)" << endl;
    } else {
        cout << "completed in " << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << endl;
    }
    return 0;
}
