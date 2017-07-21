#ifndef PASCALTRIANGLE_H
#define PASCALTRIANGLE_H
#include <cassert>

template <int N>
class PascalTriangle {
public:
    PascalTriangle() {
        triangle = new int*[N];
        for(int n = 0; n < N; n++) {
            triangle[n] = new int[n + 1];
        }
        for(int n = 0; n < N; n++) {
            for(int row = 0; row < n + 1; row++) {
                if(n == row || row == 0) {
                    triangle[n][row] = 1;
                } else {
                    triangle[n][row] = triangle[n - 1][row - 1] + triangle[n - 1][row];
                }
            }
        }
    }
    ~PascalTriangle() {
        for(int row = 0; row < N; row++) {
            delete[] triangle[row];
        }
        delete[] triangle;
    }

    int operator()(int n, int r) const {
        assert(n<N);
        if(n < r) {
            return 0;
        }
        return triangle[n][r];
    }
private:
    int** triangle;

};

#endif // PASCALTRIANGLE_H
