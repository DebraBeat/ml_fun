# About this project
This project is a learning exercise in machine learning and the C language. Currently, only Ordinary Least Squares in implemented through the pseudoinverse method.

## Files related to linear algebra structures and techniques
- In `vector.h`, a vector structure is defined, as well as constructor, destructor, and other methods related to vectors.
- In `matrix.h`, a matrix structure is defined, as well as constructor, destructor, and other methods related to matrices.
    - Highlights of this include gauss-jordan elimination and matrix inversion.
- In `regressions.h`, an ordinary least squares method is defined which calculates a linear regression analytically from matrix methods.

## Files related to testing and generating code
- In `test.c`, unit tests for the vector and matrix methods are defined and driven.
- In `generator.c`, csv files for a full rank matrix and corresponding target vector are created.
    - The target vector corresponds to the matrix as being the sum of the matrix's rows. That is given a matrix $X$ and vector $\vec{b}$ where every $b_i \in \vec{b} = 1$, the target vector is $X\vec{b}$

## Running this project
To run this project with a generated matrix, do the following:
- `make all`
- `./ml_app full_rank_matrix.csv target_vector.csv`
- **NOTE**: $\hat{y}$ should be all 1s (or abritraily close)

To run this project with a given CSV for a matrix and a CSV for a vector, do the following:
- (If make not run before)
- `make all`
- `./ml_app [your_matrix_filename].csv [your_vector_filename].csv`

To generate a random matrix of arbitrary size:
- `make generate`
- `./generate`

Testing the project:
- `make test`
- `./run_tests`