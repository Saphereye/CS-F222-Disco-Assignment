#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */

/**
 * @brief Panics and closes program, based on error message 
 * 
 * @param arr 
 */
void panic(const char* arr) {
    printf(RED);
    printf("%s", arr);
    printf(RESET);
    printf("\n");
    exit(EXIT_FAILURE);
}

/**
 * @brief Matrix implementation
 * 
 */
typedef struct Matrix {
    size_t     row_size;
    size_t     column_size;
    size_t**    matrix;
} Matrix;

/**
 * @brief Create a matrix object
 * 
 * @param rows 
 * @param cols 
 * @return Matrix 
 */
Matrix create_matrix(size_t rows, size_t cols) {
    Matrix temp = {rows, cols, (size_t**)calloc(rows, sizeof(size_t*))};

    if (temp.matrix == NULL)
        panic("Matrix not initialised");

    for (size_t i = 0; i < rows; i++) {
        temp.matrix[i] = (size_t*)calloc(cols, sizeof temp.matrix[i][0]);

        if (temp.matrix[i] == NULL)
            panic("Matrix row not initialised");
    }

    return temp;
}

/**
 * @brief Print matrix value to stdout
 * 
 * @param m 
 */
void print_matrix(const Matrix* m) {
    for(size_t i = 0; i < m->row_size ; i++){
        for(size_t j = 0; j < m->column_size; j++){
            printf("%li ", m->matrix[i][j]);
        }
        printf("\n");
    }
}

Matrix read_file_to_matrix(const char* file_name) {
    FILE* file;
    size_t num;

    file = fopen(file_name, "r");

    // Checking for failure
    if (file == NULL) panic("File could not be opened");

    // Get the num of nodes and edges
    fscanf(file, "%ld\n", &num); size_t num_of_nodes = num;
    fscanf(file, "%ld\n", &num); size_t num_of_edges = num;

    // Initialize adjacency matrix
    Matrix m = create_matrix(num_of_nodes, num_of_nodes);

    // Calling inputs from file line by line till EOF
    while(1) {
        if (fscanf(file, "%ld ", &num) == EOF) break;
        size_t parent = num;

        if (fscanf(file, "%ld\n", &num) == EOF) break;
        size_t child = num;

        // Fill the adjacency matrix
        m.matrix[parent-1][child-1] = 1;
        m.matrix[child-1][parent-1] = 1;
    }

    fclose(file);

    return m;
}

size_t* get_degree_sequence(const Matrix* m) {
    size_t num_of_nodes = m->row_size;

    // Get the degress of each edge
    size_t* node_degree = (size_t*)calloc(num_of_nodes, sizeof(size_t));
    for(size_t node = 0; node < num_of_nodes; node++) {
        // Count the degree
        size_t degree = 0;
        for(size_t neighbour = 0; neighbour < num_of_nodes; neighbour++) {
            degree += m->matrix[node][neighbour];
        }
        node_degree[node] = degree;
    }
    return node_degree;
}

void print_array(const size_t* array, size_t size) {
    for(size_t i = 0; i < size; i++) {
        printf("%ld ", array[i]);
    }
    printf("\n");
}

void clockwise_rotate_array_once(size_t* array, size_t array_size) {
    size_t last_element = array[array_size-1];

    for(size_t index = array_size-1; index > 0; index--) {
        array[index] = array[index-1];
    }
    array[0] = last_element;
}

/**
 * @brief Return 1 if array1 > array2, return -1 if array1 < array2, 0 if equal
 * 
 * @param array1 
 * @param array2 
 * @param array1_size 
 * @param array2_size 
 * @return int 
 */
int compare_sizet_arrays(const size_t* array1, const size_t* array2, size_t array1_size, size_t array2_size) {
    if(array1_size > array2_size) {
        return 1;
    } else if(array1_size < array2_size) {
        return -1;
    } else {
        for(size_t index = 0; index < array1_size; index++) {
            if(array1[index] > array2[index])
                return 1;
            if(array1[index] < array2[index])
                return -1;
        }
        return 0;
    }
}

int main(int argc, char** argv) {
    if (argc <= 1) panic("Input file not provided");
    printf("Hello Wolrd");
    Matrix m1 = read_file_to_matrix(argv[1]);
    Matrix m2 = read_file_to_matrix(argv[2]);

    size_t* degree_sequence_of_matrix_1 = get_degree_sequence(&m1);
    size_t* degree_sequence_of_matrix_2 = get_degree_sequence(&m2);

    printf("Matrix 1: ");
    print_array(degree_sequence_of_matrix_1, m1.row_size);
    printf("Matrix 2: ");
    print_array(degree_sequence_of_matrix_2, m2.row_size);

    size_t array1[] = {3, 2, 3, 2, 2};
    size_t array2[] = {2, 2, 2, 2};
    printf("Comparison: %d\n", compare_sizet_arrays(array1, array2, 5, 4));

    clockwise_rotate_array_once(degree_sequence_of_matrix_1, m1.row_size);
    for(size_t count = 0; count < m2.row_size; count++) {
        if(compare_sizet_arrays(degree_sequence_of_matrix_1, degree_sequence_of_matrix_2, m1.row_size, m2.row_size) == 0) {
            printf("Isomorphic :)");
            exit(EXIT_SUCCESS);
        }
        clockwise_rotate_array_once(degree_sequence_of_matrix_2, m2.row_size);
    }

    free(degree_sequence_of_matrix_1);
    free(degree_sequence_of_matrix_2);

    printf("Not Isomorphic :(\n");
    exit(EXIT_SUCCESS);
}