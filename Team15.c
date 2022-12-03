/**
 * @file Team15.c
 * @brief Reads details about a graph and outputs the degree sequence
 * @author Adarsh Das (2021A7PS1511H)
 * @author Shivam Atul Trivedi (2021A7PS1512H)
 */

#include <stdio.h>
#include "matrix.h"
#include "helper.h"

int main(int argc, char** argv) {
    if (argc <= 1) panic("Input file not provided");

    // Initialising the file
    FILE* file;
    int num;
    file = fopen(argv[1], "r");

    // Checking for failure
    if (file == NULL) panic("File could not be opened");
        

    // Get the num of nodes and edges
    fscanf(file, "%d\n", &num); int num_of_nodes = num;
    fscanf(file, "%d\n", &num); int num_of_edges = num;

    // Initialize adjacency matrix
    Matrix m = create_matrix(num_of_nodes, num_of_nodes);

    // Calling inputs from file line by line till EOF
    while(1) {
        if (fscanf(file, "%d ", &num) == EOF) break;
        int parent = num;

        if (fscanf(file, "%d\n", &num) == EOF) break;
        int child = num;

        // Fill the adjacency matrix
        m.matrix[parent-1][child-1] = 1;
        m.matrix[child-1][parent-1] = 1;
    }
    
    // Closing the file
    fclose(file);

    // Get the degress of each edge
    int node_degree[num_of_nodes];
    for(int node = 0; node < num_of_nodes; node++) {
        // Count the degree
        int degree = 0;
        for(int neighbour = 0; neighbour < num_of_nodes; neighbour++) {
            degree += m.matrix[node][neighbour];
        }
        node_degree[node] = degree;
    }

    // Sort in non-increasing fashion
    for(int i = 0; i < num_of_nodes-1; i++) {
        for(int j = 0; j < num_of_nodes-i-1; j++) {
            if(node_degree[j] < node_degree[j+1]) {
                // Swap the contents
                node_degree[j] ^= node_degree[j+1];
                node_degree[j+1] ^= node_degree[j];
                node_degree[j] ^= node_degree[j+1];
            }
        }
    }

    // Print array
    for(int i = 0; i < num_of_nodes; i++) {
        printf("%d ", node_degree[i]);
    }
    printf("\n");

    exit(EXIT_SUCCESS);
}