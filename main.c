#include <stdio.h>
#include "matrix.h"

int main() {
    Matrix* A = create_empty_matrix(1, 4);
    A->data[0][0] = 0.0; A->data[0][1] = 1.0; A->data[0][2] = 1.0; A->data[0][3] = 3.0;

    Matrix* B = create_empty_matrix(4, 1);
    B->data[0][0] = 0.0;
    B->data[1][0] = 1.0;
    B->data[2][0] = 2.0;
    B->data[3][0] = 3.0;

    Matrix* C = matrix_product(A, B);

    print_matrix(C);
    printf("row dim: %ld\n", C->rows);
    printf("row dim: %ld\n", A->rows);

    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
}