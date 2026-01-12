#include <stdio.h>
#include "regressions.h"

int main(int argc, char* argv[]) {
    Matrix* X = create_matrix_from_file(argv[1]);
    Vector* y = create_vector_from_file(argv[2]);
    printf("y size: %ld\n", y->rows);
    Vector* b_hat = ols(X, y);

    double mae_result;
    Vector* b = create_empty_vector(b_hat->rows);
    for (size_t i = 0; i < b->rows; i++) {
        b->data[i] = 1.0;
    }
    mae(b, b_hat, &mae_result);

    // printf("The ordinary least squares regression is: ");
    // print_vector(b_hat);
    printf("The MAE is: %f\n", mae_result);
    free_matrix(X);
    free_vector(y);
    free_vector(b_hat);
    return 0;
}