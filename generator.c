#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MATRIX_FILE_NAME "full_rank_matrix.csv"
#define VECTOR_FILE_NAME "target_vector.csv"

/** @brief Generate a random 1000 by 1000 matrix CSV file and a 1000 
 * by 1 vector of the sum of each row in the maxtrix as a csv file as well.
 *  
 * @note This doubles as a testing method. An easy way to test we got the correct
 * results from our regression is to compare it to a vector of 1s. In order for
 * our regressor to correctly output 1s, that means that the target feature
 * should be the sum of the rows.
 * 
 * In other words, we are given y and X where y = Xb. We do not know b, but
 * are trying to predict it through our regressor. This prediction is called
 * b_hat.
 * 
 * If y is the sum of the values of each row of X, then b must be all 1s.
 * This makes it easy to see if our regressor is working correctly. If b_hat is
 * all 1s, then we can tell visually it is correct.

 * @note To establish a matrix that is full rank, that must mean it is 
 * invertible. An easy to tell if a matrix is invertible is if the diagonal
 * entries cannot be made 0 by a combination of the off diagonal entries
 * on the same column.
 * 
 * To do this, we make the off-diagonal entries bound between 0 and 1, and the
 * diagonal entries greater than the number of columns plus 1.
 * 
 * This strategy is called diagonal dominance
 * 
 * @return int
 */
int main(int argc, char* argv[]) {
    FILE *matrix_fp;
    FILE *vector_fp;
    int i, j;
    double matrix_val;
    double vector_val;
    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);

    srand(time(NULL));

    matrix_fp = fopen(MATRIX_FILE_NAME, "w");
    if (matrix_fp == NULL) {
        fprintf(stderr, "Cannot open file\n");
        return 1;
    }

    vector_fp = fopen(VECTOR_FILE_NAME, "w");
    if (vector_fp == NULL) {
        fprintf(stderr, "Cannot open file\n");
        return 1;
    }

    printf("Generating a %d by %d matrix...\n", rows, cols);
    printf("Generating a %d vector...", rows);

    for (i = 0; i < rows; i++) {
        vector_val = 0.0;

        for (j = 0; j < cols; j++) {
            matrix_val = (double)rand() / RAND_MAX; // Get a random double between [0.0, 1.0]

            // Diagonal Dominance implementation
            if (i == j) {
                matrix_val += (double)rows;
            }

            fprintf(matrix_fp, "%f", matrix_val);
            vector_val += matrix_val;

            if (j < cols - 1) {
                fprintf(matrix_fp, ",");
            }
        }

        fprintf(matrix_fp, "\n");
        
        fprintf(vector_fp, "%f", vector_val);
        if (i < rows - 1) {
            fprintf(vector_fp, ",");
        }
    }

    printf("Done\n");

    fclose(matrix_fp);
    fclose(vector_fp);
    return 0;
}