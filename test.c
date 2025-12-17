#include <stdio.h>
#include <stdlib.h>
#include <math.h> 

#include "matrix.h" 

// Mini Unit Testing Framework
#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; \
                               if (message) return message; } while (0)

int tests_run = 0;

// --- 2. Helper for File I/O Tests ---
// We create a temporary CSV file so we have something to read
void create_temp_csv(const char* filename, const char* content) {
    FILE* f = fopen(filename, "w");
    if (f) {
        fprintf(f, "%s", content);
        fclose(f);
    }
}

// --- 3. The Tests ---

static char* test_create_vector() {
    size_t rows = 5;
    Vector* v = create_empty_vector(rows);

    mu_assert("Vector should not be NULL", v != NULL);
    mu_assert("Vector rows should be 5", v->rows == rows);
    mu_assert("Vector data should not be NULL", v->data != NULL);

    // Verify calloc zeroed the memory
    mu_assert("Vector data should be initialized to 0", v->data[0] == 0.0);

    free_vector(v);
    return NULL;
}

static char* test_dot_product_math() {
    Vector* a = create_empty_vector(3);
    Vector* b = create_empty_vector(3);
    
    // Vector A: [1, 2, 3]
    a->data[0] = 1.0; a->data[1] = 2.0; a->data[2] = 3.0;
    
    // Vector B: [4, 5, 6]
    b->data[0] = 4.0; b->data[1] = 5.0; b->data[2] = 6.0;

    double result = 0.0;
    int status = dot_product(a, b, &result);

    // Math: (1*4) + (2*5) + (3*6) = 4 + 10 + 18 = 32
    mu_assert("Dot product returned error status", status == EXIT_SUCCESS);
    mu_assert("Dot product math is wrong", is_close(result, 32.0));

    free_vector(a);
    free_vector(b);
    return NULL;
}

static char* test_dot_product_mismatch() {
    Vector* a = create_empty_vector(2);
    Vector* b = create_empty_vector(5); // Different size
    double result = 0.0;

    // We expect this function to fail safely
    int status = dot_product(a, b, &result);

    mu_assert("Should return failure for mismatched sizes", status == EXIT_FAILURE);

    free_vector(a);
    free_vector(b);
    return NULL;
}

static char* test_csv_loading() {
    // 1. Setup: Create a real file on disk
    const char* filename = "test_data.csv";
    create_temp_csv(filename, "1.5,2.5,3.0");

    // 2. Action: Load it using your function
    Vector* v = create_vector_from_file((char*)filename);

    // 3. Assertion
    mu_assert("Vector failed to load from file", v != NULL);
    mu_assert("Row count incorrect", v->rows == 3);
    mu_assert("First element incorrect", is_close(v->data[0], 1.5));
    mu_assert("Second element incorrect", is_close(v->data[1], 2.5));
    mu_assert("Third element incorrect", is_close(v->data[2], 3.0));

    // 4. Cleanup
    free_vector(v);
    remove(filename); // Delete the temp file
    return NULL;
}

static char* test_create_matrix() {
    size_t rows = 3;
    size_t cols = 4;
    Matrix* m = create_empty_matrix(rows, cols);

    mu_assert("Matrix should not be NULL", m != NULL);
    mu_assert("Matrix rows wrong", m->rows == rows);
    mu_assert("Matrix cols wrong", m->cols == cols);
    mu_assert("Matrix data array NULL", m->data != NULL);
    mu_assert("Matrix row 0 array NULL", m->data[0] != NULL);

    // Verify calloc zeroed the memory
    mu_assert("Matrix data should be initialized to 0", m->data[0][0] == 0.0);

    free_matrix(m);
    return NULL;
}

static char* test_copy_matrix() {
    Matrix* A = create_matrix_from_file("my_matrix.csv");
    Matrix *B = copy_matrix(A);
    
    for (size_t i = 0; i < A->rows; i++) {
        for (size_t j = 0; j < A->cols; j++) {
            mu_assert("Copy working incorrectly", A->data[i][j] == B->data[i][j]);
        }
    }

    Matrix* C = NULL;

    mu_assert("Tried to Copy NULL", copy_matrix(C) == NULL);

    free(A);
    free(B);
    return NULL;
}
static char* test_matrix_transpose() {
    // 1. Setup 2x3 Matrix
    // [ 1.0, 2.0, 3.0 ]
    // [ 4.0, 5.0, 6.0 ]
    Matrix* m = create_empty_matrix(2, 3);
    m->data[0][0] = 1.0; m->data[0][1] = 2.0; m->data[0][2] = 3.0;
    m->data[1][0] = 4.0; m->data[1][1] = 5.0; m->data[1][2] = 6.0;

    // 2. Action
    Matrix* t = tranpose_matrix(m);

    // 3. Assertions (Should be 3x2)
    // [ 1.0, 4.0 ]
    // [ 2.0, 5.0 ]
    // [ 3.0, 6.0 ]
    mu_assert("Transpose rows wrong", t->rows == 3);
    mu_assert("Transpose cols wrong", t->cols == 2);
    
    // Check corners
    mu_assert("Transpose [0][1] wrong", is_close(t->data[0][1], 4.0)); 
    mu_assert("Transpose [2][0] wrong", is_close(t->data[2][0], 3.0));

    free_matrix(m);
    free_matrix(t);
    return NULL;
}

