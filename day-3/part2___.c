#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// char *extract_str(char *startSearch, char *endSearch) {
//     size_t length = endSearch - startSearch + 1;
//     char *temp = malloc(length + 1);
//     if (temp == NULL) {
//         printf("Failed to allocate memory\n");
//         return NULL;
//     }
//     temp[length] = '\0';
//     memcpy(temp, startSearch, length);
//     return temp;
// }

// int find_num_from_start_ptr(char *start_ptr) {
//     if (start_ptr == NULL) {
//         printf("Something is wrong");
//     }
//     char *end_ptr = start_ptr;
//     while (*end_ptr >= '0' && *end_ptr <= '9') {
//         // printf("%c", *end_ptr);
//         end_ptr++;
//     }
//     char *temp = extract_str(start_ptr, end_ptr - 1);
//     int num = (int)strtol(temp, (char **)NULL, 10);
//     free(temp);
//     return num;
// }

// int edit_parts_ptr(char **first_ptr, char **second_ptr, char *search_ptr) {
//     if (*search_ptr < '0' || *search_ptr > '9') {
//         return 0;
//     } else {
//         while (*search_ptr >= '0' && *search_ptr <= '9') {
//             search_ptr--;
//         }
//         search_ptr++;
//     }

//     if (search_ptr != NULL) {
//         if (*first_ptr == NULL) {
//             *first_ptr = search_ptr;
//             // printf("%c", *search_ptr);
//         } else if (*second_ptr == NULL) {
//             *second_ptr = search_ptr;
//         } else if (*first_ptr != search_ptr && *second_ptr != search_ptr) {
//             return 1;
//         }
//     }
//     return 0;
// }

int main(int argc, char *argv[]) {
    char buffer[1024];
    printf("yes\n");
    char *input = NULL;
    int result = 0;
    // COL_COUNT does include '\n'
    int COL_COUNT;
    int row;

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Failed to open file\n");
        return 1;
    }

    for (row = 0; fgets(buffer, sizeof(buffer), file) != NULL; row++) {
        if (!COL_COUNT) COL_COUNT = (int) strlen(buffer);
        char *tmp = realloc(input, (size_t) COL_COUNT * (row + 1));
        if (tmp == NULL) {
            return 1;
        } else {
            input = tmp;
        }
        char *p_inputIdx = input + row * COL_COUNT;

        for (char *linePtr = buffer; *linePtr != '\0'; linePtr++) {
            *(p_inputIdx++) = *linePtr;
        }
    }

    for (int r = 0; r < row; r++) {
        for (int c = 0; c < COL_COUNT; c++) {
            printf("%c", *(input + (r * COL_COUNT + c)));
        }
    }

    // int number;
    // int colLeft, colRight, rowAbove, rowBelow;
    // int valid;

    // for (int r = 0; r < row; r++) {
    //     for (int c = 0; c < COL_COUNT; c++) {

    //         char *p_one = NULL;
    //         char *p_two = NULL;
    //         char current = *(input + (r * COL_COUNT + c));
    //         printf("%cdfasdf", current);
    //         // printf("yes\n");
    //         if (current == '*') {
    //             valid = 1;
    //             colLeft = c - 1 >= 0 ? c - 1 : c;
    //             rowAbove = r - 1 >= 0 ? r - 1 : r;
    //             rowBelow = r + 1 >= 0 ? r + 1 : r;
    //             printf("yes");
    //             for (colRight = c + 1; colLeft <= colRight; colLeft++) {
    //                 char *p_rowAbove =
    //                     (r - 1 < 0) ? NULL
    //                                 : input + (rowAbove * COL_COUNT + colLeft);
    //                 char *p_row = input + (r * COL_COUNT + colLeft);
    //                 char *p_rowBelow =
    //                     (r + 1 > row)
    //                         ? NULL
    //                         : input + (rowBelow * COL_COUNT + colLeft);
    //                 printf("yes");

    //                 if (edit_parts_ptr(&p_one, &p_two, p_rowAbove) ||
    //                     edit_parts_ptr(&p_one, &p_two, p_row) ||
    //                     edit_parts_ptr(&p_one, &p_two, p_rowBelow)) {
    //                     valid = 0;
    //                     break;
    //                 }
    //             }
    //             if (p_one == NULL || p_two == NULL) {
    //                 valid = 0;
    //             }
    //         }
    //         if (valid) {
    //             int num_one = find_num_from_start_ptr(p_one);
    //             int num_two = find_num_from_start_ptr(p_two);
    //             result += num_one * num_two;
    //         }
    //     }
    // }
    free(input);
    // printf("Res: %d\n", result);
    return 0;
}