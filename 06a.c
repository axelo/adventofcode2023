#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define NRACES 4

static int parse_numbers(int nchars, const char line[nchars], int inums, int n, int out_nums[n]) {
    int nnums = 0;
    int num = 0;

    for (int i = 0; i < nchars; ++i) {
        if (isdigit(line[i])) num = num * 10 + (line[i] - '0');
        else {
            if (num > 0) {
                assert(inums + nnums < n);
                out_nums[inums + nnums++] = num;

                num = 0;
            }
        }
    }

    return nnums;
}

int main(void) {
    char *line = NULL;
    size_t ngetline;

    int times[NRACES] = {0};
    int dists[NRACES] = {0};

    int nchars = (int)getline(&line, &ngetline, stdin);

    int nraces = parse_numbers(nchars, line, 0, NRACES, times);

    nchars = (int)getline(&line, &ngetline, stdin);

    int ndists = parse_numbers(nchars, line, 0, NRACES, dists);
    assert(ndists == nraces);

    int64_t error_margin = 1;

    for (int i = 0 ; i < nraces; ++i) {
        int nways = 0;

        for (int v = 0; v <= times[i]; ++v) {
            int d = 0;

            for (int s = v; s < times[i]; ++s)
                d += v;

            if (d > dists[i]) ++nways;
        }

        error_margin *= nways;
    }

    free(line);

    printf("%lld\n", error_margin);

    return 0;
}