static char* test_matrix_csv_load() {
    // 1. Setup: Create a 2x2 CSV
    const char* filename = "test_matrix.csv";
    create_temp_csv(filename, "1.0,2.0\n3.0,4.0");

    // 2. Action
    Matrix* m = create_matrix_from_file((char*)filename);

    // 3. Assertions
    mu_assert("Matrix failed to load from file", m != NULL);
    mu_assert("Loaded rows wrong", m->rows == 2);
    mu_assert("Loaded cols wrong", m->cols == 2);
    mu_assert("Value at [0][0] wrong", is_close(m->data[0][0], 1.0));
    mu_assert("Value at [1][1] wrong", is_close(m->data[1][1], 4.0));

    // 4. Cleanup
    free_matrix(m);
    remove(filename);
    return NULL;
}

static char* test_matrix_vector_product() {
    // Setup Matrix (2x2 Identity Matrix scaled by 2)
    // [ 2, 0 ]
    // [ 0, 2 ]
    Matrix* A = create_empty_matrix(2, 2);
    A->data[0][0] = 2.0; A->data[0][1] = 0.0;
    A->data[1][0] = 0.0; A->data[1][1] = 2.0;

    // Setup Vector [ 3, 4 ]
    Vector* x = create_empty_vector(2);
    x->data[0] = 3.0;
    x->data[1] = 4.0;

    // Action: b = Ax
    Vector* b = matrix_vector_product(A, x);

    // Assertion: Result should be [ 6, 8 ]
    mu_assert("Result vector is NULL", b != NULL);
    mu_assert("Result dimension wrong", b->rows == 2);
    mu_assert("Result index 0 wrong", is_close(b->data[0], 6.0));
    mu_assert("Result index 1 wrong", is_close(b->data[1], 8.0));

    free_matrix(A);
    free_vector(x);
    free_vector(b);
    return NULL;
}

static char* test_mv_product_mismatch() {
    Matrix* A = create_empty_matrix(2, 2); // 2 Cols
    Vector* x = create_empty_vector(3);    // 3 Rows

    Vector* b = matrix_vector_product(A, x);

    // Should return NULL on size mismatch
    mu_assert("Should return NULL for incompatible dimensions", b == NULL);

    free_matrix(A);
    free_vector(x);
    // No need to free b, it should be NULL
    return NULL;
}

static char* test_matrix_product() {
    Matrix* I = create_empty_matrix(2, 2);
    I->data[0][0] = 1.0; I->data[0][1] = 0.0;
    I->data[1][0] = 0.0; I->data[1][1] = 1.0;

    Matrix* A = create_empty_matrix(1, 4);
    A->data[0][0] = 0.0; A->data[0][1] = 1.0; A->data[0][2] = 1.0; A->data[0][3] = 3.0;

    Matrix* B = create_empty_matrix(4, 1);
    B->data[0][0] = 0.0;
    B->data[1][0] = 1.0;
    B->data[2][0] = 2.0;
    B->data[3][0] = 3.0;

    Matrix* C = matrix_product(A, B);
    Matrix* D = matrix_product(A, A);
    Matrix* E = matrix_product(I, I);

    mu_assert("Result index [0][0] is wrong", is_close(E->data[0][0], 1.0));
    mu_assert("Result index [1][0] is wrong", is_close(E->data[1][0], 0.0));
    mu_assert("Row dimension wrong", C->rows == A->rows);
    mu_assert("Column dimension wrong", C->cols == B->cols);
    mu_assert("Result dimension wrong, shouldn't work but is", D == NULL);

    free_matrix(I);
    free_matrix(A);
    free_matrix(B);
    if (C) free_matrix(C);
    if (D) free_matrix(D);
    if (E) free_matrix(E);

    return NULL;
}

