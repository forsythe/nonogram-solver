#include <iostream>
#include <iomanip>
#include <vector>
#include <bitset>
#include <cassert>
#include <unordered_set>

#include "utils.h"
#include "Grid.h"
#include "InputPrompter.h"
#include "PascalTriangle.h"

using namespace std;


const int WIDTH = 15;
const int HEIGHT = 15;
static PascalTriangle<2 * WIDTH> ncr;
static Grid<WIDTH, HEIGHT> grid;

vector<unordered_set<int>> ignore_index_row(HEIGHT, unordered_set<int>());
vector<unordered_set<int>> ignore_index_col(WIDTH, unordered_set<int>());
unordered_set<int> finished_row;
unordered_set<int> finished_col;

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
inline int findBiggestN(int& nstart, int k, int& thresh) {
    for(int n = nstart; n >= 0; --n) {
        if(ncr(n, k) <= thresh) {
            thresh -= ncr(n, k);
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
    //assert(crosses.size() == blocks.size() + 1);
    //assert(sum(crosses) + sum(blocks) + blocks.size() - 1 == WIDTH);
    bitset<SIZE> ans;
    int sz = blocks.size();
    //can ignore first group of crosses, since the entire row starts as crosses (0)
    for(int k = 0; k < sz; k++) {
        if(k > 0) { //every group after the first should have an X before it
            ans <<= 1; //implicitly put an X
        }
        for(int j = 0; j < blocks[k]; j++) {
            ans <<= 1;
            ans.set(0, true);
        }
        ans <<= (crosses[k + 1]);
    }
    //cout << "filled row looks like: ";
    //cout << ans << endl;
    // print_cl<SIZE>(ans);
    return ans;
}

/*
* Given some crosslogic hints, finds possibilities for this row or column.
* Also checks to skip known impossible rows/cols, and will add new ones
* if necessary by comparing it to known to be correct values
*/
template <int SIZE>
inline vector<bitset<SIZE>> findLinePossibilities(const vector<int>& blocks,
                         unordered_set<int>& bad_index,
                         const bitset<SIZE>& confirmed_blocks,
const bitset<SIZE>& confirmed_crosses) {
    vector<bitset<SIZE>> ans;
    int num_groups = blocks.size();
    int num_blocks = sum(blocks);
    int num_confirmed_xs = num_groups - 1;
    int balls = SIZE - num_blocks - num_confirmed_xs;//balls
    int bins = num_groups + 1; //bins
    int n = balls + bins - 1;
    int k = bins - 1;
//    cout << "balls = " << balls << endl;
//    cout << "bins = " << bins << endl;
//    cout << "n = " << n << endl;
//    cout << "k = " << k << endl;
    int maxindex = ncr(n, k); //how many possible combinations
//    cout << "there are <=" << maxindex << " combinations" << endl;
    int nstart; //0 index, enumerate the items to be chosen (k of them, from n total)
    int indexstart; //start here and go upwards to check new k-combinations
    for(int index = 0; index < maxindex; ++index) {
//        cout << "at index = " << setw(4) << index << ": ";
        if(bad_index.find(index) != bad_index.end()) {
//            cout << "skipping..." << endl;
            continue;
        }
        nstart = n - 1; //start looking from the rightmost obj (largest enumueratedy)
        indexstart = index;
        vector<int> sab;
        for(int i = k; i > 0; --i) {
            sab.push_back(findBiggestN(nstart, i, indexstart));
            //cout << findBiggestN(nstart, i, indexstart) << " ";
        }
        //cout << "sab contains ";
        //print(sab);
        bitset<SIZE> temp = fillLine<SIZE>(convertSABtoBAB(sab, n), blocks);
//        cout << "temp: " << temp << endl;
//        cout << "confirmed blocks " << confirmed_blocks << endl;
//        cout << "confirmed crosses " << confirmed_crosses << endl;
        if(((temp & confirmed_blocks) == confirmed_blocks)
                && ((temp | confirmed_crosses) == confirmed_crosses)) {
            ans.push_back(temp);
        } else {
            bad_index.insert(index);
        }
    }
    return ans;
}

template<int SIZE>
inline bitset<SIZE> findCommonBlocks(const vector<bitset<SIZE>>& v) {
    bitset<SIZE> ans;
    ans.set(); //make all 1s
    for(typename vector<bitset<SIZE>>::const_iterator it = v.begin(); it != v.end();
            ++it) {
        ans &= *it;
    }
    //cout << "common blocks:   ";
    //print_cl<ans.size()>(ans, ".", "#");
    return ans;
}

template<int SIZE>
inline bitset<SIZE> findCommonCrosses(const vector<bitset<SIZE>>& v) {
    bitset<SIZE> ans;
    for(typename vector<bitset<SIZE>>::const_iterator it = v.begin(); it != v.end();
            ++it) {
        ans |= *it;
    }
    //cout << "common crosses:  ";
    //print_cl<ans.size()>(ans, "X", ".");
    return ans;
}

int main() {
    vector<vector<int>> col_hints = InputPrompter::prompt<COLUMN, WIDTH>();
    vector<vector<int>> row_hints = InputPrompter::prompt<ROW, HEIGHT>();
    bool changed = false;
    while(true) {
        changed = false;
        cout << grid << endl;
        for(int row = 0; row < HEIGHT ; row++) {
            //cout << "on row " << row << endl;
            if(finished_row.find(row) != finished_row.end()) {
                //cout << "done this row..." << endl;
                continue;
            }
            vector<bitset<WIDTH>> candidates = findLinePossibilities<WIDTH>(row_hints[row],
                                               ignore_index_row[row],
                                               grid.getRow(row, BLOCK), grid.getRow(row, CROSS));
            if(candidates.size() == 1) {
                finished_row.insert(row);
                //cout << "****************************** finished row " << row << endl;
                grid.setRow(row, candidates[0]);
                //cout << grid << endl;
                changed = true;
                continue;
            }
            bitset<WIDTH> commonBlocks = findCommonBlocks<WIDTH>(candidates);
            bitset<WIDTH> commonCrosses = findCommonCrosses<WIDTH>(candidates);
            if(commonBlocks.any()) {
                //cout << "there were some common blocks!" << endl;
                for(int col = 0; col < WIDTH; col++) {
                    if(commonBlocks[WIDTH - col - 1] == BLOCK && grid[row][col] == UNKNOWN) { //iterate backwards
                        grid[row][col] = BLOCK;
                        changed = true;
                    }
                }
                //cout << grid << endl;
            }
            if(!commonCrosses.all()) {
                //out << "there were some common crosses!" << endl;
                for(int col = 0; col < WIDTH; col++) {
                    if(commonCrosses[WIDTH - col - 1] == CROSS && grid[row][col] == UNKNOWN) { //iterate backwards
                        grid[row][col] = CROSS;
                        changed = true;
                    }
                }
                //cout << grid << endl;
            }
        }
        for(int col = 0; col < WIDTH ; col++) {
            //cout << "on col " << col << endl;
            if(finished_col.find(col) != finished_col.end()) {
                //cout << "done this col..." << endl;
                continue;
            }
            vector<bitset<HEIGHT>> candidates = findLinePossibilities<HEIGHT>(col_hints[col],
                                                ignore_index_col[col],
                                                grid.getCol(col, BLOCK), grid.getCol(col, CROSS));
            bitset<HEIGHT> commonBlocks = findCommonBlocks<HEIGHT>(candidates);
            bitset<HEIGHT> commonCrosses = findCommonCrosses<HEIGHT>(candidates);
            if(candidates.size() == 1) {
                finished_col.insert(col);
                //cout << "****************************** finished col " << col << endl;
                grid.setCol(col, candidates[0]);
                changed = true;
                //cout << grid << endl;
                continue;
            }
            if(commonBlocks.any()) {
                // cout << "there were some common blocks!" << endl;
                for(int row = 0; row < HEIGHT; row++) {
                    if(commonBlocks[HEIGHT - row - 1] == BLOCK && grid[row][col] == UNKNOWN) {
                        grid[row][col] = BLOCK;
                        changed = true;
                    }
                }
                //cout << grid << endl;
            }
            if(!commonCrosses.all()) {
                //cout << "there were some common crosses!" << endl;
                for(int row = 0; row < HEIGHT; row++) {
                    if(commonCrosses[HEIGHT - row - 1] == CROSS && grid[row][col] == UNKNOWN) {
                        grid[row][col] = CROSS;
                        changed = true;
                    }
                }
            }
            //cout << grid << endl;
        }
        if(!changed) {
            break;
        }
    }
    cout << grid << endl;
    //print(grid.getRow(0, CROSS));
//    vector<bitset<WIDTH>> ans = findLinePossibilities(hints);
//    findCommonBlocks(ans);
//    findCommonCrosses(ans);
    return 0;
}
