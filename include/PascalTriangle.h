#ifndef PASCALTRIANGLE_H
#define PASCALTRIANGLE_H

template <int N>
class PascalTriangle {
public:
    PascalTriangle() {
        triangle = new int*[N];
        for(int n = 0; n < N; n++) {
            triangle[n] = new int[n + 1];
        }
        for(int n = 0; n < N; n++) {
            for(int k = 0; k < n + 1; k++) {
                if(n == k || k == 0) {
                    triangle[n][k] = 1;
                } else {
                    triangle[n][k] = triangle[n - 1][k - 1] + triangle[n - 1][k];
                }
            }
        }
    }
    ~PascalTriangle() {
        for(int k = 0; k < N; k++) {
            delete[] triangle[k];
        }
        delete[] triangle;
    }

    int ncr(int n, int r) {
        if(n < r) {
            return 0;
        }
        return triangle[n][r];
    }
private:
    int** triangle;

};

#endif // PASCALTRIANGLE_H