static char* test_swap_rows() {
    Matrix* A = create_empty_matrix(2, 2);
    A->data[0][0] = 1.0; A->data[0][1] = 0.0;
    A->data[1][0] = 0.0; A->data[1][0] = 1.0;

    swap_rows(A, 0, 1);

    mu_assert("Swap did not happen", A->data[0][0] == 1.0 && A->data[0][1] == 0.0);

    return NULL;
}
static char* test_gj_elimination() {
    // Upper Triangular Matrix
    Matrix* U = create_empty_matrix(3, 3);
    U->data[0][0] = 1.0; U->data[0][1] = 1.0; U->data[0][2] = 1.0;
    U->data[1][0] = 0.0; U->data[1][1] = 1.0; U->data[1][2] = 1.0;
    U->data[2][0] = 0.0; U->data[2][1] = 0.0; U->data[2][2] = 1.0;
    Matrix* A = gauss_jordan_elimination(U);

    mu_assert("T1: Results index[0][1] is wrong", is_close(A->data[0][1], 0.0));
    mu_assert("T1: Results index[0][2] is wrong", is_close(A->data[0][2], 0.0));
    mu_assert("T1: Results index[1][2] is wrong", is_close(A->data[1][2], 0.0));
    mu_assert("T1: Results index[1][1] is wrong", is_close(A->data[0][0], 1.0));
    mu_assert("T1: Results index[2][2] is wrong", is_close(A->data[1][1], 1.0));
    mu_assert("T1: Results index[3][3] is wrong", is_close(A->data[2][2], 1.0));

    if (A) free_matrix(A);

    U->data[0][0] = 2.0; U->data[0][1] = 3.0; U->data[0][2] = 4.0;
    U->data[1][0] = 0.0; U->data[1][1] = 4.0; U->data[1][2] = 5.0;
    U->data[2][0] = 0.0; U->data[2][1] = 0.0; U->data[2][2] = 6.0;
    A = gauss_jordan_elimination(U);

    mu_assert("T2: Results index[0][1] is wrong", is_close(A->data[0][1], 0.0));
    mu_assert("T2: Results index[0][2] is wrong", is_close(A->data[0][2], 0.0));
    mu_assert("T2: Results index[1][2] is wrong", is_close(A->data[1][2], 0.0));
    mu_assert("T2: Results index[0][0] is wrong", is_close(A->data[0][0], 1.0));
    mu_assert("T2: Results index[1][1] is wrong", is_close(A->data[1][1], 1.0));
    mu_assert("T2: Results index[2][2] is wrong", is_close(A->data[2][2], 1.0));

    if (A) free_matrix(A);

    U->data[0][0] = 2.0; U->data[0][1] = 3.0; U->data[0][2] = 4.0;
    U->data[1][0] = 5.0; U->data[1][1] = 6.0; U->data[1][2] = 7.0;
    U->data[2][0] = 8.0; U->data[2][1] = 9.0; U->data[2][2] = 10.0;
    A = gauss_jordan_elimination(U);

    mu_assert("T3: Results index[0][1] is wrong", is_close(A->data[0][1], 0.0));
    mu_assert("T3: Results index[0][2] is wrong", is_close(A->data[0][2], -1.0));
    mu_assert("T3: Results index[1][2] is wrong", is_close(A->data[1][2], 2.0));
    mu_assert("T3: Results index[0][0] is wrong", is_close(A->data[0][0], 1.0));
    mu_assert("T3: Results index[1][1] is wrong", is_close(A->data[1][1], 1.0));
    mu_assert("T3: Results index[2][2] is wrong", is_close(A->data[2][2], 0.0));
    if (A) free_matrix(A);

    Matrix* V = create_empty_matrix(4, 3);
    V->data[0][0] = 2.0; V->data[0][1] = 3.0; V->data[0][2] = 4.0;
    V->data[1][0] = 5.0; V->data[1][1] = 6.0; V->data[1][2] = 7.0;
    V->data[2][0] = 8.0; V->data[2][1] = 9.0; V->data[2][2] = 10.0;
    V->data[3][0] = 8.0; V->data[3][1] = 9.0; V->data[3][2] = 10.0;
    Matrix* B = gauss_jordan_elimination(V);

    mu_assert("T4: Results index[0][1] is wrong", is_close(B->data[0][1], 0.0));
    mu_assert("T4: Results index[0][2] is wrong", is_close(B->data[0][2], -1.0));
    mu_assert("T4: Results index[1][2] is wrong", is_close(B->data[1][2], 2.0));
    mu_assert("T4: Results index[0][0] is wrong", is_close(B->data[0][0], 1.0));
    mu_assert("T4: Results index[1][1] is wrong", is_close(B->data[1][1], 1.0));
    mu_assert("T4: Results index[2][2] is wrong", is_close(B->data[2][2], 0.0));
    mu_assert("T4: Results index[3][2] is wrong", is_close(B->data[3][2], 0.0));
 
    free_matrix(V);
    if (B) free_matrix(B);

    return NULL;
}
// --- 4. Test Runner ---

static char* all_tests() {
    mu_run_test(test_create_vector);
    mu_run_test(test_dot_product_math);
    mu_run_test(test_dot_product_mismatch);
    mu_run_test(test_csv_loading);

    mu_run_test(test_create_matrix);
    mu_run_test(test_copy_matrix);
    mu_run_test(test_matrix_transpose);
    mu_run_test(test_matrix_csv_load);
    mu_run_test(test_matrix_vector_product);
    mu_run_test(test_mv_product_mismatch);
    mu_run_test(test_matrix_product);
    mu_run_test(test_swap_rows);
    mu_run_test(test_gj_elimination);
    return NULL;
}

int main() {
    printf("Starting Vector Tests...\n");
    
    char *result = all_tests();
    
    if (result != 0) {
        printf("\nFAILED: %s\n", result);
    } else {
        printf("\nALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}