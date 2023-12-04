#include <assert.h>
#include <ctype.h> // isdigit
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> // memset

#define NLINES 4
#define NCHARS 150
#define NPARTS 1200

static bool issymbol(char c) {
    return c != '\0' && c != '.' && !isdigit(c);
}

static int parse_part_numbers(const char* above, const char* current, const char* below, int iparts, int out_parts[NPARTS]) {
    int n = 0;
    int num = -1;
    bool is_part = false;

    for (int i = 1; i < NCHARS; ++i) {
        if (isdigit(current[i])) {
            if (num < 0) num = current[i] - '0';
            else num = num * 10 + (current[i] - '0');

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
            if (num >= 0) {
                if (is_part) {
                    assert(iparts + n < NPARTS);
                    out_parts[iparts + n++] = num;
                }

                num = -1;
                is_part = false;
            }
        }
    }

    return n;
}

int main(void) {
    char *line = NULL;
    size_t ngetline = 0;
    int nchars = 0;

    char schematic[NLINES][NCHARS] = {0};

    int ir = 3;
    int jr = 0;

    int part_nums[NPARTS];
    int nparts = 0;

    while ((nchars = (int)getline(&line, &ngetline, stdin)) > 0) {
        if (line[nchars - 1] == '\n') line[--nchars] = '\0';
        assert((nchars + 1) < (NCHARS - 1));

        memcpy(&schematic[ir][1], line, nchars); // start at 1 to skip the need for boundary checks

        ir = (ir + 1) & 3;

        nparts += parse_part_numbers(
            schematic[jr],
            schematic[(jr + 1) & 3],
            schematic[(jr + 2) & 3],
            nparts,
            part_nums
        );

        jr = (jr + 1) & 3;
    }

    for (int k = 0; k < NLINES; k++) {
        memset(schematic[ir], 0, NCHARS);

        ir = (ir + 1) & 3;

        nparts += parse_part_numbers(
            schematic[jr],
            schematic[(jr + 1) & 3],
            schematic[(jr + 2) & 3],
            nparts,
            part_nums
        );

        jr = (jr + 1) & 3;
    }

    free(line);

    int64_t sum = 0;

    for (int i = 0; i < nparts; ++i) sum += part_nums[i];

    printf("%lld\n", sum);

    return 0;
}
