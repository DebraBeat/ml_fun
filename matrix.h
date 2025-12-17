#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "vector.h"

#define MAX_LINE_LENGTH 1024
#define MIN(a,b) (a < b ? (a) : (b))

/** TODO:
*
* DONE: Transpose
* DONE: Matrix-Vector product
* DONE: Matrix-Matrix product
* Gaussian Elimination
* Inverse
* Pseudoinverse
* QR decomposition
* Eigen decomposition
* SVD
*
*/

/**
 * @struct A structure encapsulating a row by column mathematical matrix
 */
typedef struct Matrix {
    size_t rows;
    size_t cols;
    double** data;
} Matrix;

/**
 * @brief Create a matrix of size rows by columns
 * 
 * @param rows The number of rows in the matrix to create.
 * @param cols The number of cols in the matrix to create.
 * @return *Matrix A reference to the matrix
 */
Matrix* create_empty_matrix(size_t rows, size_t cols) {
    Matrix* mat = (Matrix*)malloc(sizeof(Matrix));
    mat->rows = rows;
    mat->cols = cols;
    mat->data = (double**)calloc(rows, sizeof(double*));

    if (mat->data == NULL) {
        free(mat);
        return NULL;
    }

    for (size_t i= 0; i < rows; i++) {
        mat->data[i] = (double*)calloc(cols, sizeof(double));
    }

    return mat;
}

/**
 * @brief Return a copy of the given matrix
 * 
 * @param A The matrix to be copied
 * @return *Matrix A reference to the copied matrix
 */
Matrix* copy_matrix(Matrix* A) {
    if (A == NULL) {
        fprintf(stderr, "The given matrix does not exist.");
        return NULL;
    }

    Matrix* B = create_empty_matrix(A->rows, A->cols);
    for (size_t i = 0; i < A->rows; i++) {
        for (size_t j = 0; j < A->cols; j++) {
            B->data[i][j] = A->data[i][j];
        }
    }

    return B;

}
/**
 * @brief Free the memory a matrix is occupying
 * 
 * @param mat A pointer to the matrix
 * @return void
 */
void free_matrix(Matrix* mat) {
    for (size_t i = 0; i < mat->rows; i++) {
        free(mat->data[i]);
    }
    free(mat->data);
    free(mat);
}

/**
 * @brief Populate a given matrix from a given CSV file
 * 
 * @param mat A pointer to the matrix
 * @param file_name A pointer to the file name
 * @return void
 */
void print_matrix(Matrix* mat) {
    printf("[\n");
    for (size_t i = 0; i < mat->rows; i++) {
        printf("[");
        for (size_t j = 0; j < mat->cols; j++) {
            printf("%g, ", mat->data[i][j]);
        }
        printf("],\n");
    }
    printf("]\n");
}

/**
 * @brief Populate a given matrix from a given CSV file
 * 
 * @param mat A pointer to the matrix
 * @param file_name A pointer to the file name
 * @return integer representing status of population creation
 */
static int _populate_matrix(Matrix* mat, char* file_name) {
    FILE* file_pointer;
    char line_buffer[MAX_LINE_LENGTH];
    
    size_t cols = mat->cols; // number of cols
    *line_buffer = '\0';
    size_t i = 0; // Current row in matrix
    size_t j = 0; // Currsent column in matrix

    file_pointer = fopen(file_name, "r");
    if (file_pointer == NULL) {
        perror("Unable to open file");
        return EXIT_FAILURE;
    }

    while(fgets(line_buffer, MAX_LINE_LENGTH, file_pointer) != NULL) {
        char* token; // array of matrix values (as chars)
        token = strtok(line_buffer, ",");

        while (token != NULL) {
            mat->data[i][j] = atof(token);

            if (j == cols - 1) {
                i += 1;
                j = 0;
            } else {
                j++;
            }

            token = strtok(NULL, ",");
        }
    }

    fclose(file_pointer);
    return EXIT_SUCCESS;
}

/**
 * @brief Get the dimensions of a matrix from it's CSV file
 * 
 * @param file_name A pointer to the file name
 * @param dims Dimensions we will find of CSV file
 * @return void, dimensions of matrix is passed to dims paramters.
 */
static void _put_matrix_dimensions(char *file_name, int dims[2]) {
    char line_buffer[MAX_LINE_LENGTH];
    FILE *file_pointer;

    *line_buffer = '\0';
    dims[0] = 0;
    dims[1] = 0;

    file_pointer = fopen(file_name, "r");
    if (file_pointer == NULL) {
        perror("Unable to open file");
        return;
    }

    while(fgets(line_buffer, MAX_LINE_LENGTH, file_pointer) != NULL) {
        // If we're past the first the first row, don't bother counting the number of columns
        // This is a potential bug if our CSV is not rectangular
        if (dims[0] > 0) {
            dims[0]++; 
            continue;
        }

        line_buffer[strcspn(line_buffer, "\n")] = 0; // Set the newline character to NULL
        char* token; // array of matrix values (as chars)
        token = strtok(line_buffer, ",");

        while (token != NULL) {
            dims[1]++;
            token = strtok(NULL, ",");
        }

        dims[0]++;
 
    }

    fclose(file_pointer);
}

