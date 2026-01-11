# About this project
This project is a learning exercise in machine learning and the C language. Currently, only Ordinary Least Squares in implemented through the pseudoinverse method.

- In `vector.h`, a vector structure is defined, as well as constructor, destructor, and other methods related to vectors.
- In `matrix.h`, a matrix structure is defined, as well as constructor, destructor, and other methods related to matrices.
    - Highlights of this include gauss-jordan elimination and matrix inversion.
- In `regressions.h`, an ordinary least squares method is defined which calculates a linear regression analytically from matrix methods.

# Running this project
To run this project, do the following:
* `make all`
* `make generate`
* `./generate`
* `./ml_app full_rank_matrix.csv target_vector.csv`