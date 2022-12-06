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
    size_t size;
    size_t edges;
    size_t** matrix;
} Matrix;

/**
 * @brief Create a matrix object
 * 
 * @param size 
 * @param edges 
 * @return Matrix 
 */
Matrix create_matrix(size_t size, size_t edges) {
    Matrix temp = {size, edges, (size_t**)calloc(size, sizeof(size_t*))};

    if (temp.matrix == NULL)
        panic("Matrix not initialised");

    for (size_t i = 0; i < size; i++) {
        temp.matrix[i] = (size_t*)calloc(size, sizeof temp.matrix[i][0]);

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
    for(size_t i = 0; i < m->size ; i++){
        for(size_t j = 0; j < m->size; j++){
            printf("%li ", m->matrix[i][j]);
        }
        printf("\n");
    }
}

/**
 * @brief Read from file and write to matrix
 * 
 * @param file_name 
 * @return Matrix 
 */
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
    Matrix m = create_matrix(num_of_nodes, num_of_edges);

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

/**
 * @brief Get the degree sequence object
 * 
 * @param m 
 * @return size_t* 
 */
size_t* get_degree_sequence(const Matrix* m) {
    size_t num_of_nodes = m->size;

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

/**
 * @brief Print array in pretty way
 * 
 * @param array 
 * @param size 
 */
void print_array(const size_t* array, size_t size) {
    for(size_t i = 0; i < size; i++) {
        printf("%ld ", array[i]);
    }
    printf("\n");
}

/**
 * @brief Rotate array in clockwise manner
 * 
 * @param array 
 * @param array_size 
 */
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

void show_bijection_table(size_t offset, size_t max_size) {
    for(int i = 1; i <= max_size; i++) {
        printf("%d %d\n", i, (i+offset)%max_size);
    }
}

int main(int argc, char** argv) {
    if (argc <= 1) panic("Input file not provided");
    Matrix m1 = read_file_to_matrix(argv[1]);
    Matrix m2 = read_file_to_matrix(argv[2]);
    print_matrix(&m1);
    printf("\n");
    print_matrix(&m2);

    if(!(m1.size == m2.size && m1.edges == m2.edges)) {
        printf("Not Isomorphic :(\n");
        exit(EXIT_SUCCESS);
    }

    size_t* degree_sequence_of_matrix_1 = get_degree_sequence(&m1);
    size_t* degree_sequence_of_matrix_2 = get_degree_sequence(&m2);

    printf("Matrix 1: ");
    print_array(degree_sequence_of_matrix_1, m1.size);
    printf("Matrix 2: ");
    print_array(degree_sequence_of_matrix_2, m2.size);

    clockwise_rotate_array_once(degree_sequence_of_matrix_1, m1.size);
    size_t count = 0;
    for(count = 0; count < m2.size; count++) {
        printf("Matrix 2 after rot: ");
        print_array(degree_sequence_of_matrix_2, m2.size);
        if(compare_sizet_arrays(degree_sequence_of_matrix_1, degree_sequence_of_matrix_2, m1.size, m2.size) == 0) {
            printf("Isomorphic :)\n");
            printf("Rots: %ld\n", count);
            show_bijection_table(count-1, m2.size);
            exit(EXIT_SUCCESS);
        }
        clockwise_rotate_array_once(degree_sequence_of_matrix_2, m2.size);
    }
    printf("Rots: %ld\n", count);

    free(degree_sequence_of_matrix_1);
    free(degree_sequence_of_matrix_2);

    printf("Not Isomorphic :(\n");
    exit(EXIT_SUCCESS);
}