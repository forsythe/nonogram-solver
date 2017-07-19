#include <iostream>
#include <iomanip>
#include <vector>
#include <bitset>
#include <cassert>


#include "utils.h"
#include "InputPrompter.h"
#include "PascalTriangle.h"

using namespace std;


const int WIDTH = 25;
const int HEIGHT = 25;
static PascalTriangle<WIDTH> p;

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
        if(p.ncr(n, k) <= thresh) {
            thresh -= p.ncr(n, k);
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
vector<int> convertSABtoBAB(vector<int> sab, int setsize) {
    vector<int> ans;
    int old = -1;
    for(std::vector<int>::const_reverse_iterator it = sab.rbegin(); it != sab.rend(); ++it) {
        ans.push_back(*it - old - 1);
        old = *it;
    }
    ans.push_back(setsize - old - 1);
    return ans;
}

/*
* Given a crosslogic row with hints, this function
* fills it with values and returns the row as a bitset,
* where: crosses are 0s, and blocks are 1s
*
* E.g. WIDTH = 10, blocks = {2, 3}, crosses={3, 1}
* the output will be 000(11)(0111)0, but without the ().
* The () is only used to indicate the groupings from hints.
* Note that every group of blocks after the first implicitly begin
* with a cross.
*/
bitset<WIDTH> fillRow(vector<int> crosses, vector<int> blocks) {
    //assert(crosses.size() == blocks.size() + 1);
    //assert(sum(crosses) + sum(blocks) + blocks.size() - 1 == WIDTH);
    bitset<WIDTH> ans;
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
    print_cl<WIDTH>(ans);
    return ans;
}

inline vector<bitset<WIDTH>> findLinePossibilities(vector<int> groups) {
    vector<bitset<WIDTH>> ans;


    int num_groups = groups.size();
    int num_blocks = sum(groups);

    int num_confirmed_xs = num_groups - 1;

    int balls = WIDTH - num_blocks - num_confirmed_xs;//balls

    int bins = num_groups + 1; //bins

    int n = balls + bins - 1;
    int k = bins - 1;
    //using stars and bars formula
    //combination with replacement
    cout << "balls = " << balls << endl;
    cout << "bins = " << bins << endl;
    cout << "n = " << n << endl;
    cout << "k = " << k << endl;

    int maxindex = p.ncr(n, k); //enumerate the combinations
    cout << "there are " << maxindex << " combinations" << endl;

    int nstart; //0 index, enumerate the spots
    int indexstart; //start here and go downwards to check other objects
    for(int index = 0; index < maxindex; ++index) {
        cout << "at index = " << setw(4) << index << ": ";
        nstart = n - 1; //start looking from the rightmost obj (largest enumueratedy)
        indexstart = index;
        vector<int> sab;
        for(int i = k; i > 0; --i) {
            sab.push_back(findBiggestN(nstart, i, indexstart));
            //cout << findBiggestN(nstart, i, indexstart) << " ";
        }
        //cout << "sab contains ";
        //print(sab);
        ans.push_back(fillRow(convertSABtoBAB(sab, n), groups));
    }
    assert(ans.size() == maxindex);
    return ans;
}

inline bitset<WIDTH> findCommonBlocks(vector<bitset<WIDTH>> v) {
    bitset<WIDTH> ans;
    ans.set();

    for(std::vector<bitset<WIDTH>>::const_iterator it = v.begin(); it != v.end(); ++it) {
        ans &= *it;
    }
    cout << "common blocks:   ";
    print_cl<ans.size()>(ans, ". ", "¢i");
    return ans;
}

inline bitset<WIDTH> findCommonCrosses(vector<bitset<WIDTH>> v) {
    bitset<WIDTH> ans;

    for(std::vector<bitset<WIDTH>>::const_iterator it = v.begin(); it != v.end(); ++it) {
        ans |= *it;
    }
    cout << "common crosses:  ";
    print_cl<ans.size()>(ans, "X ", ". ");
    return ans;
}

int main() {
    //InputPrompter.prompt_col();
    vector<int> hints;
    // hints.push_back(1);
    // hints.push_back(1);
    // hints.push_back(3);

    hints.push_back(5);
    hints.push_back(2);
    hints.push_back(3);


    vector<bitset<WIDTH>> ans = findLinePossibilities(hints);
    findCommonBlocks(ans);
    findCommonCrosses(ans);

    return 0;
}
