#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "matrix.h"

/**
 * @brief Compute the Ordinary Least Squares Regression
 * 
 * @param A An m x n matrix of observations
 * @param b An m x 1 vector of target observations
 * 
 * @return Vector* x_hat, an m x 1 vector
 * @note The caller is reponsible for freeing this memory using free_vector()
 * 
 * A must have full column rank to have a unique solution
 */
Vector* ols(Matrix* A, Vector* b) {
    size_t rows = A->rows;
    size_t cols = A->cols;

    // Test to make sure A has full column rank
    Matrix* R = gauss_jordan_elimination(A);
    for(size_t i = 0; i < MIN(rows, cols); i++) {
        if (fabs(R->data[i][i]) < 1e-9) {
            fprintf(stderr, "A does not have full column rank.\n");
            return NULL;
        }
    }
    free_matrix(R);

    printf("Performing OLS...\n");

    // Calculate OLS via Moore-Penrose pseudo-inverse
    Matrix* At = tranpose_matrix(A);
    Matrix* AtA = matrix_product(At, A);
    Matrix* AtA_inv = invert(AtA);
    Vector* Atb = matrix_vector_product(At, b);
    Vector* x_hat = matrix_vector_product(AtA_inv, Atb);

    free_vector(Atb);
    free_matrix(AtA_inv);
    free_matrix(AtA);
    free_matrix(At);

    printf("Done\n");
    return x_hat;
}