/**
 * @file matrix.h
 * @brief 
 * @version 0.1
 * @date 2022-12-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <time.h>
#include "helper.h"

/**
 * @brief Mtrix implementation
 * 
 */
typedef struct Matrix {
    int     rowSize;
    int     columnSize;
    long int**    matrix;
} Matrix;

/**
 * @brief Create a matrix object
 * 
 * @param rows 
 * @param cols 
 * @return Matrix 
 */
Matrix create_matrix(int rows, int cols) {
    Matrix temp = {rows, cols, (long int **)calloc(rows, sizeof(long int *))};

    if (temp.matrix == NULL)
        panic("Matrix not initialised");

    for (int i = 0; i < rows; i++) {
        temp.matrix[i] = (long int *)calloc(cols, sizeof temp.matrix[i][0]);

        if (temp.matrix[i] == NULL)
            panic("Matrix row not initialised");
    }

    return temp;
}

/**
 * @brief Randomize given matrix
 * 
 * @param m 
 */
void randomize_matrix(Matrix *m){
    int i,j;
    for(i = 0; i < m->rowSize ; i++){
        for(j = 0; j < m->columnSize; j++){
            struct timespec ts;
            timespec_get(&ts, TIME_UTC);
            srand(ts.tv_nsec);
            m->matrix[i][j] = rand() % 10;
        }
    }
}

/**
 * @brief Create a random matrix object
 * 
 * @param rows 
 * @param cols 
 * @return Matrix 
 */
Matrix create_rand_matrix(int rows, int cols) {
    Matrix temp = create_matrix(rows,cols);
    randomize_matrix(&temp);
    return temp;
}

/**
 * @brief Print matrix value to stdout
 * 
 * @param m 
 */
void print_matrix(const Matrix* m) {
    for(int i = 0; i < m->rowSize ; i++){
        for(int j = 0; j < m->columnSize; j++){
            printf("%li ", m->matrix[i][j]);
        }
        printf("\n");
    }
}

Matrix add_matrix(const Matrix* a, const Matrix* b);

Matrix sub_matrix(const Matrix* a, const Matrix* b);

Matrix mul_matrix(const Matrix* a, const Matrix* b);


#endif

