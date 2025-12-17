#include <stdio.h>
#include "matrix.h"

int main() {
    // Upper Triangular Matrix
    Matrix* U = create_empty_matrix(3, 3);
    U->data[0][0] = 2.0; U->data[0][1] = 3.0; U->data[0][2] = 4.0;
    U->data[1][0] = 5.0; U->data[1][1] = 6.0; U->data[1][2] = 7.0;
    U->data[2][0] = 8.0; U->data[2][1] = 9.0; U->data[2][2] = 10.0;

    Matrix* A = gauss_jordan_elimination(U);
    print_matrix(A);

    free_matrix(U);
    if (A) free_matrix(A);

    return 0;
}