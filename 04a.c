#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NNUMS 10

int main(void) {
    char *line = NULL;
    size_t ngetline;

    int64_t sum = 0;

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

        sum += nmatches > 0
                ? (int64_t)(1 << (nmatches - 1))
                : 0;
    }

    free(line);

    printf("%lld\n", sum);

    return 0;
}
