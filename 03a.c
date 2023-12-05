#include <assert.h>
#include <ctype.h> // isdigit
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> // memset

#define NLINES 4
#define NMASK  (NLINES - 1)
#define NCHARS 150

static bool issymbol(char c) {
    return c != 0 && c != '.' && !isdigit(c);
}

static int64_t parse_part_numbers(const char above[NCHARS], const char current[NCHARS], const char below[NCHARS]) {
    int64_t sum = 0;
    int num = 0;
    bool is_part = false;

    for (int i = 1; i < NCHARS; ++i) {
        if (isdigit(current[i])) {
            num = num * 10 + (current[i] - '0');

            is_part =
                is_part                  ||
                issymbol(above[i - 1])   ||
                issymbol(above[i])       ||
                issymbol(above[i + 1])   ||
                issymbol(current[i - 1]) ||
                issymbol(current[i + 1]) ||
                issymbol(below[i - 1])   ||
                issymbol(below[i])       ||
                issymbol(below[i + 1]);
        } else {
            if (is_part) sum += num;

            num = 0;
            is_part = false;
        }
    }

    return sum;
}

int main(void) {
    char *line = NULL;
    size_t ngetline = 0;

    int64_t sum = 0;

    int ir = 3;
    int jr = 0;
    int nchars = 0;

    char schematic[NLINES][NCHARS] = {0};

    while ((nchars = (int)getline(&line, &ngetline, stdin)) > 0) {
        if (line[nchars - 1] == '\n') line[--nchars] = 0;
        assert((nchars + 1) < (NCHARS - 1));

        memcpy(&schematic[ir++ & NMASK][1], line, nchars); // start at 1 to skip the need for boundary checks

        sum += parse_part_numbers(
            schematic[jr       & NMASK],
            schematic[(jr + 1) & NMASK],
            schematic[(jr + 2) & NMASK]
        );

        ++jr;
    }

    for (int k = 0; k < NLINES; k++) {
        memset(schematic[ir++ & NMASK], 0, NCHARS);

        sum += parse_part_numbers(
            schematic[jr       & NMASK],
            schematic[(jr + 1) & NMASK],
            schematic[(jr + 2) & NMASK]
        );

        ++jr;
    }

    free(line);

    printf("%lld\n", sum);

    return 0;
}
