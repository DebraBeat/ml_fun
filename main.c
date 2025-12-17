#include <stdio.h>
#include "matrix.h"

int main() {
    // Upper Triangular Matrix
    Matrix* U = create_empty_matrix(3, 3);
    U->data[0][0] = 1.0; U->data[0][1] = 1.0; U->data[0][2] = 1.0;
    U->data[1][0] = 0.0; U->data[1][1] = 1.0; U->data[1][2] = 1.0;
    U->data[2][0] = 0.0; U->data[2][1] = 0.0; U->data[2][2] = 1.0;

    Matrix* A = gauss_jordan_elimination(U);
    print_matrix(A);

    free_matrix(U);
    if (A) free_matrix(A);

    return 0;
}