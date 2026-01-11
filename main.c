#include <stdio.h>
#include "regressions.h"

int main(int argc, char* argv[]) {
    Matrix* X = create_matrix_from_file(argv[0]);
    Vector* y = create_vector_from_file(argv[1]);
    Vector* b_hat = ols(X, y);

    printf("The ordinary least squares regression is: ");
    print_vector(b_hat);
    free_matrix(X);
    free_vector(y);
    free_vector(b_hat);
    return 0;
}