// bin: -Wall -Wextra

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define FLAG_NAME "flags.h"
#define PACMIRROR_FILE "dpacker.c"

#define ARRAY_LEN(a) sizeof((a)) / sizeof((a[0]))
#define STRLEN(s) ARRAY_LEN(("" s "")) - sizeof((s)[0])

int main(void) {
    char c = 0;

    const char *file = PACMIRROR_FILE;

    FILE *output = fopen(FLAG_NAME, "w");
    assert(output && "failed to open file: " FLAG_NAME);

    FILE *fp = fopen(file, "r");
    assert(fp && "failed to open file: " PACMIRROR_FILE);

    char *buff = malloc(sizeof(char) * 1024);
    assert(buff && "out of memory");
    size_t buff_size = 0;

    while ((c = fgetc(fp)) != EOF) {
        buff[buff_size++] = c;
        if (c == '\n') {
            size_t i = 0;
            bool has_define = false;

            if (memcmp(buff, "#define", STRLEN("#define")) == 0) {
                has_define = true;
                i = STRLEN("#define");
            } else if (memcmp(buff, "// #define", STRLEN("// #define")) == 0) {
                has_define = true;
                i = STRLEN("// #define");
            } else if (memcmp(buff, "// END OF DEFINES", STRLEN("// END OF DEFINES")) == 0) {
                break;
            }

            if (has_define) {
                for (; i < buff_size; i++) {
                    if (buff[i] != ' ') break;
                }
                const char *flag_name = buff + i;
                const size_t flag_size = buff_size - i - 1;

                char *flag_name_low = malloc(sizeof(char) * flag_size);
                memcpy(flag_name_low, flag_name, flag_size);

                for (size_t j = 0; j < flag_size; j++) {
                    flag_name_low[j] = tolower(flag_name_low[j]);
                }

                fprintf(output, "#ifdef %.*s\n", (int)flag_size, flag_name);
                fprintf(output, "#define %.*s(...) __VA_ARGS__\n", (int)flag_size, flag_name_low);
                fprintf(output, "#else\n");
                fprintf(output, "#define %.*s(...) \"0\"\n", (int)flag_size, flag_name_low);
                fprintf(output, "#endif\n\n");
            }
            buff_size = 0;
        }
    }

    assert(fp && "invalid pointer");
    assert(output && "invalid pointer");
    fclose(fp);
    fclose(output);
    return 0;
}
