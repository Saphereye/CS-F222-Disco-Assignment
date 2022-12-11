/**
 * @file Team15.c
 * @brief Reads details about 2 graphs and tells if isomorphic or not
 * @author Adarsh Das (2021A7PS1511H)
 * @author Shivam Atul Trivedi (2021A7PS1512H)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */

/**
 * @brief Panics and closes program, based on error message 
 * 
 * @param arr 
 */
void panic(const char* arr) {
    printf("%s%s%s\n", RED, arr, RESET);
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
 * @brief Compare two matrices
 *
 * @param m1
 * @param m2
 */
bool is_equal_matrix(const Matrix* m1, const Matrix* m2) {
    for(size_t i = 0; i < m1->size ; i++){
        for(size_t j = 0; j < m1->size; j++){
            if(m1->matrix[i][j] != m2->matrix[i][j])
                return false;
        }
    }

    return true;
}

/**
 * @brief Read from file and write to matrix
 * 
 * @param file_name 
 * @return Matrix 
 */
Matrix read_file_to_matrix(const char* file_name) {
    FILE* file;

    file = fopen(file_name, "r");

    // Checking for failure
    if (file == NULL) {
        printf("%sFile %s could not be opened%s\n", RED, file_name, RESET);
        exit(EXIT_FAILURE);
    }

    size_t num_of_nodes, num_of_edges, parent, child;

    // Get the num of nodes and edges
    fscanf(file, "%ld\n%ld\n", &num_of_nodes, &num_of_edges);

    // Initialize adjacency matrix
    Matrix m = create_matrix(num_of_nodes, num_of_edges);

    // Calling inputs from file line by line till EOF
    while(1) {
        if (fscanf(file, "%ld %ld\n", &parent, &child) == EOF) break;

        if((parent > num_of_nodes) || (child > num_of_nodes)) {
            printf("%sVertex index(%ld or %ld) exceeds total number of vertices(%ld)%s\n", RED, parent, child, num_of_nodes, RESET);
            exit(EXIT_FAILURE);
        }

        // Fill the adjacency matrix
        m.matrix[parent-1][child-1] = 1;
        m.matrix[child-1][parent-1] = 1;
    }

    fclose(file);

    return m;
}

/**
 * @brief Permutates the array to it's next lexicographical permutation
 * 
 * @param arr 
 * @param array_size 
 * @return bool Returns 0 if all permutations are done
 */
bool goto_next_permutation(size_t* arr, size_t array_size) {
    // Find the longest decreasing suffix
    int i = array_size - 1;
    while (i > 0 && arr[i - 1] >= arr[i]) {
        i -= 1;
    }
    if (i <= 0) {
        return false;  // The array is already at the last permutation
    }

    // Find the rightmost element that is greater than the pivot
    int j = array_size - 1;
    while (arr[j] <= arr[i - 1]) {
        j -= 1;
    }

    // Swap the pivot with the rightmost element that is greater than the pivot
    int temp = arr[i - 1];
    arr[i - 1] = arr[j];
    arr[j] = temp;

    // Reverse the suffix
    int start = i, end = array_size - 1;
    while (start < end) {
        temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start += 1;
        end -= 1;
    }

    return true;
}

int main(int argc, char** argv) {
    if (argc <= 1) panic("Input file not provided");

    Matrix m1 = read_file_to_matrix(argv[1]);
    Matrix m2 = read_file_to_matrix(argv[2]);
    
    // Check if number of edges and nodes are same or not, if not then not ISOMORPHIC
    if(!(m1.size == m2.size && m1.edges == m2.edges)) {
        printf("Not Isomorphic :(\n");
        exit(EXIT_SUCCESS);
    }

    // Make an array from 1..m1.size-1, signifying the nodes and their subsequent permutation
    size_t* num_array = (size_t*)calloc(m1.size, sizeof(size_t));

    // Initialise array as [1, 2, 3,..., m1.size]
    for(size_t i = 0; i < m1.size; i++)
        num_array[i] = i;
    
    // While all permutation have not been checked
    do {
        Matrix new_matrix = create_matrix(m1.size, m1.edges);

        for(size_t i = 0; i < m1.size ; i++)
            for(size_t j = 0; j < m1.size; j++)
                new_matrix.matrix[num_array[i]][num_array[j]] = m1.matrix[i][j];

        // Compare with m2.matrix, if match then Isomorphic, also print the bijection
        if(is_equal_matrix(&new_matrix, &m2)) {
            printf("Isomorphic\n");
            for(size_t i = 0; i < m1.size; i++)
                printf("%ld %ld\n", i+1, num_array[i]+1);
            exit(EXIT_SUCCESS);

        }
    } while (goto_next_permutation(num_array, m1.size));
    
    free(num_array);

    // If not found then not isomorphic
    printf("Not Isomorphic :(\n");
    exit(EXIT_SUCCESS);
}
