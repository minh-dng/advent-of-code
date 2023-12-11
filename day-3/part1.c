#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *extract_str(char *startSearch, char *endSearch) {
    size_t length = endSearch - startSearch + 1;
    char *temp = malloc(length + 1);
    if (temp == NULL) {
        printf("Failed to allocate memory\n");
        return NULL;
    }
    temp[length] = '\0';
    memcpy(temp, startSearch, length);
    return temp;
}

int check_symbol(char *symbol_ptr) {
    char symbol = *symbol_ptr;
    return (!(symbol >= '0' && symbol <= '9') && symbol != '\0' && symbol != '\n' &&
        symbol != '.');
}

int main(int argc, char *argv[]) {
    char buffer[1024];
    char *input = NULL;
    char *input_index = input;
    int result = 0;
    // col_count does include '\n'
    size_t col_count = 0;
    int row;

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Failed to open file\n");
        return 1;
    }

    for (row = 0; fgets(buffer, sizeof(buffer), file) != NULL; row++) {
        if (!col_count) col_count = strlen(buffer);
        int col = 0;
        input = realloc(input, (size_t)col_count * (row + 1));

        if (input_index == NULL) input_index = input;

        char *linePtr;
        for (linePtr = buffer; *linePtr != '\0'; linePtr++) {
            *(input_index++) = *linePtr;
        }
        // If file does not end on an empty line. Fill all the realloc mem
        if (*(linePtr - 1) != '\n') {
            *input_index = '\0';
        }
    }

    // for (int r = 0; r < row; r++) {
    //     for (int c = 0; c < col_count; c++) {
    //         printf("%c", *(input + (r * col_count + c)));
    //     }
    // }

    int number;
    int c_l, c_r, r_a, r_b;
    char *start = NULL;
    int valid_part;
    for (int r = 0; r < row; r++) {
        for (int c = 0; c < col_count; c++) {
            char current = *(input + (r * col_count + c));
            if (current >= '0' && current <= '9') {
                if (start == NULL) {
                    start = input + (r * col_count + c);
                    c_l = c - 1 >= 0 ? c - 1 : c;
                    r_a = r - 1 >= 0 ? r - 1 : r;
                    r_b = r + 1 >= 0 ? r + 1 : r;
                    valid_part = 0;
                }
            } else if (start != NULL) {
                char *num_char =
                    extract_str(start, input + (r * col_count + c) - 1);
                c_r = c;
                for (; c_l <= c_r; c_l++) {
                    char *r_a_val = input + (r_a * col_count + c_l);
                    char *r_val = input + (r * col_count + c_l);
                    char *r_b_val = input + (r_b * col_count + c_l);
                    if (check_symbol(r_a_val) || check_symbol(r_b_val) || check_symbol(r_val)) {
                        valid_part = 1;
                        break;
                    }
                }
                if (valid_part) {
                    result += (int) strtol(num_char, (char**)NULL, 10);
                }
                free(num_char);
                start = NULL;
            }
        }
    }
    free(input);
    printf("Res: %d\n", result);
    return 0;
}