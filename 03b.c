#include <assert.h>
#include <ctype.h> // isdigit
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> // memset

#define NLINES 4
#define NCHARS 150
#define NRATIOS 400

static int64_t parse_number(int i, const char *s) {
    if (!isdigit(s[i])) return -1;

    while (isdigit(s[--i])); // go west

    int num = 0;

    for (++i; isdigit(s[i]); ++i) num = num * 10 + (s[i] - '0');  // go east

    return num;
}

static int parse_gear_ratios(const char* above, const char* current, const char* below, int iratios, int64_t out_ratios[NRATIOS]) {
    int n = 0;
    int64_t numbers[8];

    for (int i = 1; i < NCHARS; ++i) {
        if (current[i] == '*') {
            numbers[0] = parse_number(i - 1, above);
            numbers[1] = !isdigit(above[i - 1]) ? parse_number(i, above)     : -1;
            numbers[2] = !isdigit(above[i])     ? parse_number(i + 1, above) : -1;
            numbers[3] = parse_number(i - 1, current);
            numbers[4] = parse_number(i + 1, current);
            numbers[5] = parse_number(i - 1, below);
            numbers[6] = !isdigit(below[i - 1]) ? parse_number(i, below)     : -1;
            numbers[7] = !isdigit(below[i])     ? parse_number(i + 1, below) : -1;

            int nnums = 0;
            int ratio = 1;

            for (int j = 0; j < 8; ++j) {
                if (numbers[j] >= 0) {
                    ratio *= numbers[j];
                    ++nnums;
                }
            }

            if (nnums == 2) {
                assert(iratios + n < NRATIOS);
                out_ratios[iratios + n++] = ratio;
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

    int ir = NLINES - 1; // start reading into the last line index, so we read in "nothing" until we got some real data
    int jr = 0;          // above, current, below lines

    int64_t ratios[NRATIOS];
    int nratios = 0;

    while ((nchars = (int)getline(&line, &ngetline, stdin)) > 0) {
        if (line[nchars - 1] == '\n') line[--nchars] = '\0';
        assert((nchars + 1) < (NCHARS - 1));

        memcpy(&schematic[ir][1], line, nchars); // start at 1 to skip the need for boundary checks

        ir = (ir + 1) & 3;

        nratios += parse_gear_ratios(
            schematic[jr],
            schematic[(jr + 1) & 3],
            schematic[(jr + 2) & 3],
            nratios,
            ratios
        );

        jr = (jr + 1) & 3;
    }

    // simulate reading NLINES empty lines to catch 'em all
    for (int k = 0; k < NLINES; k++) {
        memset(schematic[ir], 0, NCHARS);

        ir = (ir + 1) & 3;

        nratios += parse_gear_ratios(
            schematic[jr],
            schematic[(jr + 1) & 3],
            schematic[(jr + 2) & 3],
            nratios,
            ratios
        );

        jr = (jr + 1) & 3;
    }

    free(line);

    int64_t sum = 0;

    for (int i = 0; i < nratios; ++i) sum += ratios[i];

    printf("%lld\n", sum);

    return 0;
}
