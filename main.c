#include <stdio.h>
#include "matrix.h"

int main() {
    // Upper Triangular Matrix
    Matrix* A = create_empty_matrix(2,2);
    A->data[0][0] = 1.0; A->data[0][1] = 2.0;
    A->data[1][0] = 3.0; A->data[1][1] = 4.0;
    print_matrix(A);

    Matrix* A_inv = invert(A);
    print_matrix(A_inv);

    Matrix* P = matrix_product(A, A_inv);
    print_matrix(P);

    free_matrix(A);
    free_matrix(A_inv);
    free_matrix(P);
    return 0;
}