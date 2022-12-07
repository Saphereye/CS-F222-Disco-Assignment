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

int is_equal_matrix(const Matrix* m1, const Matrix* m2) {
    for(size_t i = 0; i < m1->size ; i++){
        for(size_t j = 0; j < m1->size; j++){
            if(m1->matrix[i][j] != m2->matrix[i][j])
                return 0;
        }
    }

    return 1;
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
    if (file == NULL) {
        printf("%sFile %s could not be opened%s\n", RED, file_name, RESET);
        exit(EXIT_FAILURE);
    }

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
 * @brief Get the nth permutation object
 * 
 * @param array 
 * @param array_size 
 * @param nth_permutation from 0 to (n!)-1
 * @return int* 
 */
size_t* get_nth_permutation(const size_t* array, size_t array_size, size_t nth_permutation) {
    int j, k = 0;
    size_t *fact = (size_t*)calloc(array_size, sizeof(size_t));
    size_t *perm = (size_t*)calloc(array_size, sizeof(size_t));

    // compute factorial numbers
    fact[k] = 1;
    while (++k < array_size)
        fact[k] = fact[k - 1] * k;

    // compute factorial code
    for (k = 0; k < array_size; ++k)
    {
        perm[k] = nth_permutation / fact[array_size - 1 - k];
        nth_permutation = nth_permutation % fact[array_size - 1 - k];
    }

    // readjust values to obtain the permutation
    // start from the end and check if preceding values are lower
    for (k = array_size - 1; k > 0; --k)
        for (j = k - 1; j >= 0; --j)
            if (perm[j] <= perm[k])
                perm[k]++;

    size_t* permutated_array = (size_t*)calloc(array_size, sizeof(size_t));

    // print permutation
    for (k = 0; k < array_size; ++k)
        permutated_array[k] = array[perm[k]];

    free(fact);
    free(perm);

    return permutated_array;
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
 * @brief Return factorial of num
 * 
 * @param n 
 * @return size_t 
 */
size_t factorial(size_t n) {
    size_t output = 1;
    for(size_t i = 1; i <= n; i++)
        output *= i;
    return output;
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

    for(size_t i = 0; i < m1.size; i++) {
        num_array[i] = i;
    }

    for(int num = 0; num < factorial(m1.size); num++) {
        // Generate appropriately permutated matrix of m1 and store in m_new
        size_t* permutation = get_nth_permutation(num_array, m1.size, num);

        Matrix new_matrix = create_matrix(m1.size, m1.edges);

        for(size_t i = 0; i < m1.size ; i++){
            for(size_t j = 0; j < m1.size; j++){
                new_matrix.matrix[permutation[i]][permutation[j]] = m1.matrix[i][j];
            }
        }

        // Compare with m2.matrix, if match then Isomorphic, also print the bijection
        if(is_equal_matrix(&new_matrix, &m2)) {
            printf("Isomorphic\n");
            for(int i = 0; i < m1.size; i++) {
                printf("%d %ld\n", i+1, permutation[i]+1);
            }
            exit(EXIT_SUCCESS);

        }
    }

    // If not found then not isomorphic
    printf("Not Isomorphic :(\n");
    exit(EXIT_SUCCESS);
}