#ifndef HELPER_H
#define HELPER_H

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */

/**
 * @brief Panics and closes program, based on error message 
 * 
 * @param arr 
 */
void panic(const char arr[]) {
    printf(RED);
    printf("%s", arr);
    printf(RESET);
    printf("\n");
    exit(EXIT_FAILURE);
}

#endif