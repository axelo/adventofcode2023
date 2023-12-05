#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define NNUMS 10
#define NSLOTS 16
#define NMASK  (NSLOTS - 1)

int main(void) {
    char *line = NULL;
    size_t ngetline;

    int64_t sum = 0;

    int ncards[NSLOTS] = {0};

    int icard = 0; // assume cards come in asc order

    int nchars;

    while ((nchars = (int)getline(&line, &ngetline, stdin)) > 0) {
        ++nchars; // count zero char to fully parse last digit on line

        int win_nums[NNUMS] = {0}; // assume 0 never appears
        int nnums = 0;

        int num = 0;

        int i = 0;
        for (; line[i] != ':' && i < nchars; ++i);

        for (; i < nchars; ++i) {
            if (isdigit(line[i])) num = num * 10 + (line[i] - '0');
            else {
                if (num > 0) {
                    assert(nnums < NNUMS);
                    win_nums[nnums++] = num;

                    num = 0;
                }

                if (line[i] == '|') break;
            }
        }

        int ncopies = ++ncards[icard & NMASK];

        sum += ncards[icard & NMASK];

        ncards[icard++ & NMASK] = 0; // clear card slot and go to next

        int nmatches = 0;

        for (; i < nchars; ++i) {
            if (isdigit(line[i])) num = num * 10 + (line[i] - '0');
            else {
                if (num > 0)
                    for (int j = 0; j < nnums; ++j)
                        if (win_nums[j] == num) ++nmatches;

                num = 0;
            }
        }

        assert(nmatches < NSLOTS);

        for (int j = icard; j < icard + nmatches; ++j)
            ncards[j & NMASK] += ncopies;
    }

    free(line);

    printf("%lld\n", sum);

    return 0;
}