/**
 * @brief Create a matrix from a file
 * 
 * Create a matrix from a file using a combination of methods
 * 
 * @param file_name The name of the CSV file to create the matrix from
 * 
 * @return Matrix*
 * @note The caller is reponsible for freeing this memory using free_matrix()
 */
Matrix* create_matrix_from_file(char* file_name) {
    int dims[2] = {0, 0};
    _put_matrix_dimensions(file_name, dims);

    Matrix* mat = create_empty_matrix(dims[0], dims[1]);
    _populate_matrix(mat, file_name);

    return mat;
}

/**
 * @brief Transpose a matrix
 * 
 * @param mat The matrix to be transposed
 * 
 * @return Matrix*
 * @note The caller is responsible for freeing this memory using free_matrix()
 */
Matrix* tranpose_matrix(Matrix* mat) {
    // transposed rows and cols
    size_t t_rows = mat->cols;
    size_t t_cols = mat->rows;

    Matrix* tranpose = create_empty_matrix(t_rows, t_cols);

    for (size_t i = 0; i < mat->cols; i++) {
        for (size_t j = 0; j < mat->rows; j++) {
            tranpose->data[i][j] = mat->data[j][i];
        }
    }

    return tranpose;
}

/**
 * @brief Compute the Matrix-vector product
 * 
 * @param A The lefthand matrix
 * @param x The righthand vector
 * 
 * @return Vector*
 * @note The caller is responsible for freeing this memory using free_vector()
 */
Vector* matrix_vector_product(Matrix* A, Vector* x) {
    // If we get a size mismatch return null
    if (A->cols != x->rows) {
        fprintf(stderr, "Matrix Vector Product: The matrix and vector have incompatible sizes\n");
        return NULL;
    }

    // Number of rows for the new vector
    size_t v_rows = A->rows;
    
    Vector* b = create_empty_vector(v_rows);

    for(size_t i = 0; i < A->rows; i++) {
        for (size_t j = 0; j < A->cols; j++) {
            b->data[i] += (A->data[i][j] * x->data[j]);
        }
    }

    return b;
}

/**
 * @brief Compute the Matrix product (Matrix multiplication) through textbook definition O(n^3)
 * 
 * @param A The lefthand matrix
 * @param A The righthand matrix
 * 
 * @return Matrix
 * @note Matrix multiplication is not communative. AB != BA
 * @note The caller is responsible for freeing this memory using free_matrix()
 */
Matrix* matrix_product(Matrix* A, Matrix* B) {
    if (A->cols != B->rows) {
        fprintf(stderr, "Matrix Product: The matrices have incompatible sizes\n");
        fprintf(stderr, "A rows: %zu, A cols: %zu, B rows: %zu, B cols: %zu\n",
            A->rows, A->cols, B->rows, B->cols);
        return NULL;
    }

    Matrix* C = create_empty_matrix(A->rows, B->cols);
    if (C == NULL) return NULL;
    
    for (size_t i = 0; i < A->rows; i++) {
        for (size_t j = 0; j < B->cols; j++) {
            double sum = 0.0;

            for (size_t k = 0; k < B->rows; k++) {
                sum += (A->data[i][k] * B->data[k][j]);
            }
        
            C->data[i][j] = sum;
        }
    }
    return C;
}

/**
 * @brief Perform Gauss-Jordan Elimination on a given matrix A, putting it in
 * Reduced Row Echelon Form (RREF)
 * 
 * - If A is non-singular, the RREF of A will be the identity matrix
 * 
 * - If A is singular, the RREF of A will have a row of all zeros.
 * 
 * @param A The matrix to put into RREF
 * @return Matrix*
 */
Matrix* gauss_jordan_elimination(Matrix* A) {
    Matrix* R = copy_matrix(A);
    size_t diagonal_len = MIN(R->rows, R->cols);

    // Work our way down the main diagonal. If the element in the current
    // main diagonal is 0, increase curr to check the one below it
    for (size_t i = 0; i < diagonal_len; i++) {
        size_t curr = i;
        while (curr < R->rows && R->data[curr][i] == 0) {
            curr++;
        }

        // The current column has only zero values, so we move onto the
        // next diagonal element
        if (curr == R->rows) {
            continue;
        }

        // Start at the top row, zeroing out the corresponding cell
        // If we're at the same row, skip it so we don't zero it out :)
        for (size_t j = 0; j < R->rows; j++) {
            if (j == curr) {
                continue;
            }

            // Get the factor for zeroing out a row
            double factor = R->data[curr][i] / R->data[j][i];

            for (size_t k = 0; k < R->cols; k++) {
                R->data[curr][k] -= (factor * R->data[curr][k]);
            }
        }
    }

    return R;
}