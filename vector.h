#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024
/**
 * @struct A structure to encapsulate a basic mathematical vector
 */
typedef struct Vector {
    size_t rows;
    double *data;
} Vector;

/**
 * @brief Constructor for a vector
 * @param rows The number of rows in the vector
 * 
 * @return A pointer to a vector.
 */
Vector* create_empty_vector(size_t rows) {
    Vector* vec = (Vector*)malloc(sizeof(Vector));
    vec->rows = rows;
    vec->data = (double*)calloc(rows, sizeof(double));

    if (vec->data == NULL) {
        free(vec);
        return NULL;
    }

    return vec;
}

/**
 * @brief Destructor for a vector
 * @param vec A pointer to the vector to free in memory
 * 
 * @return void
 */
void free_vector(Vector* vec) {
    free(vec->data);
    free(vec);
}

/**
 * @brief Print a vector
 * @param mat A pointer to the matrix
 *
 * @return void
 */
void print_vector(Vector* vec) {
    printf("[");
    for (size_t i = 0; i < vec->rows; i++) {
        printf("%g, ", vec->data[i]);
    }
    printf("]\n");
}

/**
 * @brief Populate a vector from a CSV file
 * 
 * @param vec A pointer to the vector
 * @param file_name The name of the CSV file to populate from
 * 
 * @return integer representing success status.
 */
static int _populate_vector(Vector* vec, char* file_name) {
    FILE* file_pointer;
    char buffer[MAX_LINE_LENGTH];
    
    *buffer = '\0';
    size_t i = 0; // current row

    file_pointer = fopen(file_name, "r");
    if (file_pointer == NULL) {
        perror("Unable to open file\n");
        return EXIT_FAILURE;
    }

    fgets(buffer, MAX_LINE_LENGTH, file_pointer);
    char* token = strtok(buffer, ",");
    while (token != NULL) {
        vec->data[i] = atof(token);

        i++;
        token = strtok(NULL, ",");
    }

    fclose(file_pointer);
    return EXIT_SUCCESS;
}

/**
 * @brief Get number of rows of vector (the dimension)
 * 
 * @param file_name The name of the CSV file to get the dimension from
 * @param n_rows An integer to put the number of rows in
 * 
 * @return void
 */
static void _put_vector_dimension(char* file_name, int* n_rows) {
    FILE* file_pointer;
    char buffer[MAX_LINE_LENGTH];

    *buffer = '\0';
    // set n_rows to 0 for safety
    *n_rows = 0;

    file_pointer = fopen(file_name, "r");
    if (file_pointer == NULL) {
        perror("Unable to open file\n");
        return;
    }

    fgets(buffer, MAX_LINE_LENGTH, file_pointer);
    char* token = strtok(buffer, ",");

    while (token != NULL) {
        (*n_rows)++;
        token = strtok(NULL, ",");
    }

    fclose(file_pointer);
}

/**
 * @brief Create a vector from a file
 * 
 * Create a vector from a file using a combination of methods
 * 
 * @param file_name The name of the CSV file to create the vector from
 * 
 * @return Vector*
 * @note The caller is reponsible for freeing this memory using free_vector()
 */
Vector* create_vector_from_file(char* file_name) {
    int rows = 0;
    _put_vector_dimension(file_name, &rows);

    Vector* vec = create_empty_vector(rows);
    _populate_vector(vec, file_name);

    return vec;
}

/**
 * @brief Compute the dot product (inner product) of two vectors
 * 
 * @param a A vector to compute the dot product with.
 * @param b A vector to compute the dot product with.
 * @param c A double where the results will be stored.
 * 
 * @return int The resulting status code
 */
int dot_product(Vector* a, Vector* b, double* c) {
    if (a->rows != b->rows) {
        fprintf(stderr, "Dot Product: Mismatch of data types %lu vs %lu\n", a->rows, b->rows);
        return EXIT_FAILURE;
    }

    *c = 0.0;

    for (size_t i=0; i < a->rows; i++) {
        *c += (a->data[i] * b->data[i]);
    }

    return EXIT_SUCCESS;
}